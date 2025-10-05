#include "TerrainGenerator.h"

#include <iostream>

namespace Mineclone {
    TerrainGenerator::TerrainGenerator(int seed)
        : m_seed(seed) {

        m_heightNoise.SetSeed(m_seed);
        m_heightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_heightNoise.SetFrequency(0.003f);
        m_heightNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_heightNoise.SetFractalOctaves(5);
        m_heightNoise.SetFractalLacunarity(2.2f);
        m_heightNoise.SetFractalGain(0.45f);

        m_temperatureNoise.SetSeed(m_seed + 1000);
        m_temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_temperatureNoise.SetFrequency(0.002f);
        m_temperatureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_temperatureNoise.SetFractalOctaves(3);
        m_temperatureNoise.SetFractalLacunarity(2.0f);
        m_temperatureNoise.SetFractalGain(0.5f);

        m_humidityNoise.SetSeed(m_seed + 2000);
        m_humidityNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_humidityNoise.SetFrequency(0.0025f);
        m_humidityNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_humidityNoise.SetFractalOctaves(3);
        m_humidityNoise.SetFractalLacunarity(2.0f);
        m_humidityNoise.SetFractalGain(0.5f);

        m_continentalnessNoise.SetSeed(m_seed + 3000);
        m_continentalnessNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_continentalnessNoise.SetFrequency(0.0005f);
        m_continentalnessNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_continentalnessNoise.SetFractalOctaves(4);
        m_continentalnessNoise.SetFractalLacunarity(2.0f);
        m_continentalnessNoise.SetFractalGain(0.5f);

        m_erosionNoise.SetSeed(m_seed + 4000);
        m_erosionNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_erosionNoise.SetFrequency(0.006f);
        m_erosionNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_erosionNoise.SetFractalOctaves(4);
        m_erosionNoise.SetFractalLacunarity(2.0f);
        m_erosionNoise.SetFractalGain(0.45f);

        m_weirdnessNoise.SetSeed(m_seed + 5000);
        m_weirdnessNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_weirdnessNoise.SetFrequency(0.008f);
        m_weirdnessNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_weirdnessNoise.SetFractalOctaves(3);
        m_weirdnessNoise.SetFractalLacunarity(2.0f);
        m_weirdnessNoise.SetFractalGain(0.5f);
    }


    void TerrainGenerator::generate(ChunkPos chunkPos, Chunk& chunk, const BlockRegistry& blockRegistry, const BiomeRegistry& biomeRegistry) {
        std::vector<float> heightBuffer(CHUNK_WIDTH * CHUNK_DEPTH);
#ifdef DEBUG
        std::cout << "Generating height for chunk " << chunkPos.x << ", " << chunkPos.z << std::endl;
#endif

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                float fx = chunkPos.x * CHUNK_WIDTH + x;
                float fz = chunkPos.z * CHUNK_DEPTH + z;
                heightBuffer[x + z * CHUNK_WIDTH] = m_heightNoise.GetNoise(fx, fz);
            }
        }

        std::vector<BiomeTerrainParameters> biomeParams(CHUNK_WIDTH * CHUNK_DEPTH);
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                int worldX = chunkPos.x * CHUNK_WIDTH + x;
                int worldZ = chunkPos.z * CHUNK_DEPTH + z;
                biomeParams[x + z * CHUNK_WIDTH] = sampleBiomeParameters(worldX, worldZ);
            }
        }

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                int idx = x + z * CHUNK_WIDTH;
                float heightNoise = heightBuffer[idx];
                BiomeTerrainParameters& params = biomeParams[idx];

                BiomeId biome = findBestBiome(params, biomeRegistry);
                chunk.setBiome(x, z, biome);
                int height = getTerrainHeight(heightNoise, biome, biomeRegistry);

                generateTerrainColumn(x, z, height, biome,
                                      chunk, blockRegistry, biomeRegistry);
            }
        }
    }

    BiomeId TerrainGenerator::getBiomeAt(WorldPos pos, BiomeRegistry& biomeRegistry) const {
        BiomeTerrainParameters params = sampleBiomeParameters(pos.x, pos.z);
        return findBestBiome(params, biomeRegistry);
    }

    BiomeTerrainParameters TerrainGenerator::sampleBiomeParameters(int worldX, int worldZ) const {
        BiomeTerrainParameters params{};

        params.temperature = m_temperatureNoise.GetNoise(worldX * 0.002f, worldZ * 0.002f);
        params.humidity = m_humidityNoise.GetNoise(worldX * 0.0025f, worldZ * 0.0025f);
        params.continentalness = m_continentalnessNoise.GetNoise(worldX * 0.0005f, worldZ * 0.0005f);
        params.erosion = m_erosionNoise.GetNoise(worldX * 0.006f, worldZ * 0.006f);
        params.weirdness = m_weirdnessNoise.GetNoise(worldX * 0.008f, worldZ * 0.008f);

        params.temperature = std::clamp(params.temperature, -1.0f, 1.0f);
        params.humidity = std::clamp(params.humidity, -1.0f, 1.0f);
        params.continentalness = std::clamp(params.continentalness, -1.0f, 1.0f);
        params.erosion = std::clamp(params.erosion, -1.0f, 1.0f);
        params.weirdness = std::clamp(params.weirdness, -1.0f, 1.0f);

        return params;
    }

    BiomeId TerrainGenerator::findBestBiome(const BiomeTerrainParameters &params, const BiomeRegistry &biomeRegistry) const {
        BiomeId bestBiome = 0;
        float bestDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < biomeRegistry.getBiomeCount(); i++) {
            const BiomeMetadata* meta = biomeRegistry.getMetadata(static_cast<BiomeId>(i));
            if (!meta) continue;

            float dist = calculateBiomeDistance(params, *meta);
            if (dist < bestDistance) {
                bestDistance = dist;
                bestBiome = static_cast<BiomeId>(i);
            }
        }
        return bestBiome;
    }

    float TerrainGenerator::calculateBiomeDistance(const BiomeTerrainParameters &params, const BiomeMetadata &biome) const {
        const auto& bp = biome.terrainParameters;

        float tempDiff = params.temperature - bp.temperature;
        float humidDiff = params.humidity - bp.humidity;
        float continentDiff = params.continentalness - bp.continentalness;
        float erosionDiff = params.erosion - bp.erosion;
        float weirdDiff = params.weirdness - bp.weirdness;

        return std::sqrt(
            tempDiff * tempDiff * 2.0f +
            humidDiff * humidDiff * 2.0f +
            continentDiff * continentDiff * 1.0f +
            erosionDiff * erosionDiff * 1.5f +
            weirdDiff * weirdDiff * 0.5f
        );
    }

    int TerrainGenerator::getTerrainHeight(float heightNoise, BiomeId biome, const BiomeRegistry &biomeRegistry) {
        const BiomeMetadata* metadata = biomeRegistry.getMetadata(biome);
        if (!metadata) return 64;

        const auto& bp = metadata->terrainParameters;

        int baseHeight = 64;
        int amplitude = 12;

        float continentalFactor = (bp.continentalness + 1.0f) * 0.5f;
        baseHeight += static_cast<int>((continentalFactor - 0.5f) * 35);

        float erosionFactor = (bp.erosion + 1.0f) * 0.5f;
        amplitude += static_cast<int>((1.0f - erosionFactor) * 25);

        if (bp.temperature < -0.3f) {
            baseHeight += static_cast<int>((bp.temperature + 0.3f) * -12);
            amplitude += static_cast<int>((bp.temperature + 0.3f) * -8);
        }

        float weirdnessFactor = bp.weirdness * bp.weirdness * (bp.weirdness > 0 ? 1 : -1);
        amplitude += static_cast<int>(weirdnessFactor * 12);

        float normalizedHeight = (heightNoise + 1.0f) * 0.5f;
        int height = baseHeight + static_cast<int>(normalizedHeight * amplitude);

        if (bp.continentalness < -0.3f) {
            int waterLevel = 62;
            height = std::min(height, waterLevel - static_cast<int>((bp.continentalness + 0.3f) * -15));
        }

        return std::clamp(height, 8, CHUNK_HEIGHT - 8);
    }


    void TerrainGenerator::generateTerrainColumn(int x, int z, int height, BiomeId biome, Chunk &chunk, const BlockRegistry &blockRegistry,
                          const BiomeRegistry &biomeRegistry) {
        const BiomeMetadata* metadata = biomeRegistry.getMetadata(biome);

        static BlockId stoneId = blockRegistry.getBlockId("stone");
        static BlockId dirtId  = blockRegistry.getBlockId("dirt");
        static BlockId grassId = blockRegistry.getBlockId("grass");
        static BlockId sandId  = blockRegistry.getBlockId("sand");
        static BlockId snowId  = blockRegistry.getBlockId("snow");

        int surfaceLayers = 3;
        if (metadata) {
            surfaceLayers = 2 + static_cast<int>((0.5f - metadata->terrainParameters.erosion) * 4);
            surfaceLayers = std::clamp(surfaceLayers, 2, 5);
        }

        for (int y = 0; y <= height; y++) {
            BlockId blockId = stoneId;

            if (y == height) {
                bool isUnderwater = height < 62;
                bool isDesert = metadata && metadata->terrainParameters.humidity < -0.2f &&
                               metadata->terrainParameters.temperature > 0.3f;
                bool isCold = metadata && metadata->terrainParameters.temperature < -0.4f;
                bool isSnowy = metadata && metadata->terrainParameters.temperature < -0.7f;
                bool isCoastal = metadata && metadata->terrainParameters.continentalness < -0.1f &&
                                metadata->terrainParameters.continentalness > -0.5f;

                if (isUnderwater || isCoastal || isDesert) blockId = sandId;
                else if (isSnowy) blockId = snowId;
                else if (isCold) blockId = dirtId;
                else blockId = grassId;
            } else if (y >= height - surfaceLayers) {
                bool isDesert = metadata && metadata->terrainParameters.humidity < -0.2f &&
                               metadata->terrainParameters.temperature > 0.2f;
                bool isCoastal = metadata && metadata->terrainParameters.continentalness < -0.1f;
                if (isDesert || isCoastal) blockId = sandId;
                else blockId = dirtId;
            } else {
                blockId = stoneId;
            }

            chunk.setBlock(x, y, z, blockId);
        }
    }
}