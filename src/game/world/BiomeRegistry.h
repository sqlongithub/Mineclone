#pragma once

#include <filesystem>
#include "Biome.h"
#include "Block.h"

namespace Mineclone {
    class BiomeRegistry {
    public:
        BiomeRegistry();

        ~BiomeRegistry() = default;

        bool loadFromDirectory(const std::string& modelsPath);
        BiomeId loadFromFile(const std::filesystem::path& filePath);
        BiomeId loadFromFile(const std::string& filePath);

        BiomeId registerBiome(const BiomeMetadata& metadata);

        bool exists(const std::string& name) const;

        BiomeId getBiomeId(const std::string& name) const;
        std::optional<std::reference_wrapper<const std::string>> getBiomeName(BiomeId id) const;

        const BiomeMetadata* getMetadata(BiomeId id) const;
        const BiomeMetadata* getMetadata(const std::string& name) const;

        std::vector<BiomeId> getBiomesByTags(const std::string& tag) const;
        bool hasTag(BiomeId id, const std::string& tag) const;

        size_t getBiomeCount() const;

        const std::vector<glm::vec3>& getTints() { return m_tints; };
        const std::vector<glm::vec3>& getTints() const { return m_tints; };

    private:
        std::vector<BiomeMetadata> m_metadata;
        std::unordered_map<std::string, BiomeId> m_nameToId;
        std::unordered_map<std::string, std::vector<BiomeId>> m_tagToBiomes;
        std::vector<glm::vec3> m_tints;

        int m_nextId = 0;
    };

}
