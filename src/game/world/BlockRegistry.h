#pragma once

#include "Block.h"
#include "CubeFace.h"
#include "BlockModel.h"
#include "TextureArray.h"
#include <array>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace Mineclone {
    class BlockRegistry {
    public:
        BlockRegistry(TextureArray& textureArray);

        ~BlockRegistry() = default;

        bool loadFromDirectory(const std::string& modelsPath);
        BlockId loadFromFile(const std::filesystem::path& filePath);
        BlockId loadFromFile(const std::string& filePath);

        BlockId registerBlock(const BlockMetadata& metadata, const BlockModel& model);

        bool exists(const std::string& name) const;

        BlockId getBlockId(const std::string& name) const;

        const BlockModel* getModel(BlockId id) const;
        const BlockMetadata* getMetadata(BlockId id) const;
        const BlockMetadata* getMetadata(const std::string& name) const;

        int getTextureIndex(BlockId id, CubeFace face) const;
        const std::array<int, 6>& getTextureIndices(BlockId id) const;
        bool hasValidTextures(BlockId id) const;

        std::vector<BlockId> getBlocksByTag(const std::string& tag) const;
        bool hasTag(BlockId id, const std::string& tag) const;

        size_t getBlockCount() const;

    private:
        TextureArray& m_textureArray;
        std::vector<BlockModel> m_models;
        std::vector<BlockMetadata> m_metadata;
        std::unordered_map<std::string, BlockId> m_nameToId;
        std::unordered_map<std::string, std::vector<BlockId>> m_tagToBlocks;

        int m_nextId = 0;
    };

} // Mineclone
