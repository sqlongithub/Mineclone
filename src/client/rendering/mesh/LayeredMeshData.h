#pragma once

#include <vector>
#include <array>

#include "client/rendering/render_queue/RenderLayer.h"

namespace Mineclone {

    template<typename VertexT>
    struct LayerData {
        std::vector<VertexT> vertices;
        std::vector<uint32_t> indices;

        void clear() {
            vertices.clear();
            indices.clear();
        }

        void shrink_to_fit() {
            vertices.shrink_to_fit();
            indices.shrink_to_fit();
        }

        bool empty() const {
            return vertices.empty() && indices.empty();
        }

        size_t getMemoryUsage() const {
            return vertices.capacity() * sizeof(VertexT) +
                   indices.capacity() * sizeof(uint32_t);
        }
    };

    template<typename VertexT>
    class LayeredMeshData {
    public:
        LayeredMeshData() = default;
        ~LayeredMeshData() noexcept = default;


        LayeredMeshData(LayeredMeshData&&) noexcept = default;
        LayeredMeshData& operator=(LayeredMeshData&&) noexcept = default;

        LayeredMeshData(const LayeredMeshData&) = delete;
        LayeredMeshData& operator=(const LayeredMeshData&) = delete;


        void clear(bool releaseMemory = false) noexcept {
            for (auto& layer : m_layers) {
                layer.clear();
                if (releaseMemory) {
                    layer.shrink_to_fit();
                }
            }
        }


        bool empty() const noexcept {
            for (const auto& layer : m_layers) {
                if (!layer.empty()) return false;
            }
            return true;
        }


        bool empty(Layer layer) const noexcept {
            return m_layers[layerToIndex(layer)].empty();
        }


        const std::vector<VertexT>& getVertices(const Layer layer) const {
            return m_layers[layerToIndex(layer)].vertices;
        }

        std::vector<VertexT>& getVertices(const Layer layer) {
            return m_layers[layerToIndex(layer)].vertices;
        }

        const std::vector<uint32_t>& getIndices(const Layer layer) const {
            return m_layers[layerToIndex(layer)].indices;
        }

        std::vector<uint32_t>& getIndices(const Layer layer) {
            return m_layers[layerToIndex(layer)].indices;
        }


        const LayerData<VertexT>& getLayer(Layer layer) const {
            return m_layers[layerToIndex(layer)];
        }

        LayerData<VertexT>& getLayer(Layer layer) {
            return m_layers[layerToIndex(layer)];
        }


        bool isValid(Layer* outLayer = nullptr, uint32_t* outIndex = nullptr) const noexcept {
            for (size_t i = 0; i < m_layers.size(); ++i) {
                const auto& layer = m_layers[i];
                for (uint32_t idx : layer.indices) {
                    if (idx >= layer.vertices.size()) {
                        if (outLayer) *outLayer = static_cast<Layer>(i);
                        if (outIndex) *outIndex = idx;
                        return false;
                    }
                }
            }
            return true;
        }


        void reserve(Layer layer, size_t vertexCount, size_t indexCount) {
            auto& layerData = m_layers[layerToIndex(layer)];
            layerData.vertices.reserve(vertexCount);
            layerData.indices.reserve(indexCount);
        }


        size_t getMemoryUsage() const noexcept {
            size_t total = 0;
            for (const auto& layer : m_layers) {
                total += layer.getMemoryUsage();
            }
            return total;
        }

        // Get total counts across all layers
        size_t getTotalVertexCount() const noexcept {
            size_t total = 0;
            for (const auto& layer : m_layers) {
                total += layer.vertices.size();
            }
            return total;
        }

        size_t getTotalIndexCount() const noexcept {
            size_t total = 0;
            for (const auto& layer : m_layers) {
                total += layer.indices.size();
            }
            return total;
        }

        // Iterate over all layers
        template<typename Func>
        void forEachLayer(Func&& func) {
            for (size_t i = 0; i < m_layers.size(); ++i) {
                func(static_cast<Layer>(i), m_layers[i]);
            }
        }

        template<typename Func>
        void forEachLayer(Func&& func) const {
            for (size_t i = 0; i < m_layers.size(); ++i) {
                func(static_cast<Layer>(i), m_layers[i]);
            }
        }

        // Iterate over non-empty layers only
        template<typename Func>
        void forEachNonEmptyLayer(Func&& func) {
            for (size_t i = 0; i < m_layers.size(); ++i) {
                if (!m_layers[i].empty()) {
                    func(static_cast<Layer>(i), m_layers[i]);
                }
            }
        }

        template<typename Func>
        void forEachNonEmptyLayer(Func&& func) const {
            for (size_t i = 0; i < m_layers.size(); ++i) {
                if (!m_layers[i].empty()) {
                    func(static_cast<Layer>(i), m_layers[i]);
                }
            }
        }

    private:
        std::array<LayerData<VertexT>, static_cast<size_t>(Layer::LAYER_COUNT)> m_layers;

        static constexpr size_t layerToIndex(Layer layer) {
            return static_cast<size_t>(layer);
        }
    };

} // namespace Mineclone