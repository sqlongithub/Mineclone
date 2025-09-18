#pragma once

#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Handle.h"

namespace Mineclone {
    template<typename T>
    class ResourceLibrary {
    public:
        using HandleType = Handle<T>;

        ResourceLibrary() {
            // slot 0 is invalid
            m_resources.emplace_back(nullptr);
            m_generations.push_back(0);
        }

        ResourceLibrary(const ResourceLibrary&) = delete;
        ResourceLibrary& operator=(const ResourceLibrary&) = delete;


        template<typename... Args>
        HandleType create(const std::string& name, Args&&... args) {
            uint32_t index;

            if (!m_freeIndices.empty()) {
                index = m_freeIndices.back();
                m_freeIndices.pop_back();

                m_resources[index] = std::make_unique<T>(std::forward<Args>(args)...);
            } else {
                index = static_cast<uint32_t>(m_resources.size());
                m_resources.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                m_generations.push_back(1);
            }

            HandleType handle(index, m_generations[index]);
            m_nameToHandle[name] = handle;
            return handle;
        }

        T* get(HandleType handle) {
            if (!isValid(handle)) return nullptr;
            return m_resources[handle.index].get();
        }

        const T* get(HandleType handle) const {
            if (!isValid(handle)) return nullptr;
            return m_resources[handle.index].get();
        }

        // Get resource by name
        T* get(const std::string& name) {
            auto it = m_nameToHandle.find(name);
            if (it == m_nameToHandle.end()) return nullptr;
            return get(it->second);
        }

        // Get handle by name
        HandleType getHandle(const std::string& name) const {
            auto it = m_nameToHandle.find(name);
            return (it != m_nameToHandle.end()) ? it->second : HandleType::invalid();
        }

        // Validate handle
        bool isValid(HandleType handle) const {
            return handle.index < m_resources.size() &&
                   handle.index > 0 &&
                   m_generations[handle.index] == handle.generation;
        }

        // Unload resource
        void unload(const std::string& name) {
            auto it = m_nameToHandle.find(name);
            if (it != m_nameToHandle.end()) {
                uint32_t index = it->second.index;

                m_resources[index].reset();

                m_generations[index]++;
                m_freeIndices.push_back(index);
                m_nameToHandle.erase(it);
            }
        }

        // Clear all resources
        void clear() {
            m_resources.clear();
            m_generations.clear();
            m_nameToHandle.clear();
            m_freeIndices.clear();

            m_resources.emplace_back(nullptr);
            m_generations.push_back(0);
        }

        // Get total count (including freed slots)
        [[nodiscard]] size_t size() const { return m_resources.size() - 1; } // -1 for invalid slot

    private:
        std::vector<std::unique_ptr<T>> m_resources;
        std::vector<uint32_t> m_generations;
        std::unordered_map<std::string, HandleType> m_nameToHandle;
        std::vector<uint32_t> m_freeIndices;
    };
}