#pragma once

#include "ResourceLibrary.h"
#include "materials/Shader.h"
#include "materials/Material.h"
#include "TextureArray.h"

namespace Mineclone {
    class AssetManager {
    public:
        AssetManager() = default;

        ResourceLibrary<Material>& materials() { return m_materials; }
        ResourceLibrary<Shader>& shaders() { return m_shaders; }
        ResourceLibrary<Texture>& textures() { return m_textures; }
        TextureArray& blockTextures() { return m_textureArray; }

        const ResourceLibrary<Material>& materials() const { return m_materials; }
        const ResourceLibrary<Shader>& shaders() const { return m_shaders; }
        const ResourceLibrary<Texture>& textures() const { return m_textures; }
        const TextureArray& blockTextures() const { return m_textureArray; }

        bool isMaterialValid(Handle<Material> handle) const;

    private:
        ResourceLibrary<Material> m_materials;
        ResourceLibrary<Shader> m_shaders;
        ResourceLibrary<Texture> m_textures;
        TextureArray m_textureArray;
    };

}
