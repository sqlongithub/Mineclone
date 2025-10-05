#pragma once

#include "ResourceLibrary.h"
#include "client/rendering/materials/Material.h"
#include "client/rendering/materials/Shader.h"
#include "client/rendering/materials/Texture.h"
#include "client/rendering/materials/TextureArray.h"


namespace Mineclone {
    class AssetManager {
    public:
        AssetManager() = default;

        ResourceLibrary<Material>& materials() { return m_materials; }
        ResourceLibrary<Shader>& shaders() { return m_shaders; }
        ResourceLibrary<Texture>& textures() { return m_textures; }
        TextureArray& blockTextures() { return m_textureArray; }

        [[nodiscard]] const ResourceLibrary<Material>& materials() const { return m_materials; }
        [[nodiscard]] const ResourceLibrary<Shader>& shaders() const { return m_shaders; }
        [[nodiscard]] const ResourceLibrary<Texture>& textures() const { return m_textures; }
        [[nodiscard]] const TextureArray& blockTextures() const { return m_textureArray; }

        [[nodiscard]] bool isMaterialValid(Handle<Material> handle) const;

    private:
        ResourceLibrary<Material> m_materials;
        ResourceLibrary<Shader> m_shaders;
        ResourceLibrary<Texture> m_textures;
        TextureArray m_textureArray;
    };

}
