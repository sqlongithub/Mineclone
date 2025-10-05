#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <optional>
#include "TextureArray.h"
#include "stb_image.h"

namespace Mineclone {
    TextureArray::TextureArray(int size, int maxTextures)
            : m_textureSize(size), m_maxLayers(maxTextures), m_currentLayers(0) {

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                     m_textureSize, m_textureSize, m_maxLayers,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    TextureArray::~TextureArray() {
        glDeleteTextures(1, &m_id);
    }

    int TextureArray::addTexture(const std::string& name, const std::string &filepath) {
        auto it = m_textureIndexMap.find(filepath);
        if (it != m_textureIndexMap.end()) {
            return it->second;
        }

        if (m_currentLayers >= m_maxLayers) {
            throw std::runtime_error("texture array is full!");
        }

        unsigned char* data = loadImageData(filepath);
        if (!data) {
            throw std::runtime_error("Failed to load texture: " + filepath);
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                        0, 0, m_currentLayers,  // x, y, z offset
                        m_textureSize, m_textureSize, 1,  // width, height, depth
                        GL_RGBA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        m_textureIndexMap[name] = m_currentLayers;
        freeImageData(data);

        std::cout << "loaded texture: " << name << " at index " << m_currentLayers << std::endl;

        return m_currentLayers++;
    }

    int TextureArray::addTextures(const std::string& directory) {
        namespace fs = std::filesystem;
        int success = m_currentLayers;
        for (const auto& entry: fs::directory_iterator(directory)) {
            if (!entry.is_regular_file())
                continue;

            const auto& path = entry.path();
            auto ext = path.extension().string();

            if(ext != ".png" && ext != ".jpg")
                continue;

            std::string name = path.stem().string();

            int index = addTexture(name, path.string());
            if (index < 0) {
                success = index;
            } else {
                success = index;
            }
        }

        generateMipmaps();
        return success;
    }

    int TextureArray::getTextureIndex(const std::string &name) const {
        auto it = m_textureIndexMap.find(name);
        return (it != m_textureIndexMap.end()) ? it->second : -1;
    }

    void TextureArray::bind(GLuint unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    }

    void TextureArray::unbind() const {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void TextureArray::generateMipmaps() const {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    GLuint TextureArray::getTextureID() const {
        return m_id;
    }

    int TextureArray::getTextureCount() const {
        return m_currentLayers;
    }

    unsigned char *TextureArray::loadImageData(const std::string &filepath) {
        int width, height, channels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

        if (!data) {
            std::cerr << "Failed to load image: " << filepath << std::endl;
            return nullptr;
        }

        if (width != m_textureSize || height != m_textureSize) {
            std::cerr << "texture size mismatch: " << filepath
                      << " is " << width << "x" << height
                      << " but expected " << m_textureSize << "x" << m_textureSize << std::endl;
            freeImageData(data);
            return nullptr;
        }

        return data;
    }

    void TextureArray::freeImageData(unsigned char *data) {
        stbi_image_free(data);
    }

    std::string TextureArray::getTextureName(const int& i) const {
        std::optional<std::string> foundKey;

        for (const auto& [key, value] : m_textureIndexMap) {
            if (value == i) {
                foundKey = key;
                break;
            }
        }

        if (foundKey.has_value())
            return foundKey.value();
        else
            return "none";
    }


}