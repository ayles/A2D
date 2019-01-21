//
// Created by selya on 29.12.2018.
//

#ifndef A2D_TEXTURE_H
#define A2D_TEXTURE_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/graphics/texture_buffer.hpp>

#include <string>
#include <map>

namespace a2d {

DECLARE_SMART_POINTER(Texture)

class Texture : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class TextureRegion;
    friend class SpriteBatch;

public:
    enum Filtering {
        NEAREST,
        LINEAR,
        TRILINEAR
    };

    enum Wrapping {
        EDGE,
        REPEAT,
        MIRROR
    };

    bool mipmaps;
    TextureBuffer buffer;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Texture)
    Texture(int width, int height, const unsigned char *data = nullptr, bool mipmaps = false);
    Texture(const TextureBuffer &buffer, bool mipmaps = false);
    Texture(TextureBuffer &&buffer, bool mipmaps = false);
    ~Texture() override;

    int GetWidth() const;
    int GetHeight() const;

    void Load();
    void Unload();
    void Bind(unsigned int texture_unit = 0, Filtering filtering = NEAREST, Wrapping wrapping = REPEAT);

    static void Unbind(unsigned int texture_unit = 0);
private:
    unsigned int texture_id;
    Filtering filtering;
    Wrapping wrapping;

    static std::map<std::string, pTexture> textures;
};

} //namespace a2d

#endif //A2D_TEXTURE_H
