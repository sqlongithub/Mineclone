#pragma once

#include "Chunk.h"
#include "BlockRegistry.h"
#include "ChunkConstants.h"
#include "Biome.h"
#include "BiomeRegistry.h"
#include "WorldPos.h"
#include "FastNoiseLite.h"

namespace Mineclone {

    class TerrainGenerator {
    public:
        TerrainGenerator(int seed = 1337);

        void generate(ChunkPos chunkPos, Chunk &chunk, const BlockRegistry &blockRegistry,
                      const BiomeRegistry &biomeRegistry);

        BiomeId getBiomeAt(WorldPos pos, BiomeRegistry& biomeRegistry) const;

    private:
        int m_seed;

        FastNoiseLite m_heightNoise;
        FastNoiseLite m_temperatureNoise;
        FastNoiseLite m_humidityNoise;
        FastNoiseLite m_continentalnessNoise;
        FastNoiseLite m_erosionNoise;
        FastNoiseLite m_weirdnessNoise;

        BiomeTerrainParameters sampleBiomeParameters(int worldX, int worldZ) const;

        BiomeId findBestBiome(const BiomeTerrainParameters &params,
                              const BiomeRegistry &biomeRegistry) const;

        float calculateBiomeDistance(const BiomeTerrainParameters &params,
                                     const BiomeMetadata &biome) const;

        int getTerrainHeight(float heightNoise,
                             BiomeId biome,
                             const BiomeRegistry &biomeRegistry);

        void generateTerrainColumn(int x, int z, int height, BiomeId biome,
                                   Chunk &chunk,
                                   const BlockRegistry &blockRegistry,
                                   const BiomeRegistry &biomeRegistry);
    };
} // namespace Mineclone