
#include "TerrainGenerator.h"

namespace Mineclone {
    TerrainGenerator::TerrainGenerator(int seed)
        : m_seed(seed) {
        m_simplexNoise = FastNoise::New<FastNoise::Simplex>();
        m_fractal  = FastNoise::New<FastNoise::FractalFBm>();


        m_fractal->SetSource(m_simplexNoise);
        m_fractal->SetOctaveCount(5);
        m_fractal->SetLacunarity(2.0f);     // frequency multiplier per octave
        m_fractal->SetGain(0.5f);
    }

    void TerrainGenerator::generate(ChunkPos chunkPos, Chunk &chunk, const BlockRegistry &blockRegistry) {
        std::vector<float> noiseBuffer(CHUNK_WIDTH * CHUNK_DEPTH);

        // Generate a 2D noise map for this chunk
        m_fractal->GenUniformGrid2D(
                noiseBuffer.data(),
                chunkPos.x * CHUNK_WIDTH,   // startX
                chunkPos.z * CHUNK_DEPTH,   // startZ
                CHUNK_WIDTH,
                CHUNK_DEPTH,
                0.01f,                      // frequency (lower = larger hills)
                m_seed
        );

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                float n = noiseBuffer[x + z * CHUNK_WIDTH]; // range ~ [-1, 1]

                int baseHeight = 64;
                int amplitude = 20;
                int height = baseHeight + static_cast<int>(n * amplitude);
                height = std::clamp(height, 1, CHUNK_HEIGHT - 1);

                for (int y = 0; y <= height; y++) {
                    BlockId id;
                    if (y == height) {
                        id = blockRegistry.getBlockId("grass");
                    } else if (y >= height - 3) {
                        id = blockRegistry.getBlockId("dirt");
                    } else {
                        id = blockRegistry.getBlockId("stone");
                    }
                    chunk.setBlock(x, y, z, id);
                }
            }
        }
    }
} // Mineclone