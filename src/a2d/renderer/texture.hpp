//
// Created by selya on 29.12.2018.
//

#ifndef A2D_TEXTURE_H
#define A2D_TEXTURE_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/renderer/texture_buffer.hpp>
#include <a2d/renderer/gl.hpp>

#include <string>
#include <vector>
#include <map>

namespace a2d {

DECLARE_SMART_POINTER(Texture)

class Texture : public ref_counter {
    friend class Renderer;
    friend class TextureBindManager;

public:
    enum Filtering {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
        TRILINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    enum Wrapping {
        EDGE = GL_CLAMP_TO_EDGE,
        REPEAT = GL_REPEAT,
        MIRROR = GL_MIRRORED_REPEAT
    };

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Texture)

    int GetWidth() const;
    int GetHeight() const;
    bool HaveMipmaps() const;
    bool IsLoaded() const;
    TextureBuffer &GetBuffer();
    Filtering GetFiltering() const;
    Wrapping GetWrapping() const;
    unsigned int GetHandle();

    void Load();
    void Unload();
    void FetchBuffer();
    void Reload();
    void BindToActiveUnit();
    int Bind();

    void SetFiltering(Filtering filtering);
    void SetWrapping(Wrapping wrapping);

    static pTexture Create(int width, int height,
            const std::vector<unsigned char> &data = std::vector<unsigned char>(), bool mipmaps = false);
    static pTexture Create(const TextureBuffer &buffer, bool mipmaps = false);
    static pTexture Create(TextureBuffer &&buffer, bool mipmaps = false);

private:
    unsigned int texture_handle;
    Filtering filtering;
    Wrapping wrapping;
    TextureBuffer buffer;
    const bool mipmaps;
    bool filtering_update_required;
    bool wrapping_update_required;

    Texture(int width, int height, const std::vector<unsigned char> &data, bool mipmaps = false);
    Texture(const TextureBuffer &buffer, bool mipmaps = false);
    Texture(TextureBuffer &&buffer, bool mipmaps = false);
    ~Texture() override;

    void BindToGL();
};

} //namespace a2d

#endif //A2D_TEXTURE_H
