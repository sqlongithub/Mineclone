#pragma once
#include "RenderCommand.h"
#include <array>

namespace Mineclone {
    class RenderQueue {
    public:
        RenderQueue();

        void add(const RenderCommand& cmd);
        void clear();

        std::vector<RenderCommand>& getCommands(Layer layer);
        [[nodiscard]] const std::vector<RenderCommand>& getCommands(Layer layer) const;

        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

    private:
        std::array<std::vector<RenderCommand>, LAYER_COUNT> m_commands{};
    };
}