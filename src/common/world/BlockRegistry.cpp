#include <stdexcept>
#include "BlockRegistry.h"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "common/core/glm_glaze_support.h"
#include "client/rendering/materials/TextureArray.h"

namespace Mineclone {

    BlockRegistry::BlockRegistry(TextureArray& textureArray)
            : m_textureArray(textureArray) {
        BlockDefinition airDefinition;
        airDefinition.metadata.name = "air";
        airDefinition.metadata.displayName = "Air";
        airDefinition.metadata.placeable = false;
        airDefinition.metadata.breakable = false;
        airDefinition.metadata.solid = false;
        registerBlock(airDefinition);
    }

    bool BlockRegistry::loadFromDirectory(const std::string& modelsPath) {
        namespace fs = std::filesystem;
        if (!fs::exists(modelsPath) || !fs::is_directory(modelsPath)) {
            throw std::runtime_error("Invalid directory: " + modelsPath);
        }

        for(const auto& entry : fs::directory_iterator(modelsPath)) {
            if (entry.is_regular_file() &&
                (entry.path().extension() == ".json" || entry.path().extension() == ".block")) {
                try {
                    loadFromFile(entry);
                } catch (const std::exception& e) {
                    std::cerr << "Error loading " << entry.path() << ": " << e.what() << std::endl;
                }
            }
        }
        return true;
    }

    BlockId BlockRegistry::loadFromFile(const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        std::ostringstream oss;
        oss << file.rdbuf();
        std::string jsonText = oss.str();

        BlockDefinition definition;
        auto ec = glz::read_json(definition, jsonText);
        if (ec) {
            throw std::runtime_error("Glaze parse error in " + filePath.string() + ": " + glz::format_error(ec, jsonText));
        }

        processBlockDefinition(definition);

        std::cout << "\nLoaded block definition: " << definition.metadata.name << std::endl;
        printDefinitionInfo(definition);

        return registerBlock(definition);
    }

    BlockId BlockRegistry::loadFromFile(const std::string& filePath) {
        return loadFromFile(std::filesystem::path(filePath));
    }

    void BlockRegistry::processBlockDefinition(BlockDefinition& definition) {
        if (definition.metadata.displayName.empty()) {
            definition.metadata.displayName = definition.metadata.name;
        }

        expandTextureAliases(definition.model);
        resolveTextureReferences(definition.model, definition.metadata.name);
    }

    void BlockRegistry::expandTextureAliases(BlockModel& model) {
        for (auto& element : model.elements) {
            element.expandFaceGroup("all", {"up", "down", "north", "south", "west", "east"});
            element.expandFaceGroup("side", {"north", "south", "west", "east"});
            element.expandFaceGroup("top", {"up"});
            element.expandFaceGroup("bottom", {"down"});
        }
    }

    void BlockRegistry::resolveTextureReferences(BlockModel& model, const std::string& name) {
        for (auto& element : model.elements) {
            for (auto& [faceName, facedef] : element.faces) {
                if (facedef.texture.starts_with("#")) {
                    std::string alias = facedef.texture.substr(1);
                    if (auto it = model.textures.find(alias); it != model.textures.end()) {
                        facedef.texture = it->second;
                    } else {
                        std::cerr << "Warning: Unresolved texture alias '#" << alias
                                  << "' in model " << name << std::endl;
                    }
                }
            }
        }
    }

    void BlockRegistry::printDefinitionInfo(const BlockDefinition& definition) {
        const auto& metadata = definition.metadata;
        const auto& model = definition.model;

        std::cout << "  Display Name: " << metadata.displayName << std::endl;
        std::cout << "  Tags: ";
        for (const auto& tag : metadata.tags) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;

        if (model.elements.empty()) {
            std::cout << "  No model elements" << std::endl;
            return;
        }

        for (size_t i = 0; i < model.elements.size(); ++i) {
            const auto& element = model.elements[i];
            std::cout << "  Element " << i << ":" << std::endl;

            for (const auto& [faceName, facedef] : element.faces) {
                std::cout << "    Face " << faceName << ": " << facedef.texture;
                if (facedef.tintIndex >= 0) {
                    std::cout << " (tint: " << facedef.tintIndex << ")";
                }
                std::cout << std::endl;
            }
        }
    }

    BlockId BlockRegistry::registerBlock(const BlockDefinition& definition) {
        auto id = m_nextId++;

        m_definitions.push_back(definition);
        m_nameToId[definition.metadata.name] = id;

        return id;
    }

    bool BlockRegistry::exists(const std::string& name) const {
        return m_nameToId.contains(name);
    }

    BlockId BlockRegistry::getBlockId(const std::string& name) const {
        auto pos = m_nameToId.find(name);
        return (pos != m_nameToId.end()) ? pos->second : INVALID_BLOCK_ID;
    }

    const BlockModel* BlockRegistry::getModel(BlockId id) const {
        if (id >= m_definitions.size()) return nullptr;
        return &m_definitions[id].model;
    }

    const BlockMetadata* BlockRegistry::getMetadata(BlockId id) const {
        if (id >= m_definitions.size()) return nullptr;
        return &m_definitions[id].metadata;
    }

    const BlockMetadata* BlockRegistry::getMetadata(const std::string& name) const {
        BlockId id = getBlockId(name);
        if (id == INVALID_BLOCK_ID) return nullptr;
        return getMetadata(id);
    }

    const BlockDefinition* BlockRegistry::getDefinition(BlockId id) const {
        if (id >= m_definitions.size()) return nullptr;
        return &m_definitions[id];
    }

    int BlockRegistry::getTextureIndex(BlockId id, const BlockElement& element, CubeFace face) const {
        const auto* model = getModel(id);
        if (!model || model->elements.empty()) {
            return -1;
        }

        std::string faceName = getCubeFaceName(face);

        if (auto it = element.faces.find(faceName); it != element.faces.end()) {
            return m_textureArray.getTextureIndex(it->second.texture);
        }

        return -1;
    }

    std::array<int, 6> BlockRegistry::getTextureIndices(BlockId id) const {
        std::array<int, 6> indices;
        const auto& element = getModel(id)->elements[0];
        for (int i = 0; i < 6; ++i) {
            indices[i] = getTextureIndex(id, element, static_cast<CubeFace>(i));
        }
        return indices;
    }

    bool BlockRegistry::hasValidTextures(BlockId id) const {
        const auto* model = getModel(id);
        if (!model || model->elements.empty()) return false;

        for (const auto& [faceName, facedef] : model->elements[0].faces) {
            if (m_textureArray.getTextureIndex(facedef.texture) < 0) {
                return false;
            }
        }
        return true;
    }

    int BlockRegistry::getTintIndex(BlockId id, CubeFace face) const {
        const auto* model = getModel(id);
        if (!model || model->elements.empty()) {
            return -1;
        }

        const auto& element = model->elements[0];
        std::string faceName = getCubeFaceName(face);

        if (auto it = element.faces.find(faceName); it != element.faces.end()) {
            return it->second.tintIndex;
        }

        return -1;
    }

    std::vector<BlockId> BlockRegistry::getBlocksByTag(const std::string& tag) const {
        std::vector<BlockId> result;
        for (BlockId id = 0; id < m_definitions.size(); ++id) {
            if (hasTag(id, tag)) {
                result.push_back(id);
            }
        }
        return result;
    }

    bool BlockRegistry::hasTag(BlockId id, const std::string& tag) const {
        const auto* metadata = getMetadata(id);
        if (!metadata) return false;

        return std::find(metadata->tags.begin(), metadata->tags.end(), tag) != metadata->tags.end();
    }

    size_t BlockRegistry::getBlockCount() const {
        return m_nextId;
    }

}
