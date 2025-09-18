// ChunkTerrain.h
#pragma once

#include "Chunk.h"
#include "BlockRegistry.h"
#include "ChunkConstants.h"

#include "FastNoise/FastNoise.h"
#include "FastNoise2/include/FastNoise/SmartNode.h"
#include "FastNoise2/include/FastNoise/Generators/Simplex.h"
#include "FastNoise2/include/FastNoise/Generators/Fractal.h"

namespace Mineclone {

    class TerrainGenerator {
    public:
        TerrainGenerator(int seed = 1337);

        void generate(ChunkPos chunkPos, Chunk & chunk, const BlockRegistry & blockRegistry);

    private:
        FastNoise::SmartNode<FastNoise::Simplex> m_simplexNoise;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_fractal;
        int m_seed;
    };

} // namespace Mineclone
