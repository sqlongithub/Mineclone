#pragma once
#include "RenderCommand.h"

namespace Mineclone {
    class RenderQueue {
    public:
        RenderQueue();

        void add(const RenderCommand& cmd);
        void clear();

        std::vector<RenderCommand>& getBucket(Layer layer);
        [[nodiscard]] const std::vector<RenderCommand>& getBucket(Layer layer) const;
    private:
        std::vector<RenderCommand> m_buckets[static_cast<size_t>(Layer::LAYER_COUNT)];
    };
}
