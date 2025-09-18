#include <stdexcept>
#include "BlockRegistry.h"
#include "TextureArray.h"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <iostream>

namespace Mineclone {

    BlockRegistry::BlockRegistry(TextureArray& textureArray)
        : m_textureArray(textureArray) {
        registerBlock({
            "air"
        }, {});
    }

    bool BlockRegistry::loadFromDirectory(const std::string& modelsPath) {
        namespace fs = std::filesystem;
        if (!fs::exists(modelsPath) || !fs::is_directory(modelsPath)) {
            throw std::runtime_error("Invalid directory: " + modelsPath);
            return false;
        }


        for(const auto& entry : fs::directory_iterator(modelsPath)) {
            loadFromFile(entry);
        }
        return true;
    }

    BlockId BlockRegistry::loadFromFile(const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        // Move file contents into a string
        std::ostringstream oss;
        oss << file.rdbuf();

        std::string jsonText = oss.str();

        BlockJson blockData;
        auto ec = glz::read_json(blockData, jsonText);
        if (ec) {
            throw std::runtime_error("Glaze parse error in " + filePath.string() + ": " + glz::format_error(ec, jsonText));
        }

        BlockModel model{};

        // expand 'all'
        if (auto it = blockData.textures.find("all"); it != blockData.textures.end()) {
            const std::string& tex = it->second;
            static constexpr std::array<const char*, 6> faces = {
                    "top", "bottom", "north", "south", "west", "east"
            };
            for (auto face : faces) {
                blockData.textures.try_emplace(face, tex);
            }
            blockData.textures.erase(it);
        }

        // expand 'side'
        if (auto it = blockData.textures.find("side"); it != blockData.textures.end()) {
            const std::string& tex = it->second;
            static constexpr std::array<const char*, 6> faces = {
                    "north", "south", "west", "east", "up", "down"
            };
            for (auto face : faces) {
                blockData.textures.try_emplace(face, tex);
            }
            blockData.textures.erase(it);
        }

        for(const auto& tex : blockData.textures) {
            int index = getCubeFaceIndex(tex.first);
            if(index == static_cast<int>(CubeFace::UNKNOWN)) {
                std::cout << "Unknown index for face " << tex.first << " in model for " << blockData.name;
            }
            model.faceTextureIndices[getCubeFaceIndex(tex.first)] = m_textureArray.getTextureIndex(tex.second);
        }

        std::cout << "\nLoaded block model\n";
        std::cout << "Face North: " << m_textureArray.getTextureName(model.faceTextureIndices[0]);
        std::cout << "\nFace South: " << m_textureArray.getTextureName(model.faceTextureIndices[1]);
        std::cout << "\nFace East: " << m_textureArray.getTextureName(model.faceTextureIndices[2]);
        std::cout << "\nFace West: " << m_textureArray.getTextureName(model.faceTextureIndices[3]);
        std::cout << "\nFace Up: " << m_textureArray.getTextureName(model.faceTextureIndices[4]);
        std::cout << "\nFace Down: " << m_textureArray.getTextureName(model.faceTextureIndices[5]);

        return registerBlock({
            blockData.name
        }, model);
    }

    BlockId BlockRegistry::loadFromFile(const std::string& filePath) {
        return loadFromFile(std::filesystem::path(filePath));
    }

    BlockId BlockRegistry::registerBlock(const BlockMetadata& metadata, const BlockModel& model) {
        auto id = m_nextId++;

        m_models.push_back(model);
        m_nameToId[metadata.name] = id;
        m_metadata.push_back(metadata);

        return id;
    }

    bool BlockRegistry::exists(const std::string& name) const {
        return m_nameToId.contains(name);
    }

    BlockId BlockRegistry::getBlockId(const std::string& name) const {
        auto pos = m_nameToId.find(name);
        if(pos == m_nameToId.end()) {
            return INVALID_BLOCK_ID;
        } else {
            return pos->second;
        }
    }

    const BlockModel* BlockRegistry::getModel(BlockId id) const {
        return &m_models[id];
    }

    const BlockMetadata* BlockRegistry::getMetadata(BlockId id) const {
        return &m_metadata[id];
    }

    const BlockMetadata* BlockRegistry::getMetadata(const std::string& name) const {
        return &m_metadata[getBlockId(name)];
    }

    int BlockRegistry::getTextureIndex(BlockId id, CubeFace face) const {
        return m_models[id].faceTextureIndices[static_cast<int>(face)];
    }

    const std::array<int, 6> &BlockRegistry::getTextureIndices(BlockId id) const {
        return m_models[id].faceTextureIndices;
    }

    bool BlockRegistry::hasValidTextures(BlockId id) const {
        return std::ranges::all_of(m_models[id].faceTextureIndices, [this](auto index) {
            return index != BlockModel::INVALID_TEXTURE_INDEX;
        });
    }

    std::vector<BlockId> BlockRegistry::getBlocksByTag(const std::string& tag) const {
        return std::vector<BlockId>();
    }

    bool BlockRegistry::hasTag(BlockId id, const std::string& tag) const {
        return false;
    }

    size_t BlockRegistry::getBlockCount() const {
        return m_nextId;
    }
}