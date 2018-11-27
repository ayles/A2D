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


struct TextureBuffer {
    unsigned char * const data;
    int width;
    int height;

    TextureBuffer(int width, int height);

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetPixel(int x, int y, float r, float g, float b, float a);
    void SetPixel(int x, int y, const Vector4f &color);

    void Fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    Vector4f GetPixel(int x, int y);

    ~TextureBuffer();
};



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

    const int width, height;
    const bool mipmaps;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Texture)

    static pTexture GetTexture(const std::string &name);

    Texture(int width, int height, const unsigned char *data, bool flip = false, bool mipmaps = false);
    Texture(const TextureBuffer &texture_buffer, bool flip = false, bool mipmaps = false);

    void SetData(const unsigned char *data, bool flip = false, bool mipmaps = false);

private:
    ~Texture() override;


#if defined(RENDERER_GL) || defined(RENDERER_GL_ES)
    GLuint texture_id;
#endif

    Filtering filtering;
    Wrapping wrapping;

    void Bind(unsigned int texture_unit = 0, Filtering filtering = LINEAR, Wrapping wrapping = REPEAT);

    static void Unbind(unsigned int texture_unit = 0);

    static std::map<std::string, pTexture> textures;
};





DECLARE_SMART_POINTER(TextureRegion)

class TextureRegion : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class SpriteBatch;

public:
    TextureRegion();
    TextureRegion(pTexture texture,
                  Texture::Filtering = Texture::Filtering::LINEAR, Texture::Wrapping = Texture::Wrapping::REPEAT);
    TextureRegion(pTexture texture, int x, int y, int width, int height,
                  Texture::Filtering = Texture::Filtering::LINEAR, Texture::Wrapping = Texture::Wrapping::REPEAT);
    ~TextureRegion() override;

    void SetTexture(const pTexture &texture);
    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFiltering(Texture::Filtering filtering);
    void SetWrapping(Texture::Wrapping wrapping);

    const pTexture &GetTexture();
    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    float GetRatio();
    Texture::Filtering GetFiltering();
    Texture::Wrapping GetWrapping();

    void Bind(unsigned int texture_unit = 0);

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(TextureRegion)

private:
    pTexture texture;
    Vector2i offset;
    Vector2i size;
    Vector2f uv_lb;
    Vector2f uv_rt;
    float ratio;
    Texture::Filtering filtering;
    Texture::Wrapping wrapping;
};



} //namespace a2d


#endif //A2D_TEXTURE_H
