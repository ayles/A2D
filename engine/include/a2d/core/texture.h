//
// Created by selya on 13.11.2018.
//

#ifndef A2D_TEXTURE_H
#define A2D_TEXTURE_H

#include <a2d/core/macro.h>
#include <a2d/core/ref_counter.h>
#include <a2d/math/vector.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#endif

#include <string>
#include <map>

namespace a2d {

DECLARE_SMART_POINTER(Texture)

class Texture : public ref_counter {
    friend class Engine;
    friend class NativeRenderer;
public:
    const int width, height;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Texture)

    static pTexture GetTexture(const std::string &name);

private:
    Texture(int width, int height, const unsigned char *data, bool flip = false, bool mipmaps = false);

    ~Texture() override;


#if defined(RENDERER_GL) || defined(RENDERER_GL_ES)
    GLuint texture_id;
#endif

    void Bind(unsigned int texture_unit = 0) const;

    static void Unbind(unsigned int texture_unit = 0);

    static std::map<std::string, pTexture> textures;
};

DECLARE_SMART_POINTER(TextureRegion)

// TODO add filtering and wrapping here
class TextureRegion : public ref_counter {
    friend class Engine;

public:
    const pTexture texture;
    const int x, y;
    const int width, height;
    const Vector2f offset;
    const Vector2f size;

    TextureRegion();
    TextureRegion(pTexture texture);
    TextureRegion(pTexture texture, int x, int y, int width, int height);
    ~TextureRegion() override;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(TextureRegion)
};

} //namespace a2d


#endif //A2D_TEXTURE_H
