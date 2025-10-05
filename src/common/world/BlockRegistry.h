#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "Block.h"
#include "CubeFace.h"
#include "BlockDefinition.h"

namespace Mineclone {
    class TextureArray;

    class BlockRegistry {
    public:
        explicit BlockRegistry(TextureArray& textureArray);

        bool loadFromDirectory(const std::string& modelsPath);
        BlockId loadFromFile(const std::filesystem::path& filePath);
        BlockId loadFromFile(const std::string& filePath);

        BlockId registerBlock(const BlockDefinition& definition);

        bool exists(const std::string& name) const;
        BlockId getBlockId(const std::string& name) const;

        const BlockModel* getModel(BlockId id) const;
        const BlockMetadata* getMetadata(BlockId id) const;
        const BlockMetadata* getMetadata(const std::string& name) const;
        const BlockDefinition* getDefinition(BlockId id) const;

        int getTextureIndex(BlockId id, const BlockElement& element, CubeFace face) const;
        std::array<int, 6> getTextureIndices(BlockId id) const;
        bool hasValidTextures(BlockId id) const;

        int getTintIndex(BlockId id, CubeFace face) const;

        std::vector<BlockId> getBlocksByTag(const std::string& tag) const;
        bool hasTag(BlockId id, const std::string& tag) const;

        size_t getBlockCount() const;

    private:
        TextureArray& m_textureArray;
        std::vector<BlockDefinition> m_definitions;
        std::unordered_map<std::string, BlockId> m_nameToId;
        BlockId m_nextId = 0;

        void processBlockDefinition(BlockDefinition& definition);
        void printDefinitionInfo(const BlockDefinition& definition);
        void expandTextureAliases(BlockModel& model);
        void resolveTextureReferences(BlockModel& model, const std::string& name);
    };
}