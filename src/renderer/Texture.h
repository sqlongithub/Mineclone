#pragma once
#include <string>
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION

namespace Mineclone {
    class Texture {
    public:
        Texture() = default;
        explicit Texture(const std::string& path);
        ~Texture();

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        [[nodiscard]] GLuint getID() const { return m_id; }

    private:
        GLuint m_id = 0;
        void loadFromFile(const std::string& path);
    };

}
