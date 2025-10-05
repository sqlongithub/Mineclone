
#include "AssetManager.h"

namespace Mineclone {
    bool AssetManager::isMaterialValid(Handle<Material> handle) const {
        const Material* mat = m_materials.get(handle);
        if (!mat) return false;

        return (!mat->shader.isValid() || m_shaders.isValid(mat->shader));
    }
}