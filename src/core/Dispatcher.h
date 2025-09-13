#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <iostream>

namespace Mineclone {

    struct IEvent {
        virtual ~IEvent() = default;
    };

    class Dispatcher {
    public:
        template<typename Event>
        void subscribe(std::function<void(const Event&)> fn) {
            std::cout << "typeid: " << typeid(Event).name() << "\n";
            std::cout << "bucket count: " << m_subscribers.bucket_count();
            auto& subs = m_subscribers[typeid(Event)];
            subs.push_back([fn](const void* e) {
                fn(*static_cast<const Event*>(e));
            });
        }

        template<typename Event>
        void trigger(const Event& e) const {
            auto it = m_subscribers.find(typeid(Event));
            if (it != m_subscribers.end()) {
                for (auto& sub : it->second) {
                    sub(&e);
                }
            }
        }
    private:
        using Callback = std::function<void(const void*)>;
        std::unordered_map<std::type_index, std::vector<Callback>> m_subscribers;
    };

}
