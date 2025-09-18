
#pragma once
#include <string>
#include <unordered_map>
#include "glad/glad.h"

namespace Mineclone {
    class TextureArray {
    public:
        explicit TextureArray(int size = 16, int maxTextures = 256);
        ~TextureArray();

        TextureArray(const TextureArray&) = delete;
        TextureArray& operator=(const TextureArray&) = delete;

        int addTexture(const std::string& name, const std::string& filepath);

        // this uses the name of the file without extension as name for the texture
        int addTextures(const std::string& directory);
        int getTextureIndex(const std::string& name) const;

        void bind(GLuint unit = 0) const;
        void unbind() const;

        void generateMipmaps() const;

        GLuint getTextureID() const;
        int getTextureCount() const;

        std::string getTextureName(const int& i) const;

    private:
        GLuint m_id;
        int m_textureSize;
        int m_maxLayers;
        int m_currentLayers;
        std::unordered_map<std::string, int> m_textureIndexMap;

        unsigned char* loadImageData(const std::string& filepath);

        void freeImageData(unsigned char* data);

    };

}
