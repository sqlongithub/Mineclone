#include "RenderQueue.h"

namespace Mineclone {
    RenderQueue::RenderQueue() {
        m_commands[static_cast<size_t>(Layer::TERRAIN)].reserve(256);
    }

    void RenderQueue::add(const RenderCommand& cmd) {
        m_commands[static_cast<size_t>(cmd.layer)].push_back(cmd);
    }

    void RenderQueue::clear() {
        for (auto& vec : m_commands)
            vec.clear();
    }

    std::vector<RenderCommand>& RenderQueue::getCommands(Layer layer) {
        return m_commands[static_cast<size_t>(layer)];
    }

    const std::vector<RenderCommand>& RenderQueue::getCommands(Layer layer) const {
        return m_commands[static_cast<size_t>(layer)];
    }

    size_t RenderQueue::size() const {
        return m_commands.size();
    }

    bool RenderQueue::empty() const {
        for (const auto& layerVec : m_commands)
            if (!layerVec.empty()) return false;
        return true;
    }
}