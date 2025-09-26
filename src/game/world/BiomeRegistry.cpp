
#include "core/glm_glaze_support.h"
#include <glaze/json/read.hpp>
#include <fstream>
#include "BiomeRegistry.h"

namespace Mineclone {
    BiomeRegistry::BiomeRegistry() {

    }

    bool BiomeRegistry::loadFromDirectory(const std::string &modelsPath) {
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

    BiomeId BiomeRegistry::loadFromFile(const std::filesystem::path &filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        std::ostringstream oss;
        oss << file.rdbuf();

        std::string jsonText = oss.str();

        auto biomeMetadata = glz::read_json<BiomeMetadata>(jsonText);
        int id = INVALID_BIOME_ID;
        if(biomeMetadata.has_value()) {
            id = registerBiome(biomeMetadata.value());
        }
        return id;
    }

    BiomeId BiomeRegistry::loadFromFile(const std::string &filePath) {
        return loadFromFile(std::filesystem::path(filePath));
    }

    BiomeId BiomeRegistry::registerBiome(const BiomeMetadata &metadata) {
        auto id = m_nextId++;

        m_nameToId[metadata.name] = id;
        m_metadata.push_back(metadata);
        m_tints.push_back(metadata.grassTint);

        return id;
    }

    bool BiomeRegistry::exists(const std::string &name) const {
        return m_nameToId.contains(name);
    }

    BiomeId BiomeRegistry::getBiomeId(const std::string &name) const {
        return 0;
    }

    std::optional<std::reference_wrapper<const std::string>> BiomeRegistry::getBiomeName(BiomeId id) const {
        for (const auto& [key, val] : m_nameToId) {
            if (val == id) {
                return std::cref(key);
            }
        }
        return std::nullopt; // not found
    }

    const BiomeMetadata* BiomeRegistry::getMetadata(BiomeId id) const {
        return &m_metadata[id];
    }

    const BiomeMetadata* BiomeRegistry::getMetadata(const std::string& name) const {
        return &m_metadata[m_nameToId.find(name)->second];
    }

    std::vector<BiomeId> BiomeRegistry::getBiomesByTags(const std::string &tag) const {
        return std::vector<BiomeId>();
    }

    bool BiomeRegistry::hasTag(BiomeId id, const std::string &tag) const {
        return false;
    }

    size_t BiomeRegistry::getBiomeCount() const {
        return m_nextId;
    }


}