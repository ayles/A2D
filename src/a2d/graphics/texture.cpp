//
// Created by selya on 29.12.2018.
//

#include <a2d/graphics/texture.hpp>
#include <a2d/filesystem/filesystem.hpp>
#include <a2d/graphics/gl.hpp>
#include <a2d/core/engine.hpp>
#include "texture.hpp"


namespace a2d {

std::map<std::string, pTexture> Texture::textures;

Texture::Texture(
        int width, int height, const unsigned char *data, bool mipmaps
) : Texture(TextureBuffer(width, height, data), mipmaps) {
    ASSERT_MAIN_THREAD
}

Texture::Texture(const TextureBuffer &buffer, bool mipmaps) :
mipmaps(mipmaps), buffer(buffer), texture_id(0), filtering(NEAREST), wrapping(REPEAT) {
    ASSERT_MAIN_THREAD
}

Texture::Texture(TextureBuffer &&buffer, bool mipmaps) :
mipmaps(mipmaps), buffer(std::move(buffer)), texture_id(0), filtering(NEAREST), wrapping(REPEAT) {
    ASSERT_MAIN_THREAD
}

Texture::~Texture() {
    ASSERT_MAIN_THREAD
    glDeleteTextures(1, &texture_id);
}

int Texture::GetWidth() const {
    ASSERT_MAIN_THREAD
    return buffer.GetWidth();
}

int Texture::GetHeight() const {
    ASSERT_MAIN_THREAD
    return buffer.GetHeight();
}

void Texture::Load() {
    ASSERT_MAIN_THREAD
    if (texture_id) return;

    if ((GetWidth() & (GetWidth() - 1)) || (GetHeight() & (GetHeight() - 1))) mipmaps = false;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.GetBuffer());
    if (mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Unload() {
    ASSERT_MAIN_THREAD
    if (!texture_id) return;

    glDeleteTextures(1, &texture_id);
    texture_id = 0;
}

void Texture::Bind(unsigned int texture_unit, Filtering filtering, Wrapping wrapping) {
    ASSERT_MAIN_THREAD
    Load();

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (this->filtering != filtering) {
        GLuint f;
        switch (filtering) {
            case NEAREST:
                f = GL_NEAREST;
                break;
            case LINEAR:
                f = GL_LINEAR;
                break;
            case TRILINEAR:
                f = GL_LINEAR_MIPMAP_LINEAR;
                break;
            default:
                return;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        this->filtering = filtering;
    }

    if (this->wrapping != wrapping) {
        GLuint w;
        switch (wrapping) {
            case EDGE:
                w = GL_CLAMP_TO_EDGE;
                break;
            case REPEAT:
                w = GL_REPEAT;
                break;
            case MIRROR:
                w = GL_MIRRORED_REPEAT;
                break;
            default:
                return;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, w);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, w);
        this->wrapping = wrapping;
    }
}

void Texture::Unbind(unsigned int texture_unit) {
    ASSERT_MAIN_THREAD
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

pTexture Texture::Create(int width, int height, const unsigned char *data, bool mipmaps) {
    return new Texture(width, height, data, mipmaps);
}

pTexture Texture::Create(const TextureBuffer &buffer, bool mipmaps) {
    return new Texture(buffer, mipmaps);
}

pTexture Texture::Create(TextureBuffer &&buffer, bool mipmaps) {
    return new Texture(std::move(buffer), mipmaps);
}


} //namespace a2d