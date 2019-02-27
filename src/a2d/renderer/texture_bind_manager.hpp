//
// Created by selya on 27.02.2019.
//

#ifndef A2D_TEXTURE_BIND_MANAGER_HPP
#define A2D_TEXTURE_BIND_MANAGER_HPP

#include <a2d/renderer/gl.hpp>
#include <a2d/renderer/texture.hpp>

#include <vector>
#include <map>
#include <list>
#include <algorithm>

namespace a2d {

class TextureBindManager {
    friend class Texture;
    friend class Renderer;

    struct TextureUnit {
        int unit_index;
        unsigned int bound_texture_handle;
        std::list<TextureUnit *>::const_iterator iter_in_queue;

        TextureUnit(int unit_index);
    };

    static std::vector<TextureUnit> units;
    static std::map<unsigned int, TextureUnit *> handle_to_unit;
    static std::list<TextureUnit *> units_queue;
    static int active_unit;

public:
    static void ActiveUnit(int unit_index);
    static int GetActiveUnit();

private:
    static void Initialize(int units_count);
    static void BindTextureToUnit(Texture *texture, int unit_index);
    static int BindTextureToCurrentUnit(Texture *texture);
    static int BindTexture(Texture *texture);
    static void UnbindTexture(unsigned int texture_handle);
};

}

#endif //A2D_TEXTURE_BIND_MANAGER_HPP
