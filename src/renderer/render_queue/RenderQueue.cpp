#include "RenderQueue.h"

namespace Mineclone {
    RenderQueue::RenderQueue() {
        for (size_t i = 0; i < static_cast<size_t>(Layer::LAYER_COUNT); i++) {
            auto& bucket = m_buckets[i];
            switch(static_cast<Layer>(i)) {
                case Layer::TERRAIN:
                    bucket.reserve(512);
                    break;
                case Layer::TRANSPARENT:
                    bucket.reserve(256);
                    break;
                default:
                    bucket.reserve(8);
                    break;
            }
        }

    }

    void RenderQueue::add(const RenderCommand &cmd) {
        m_buckets[static_cast<size_t>(cmd.layer)].push_back(cmd);
    }

    void RenderQueue::clear() {
        for(auto& bucket : m_buckets) {
            bucket.clear();
        }
    }

    std::vector<RenderCommand> &RenderQueue::getBucket(Layer layer) {
        return m_buckets[static_cast<size_t>(layer)];
    }

    const std::vector<RenderCommand> &RenderQueue::getBucket(Layer layer) const {
        return m_buckets[static_cast<size_t>(layer)];
    }
}