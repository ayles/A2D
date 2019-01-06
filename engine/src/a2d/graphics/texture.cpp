//
// Created by selya on 29.12.2018.
//

#include <a2d/graphics/texture.hpp>
#include <a2d/filesystem/filesystem.hpp>

#include <a2d/graphics/gl.hpp>

#include <lodepng.h>


namespace a2d {

std::map<std::string, pTexture> Texture::textures;

Texture::Texture(
        int width, int height, const unsigned char *data, bool mipmaps
) : Texture(TextureBuffer(width, height, data), mipmaps) {}

Texture::Texture(const TextureBuffer &buffer, bool mipmaps) :
mipmaps(mipmaps), buffer(buffer), texture_id(0), filtering(NEAREST), wrapping(REPEAT) {}

Texture::Texture(TextureBuffer &&buffer, bool mipmaps) :
mipmaps(mipmaps), buffer(std::move(buffer)), texture_id(0), filtering(NEAREST), wrapping(REPEAT) {}

int Texture::GetWidth() const {
    return buffer.GetWidth();
}

int Texture::GetHeight() const {
    return buffer.GetHeight();
}

void Texture::Load() {
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
    if (!texture_id) return;

    glDeleteTextures(1, &texture_id);
    texture_id = 0;
}

void Texture::Bind(unsigned int texture_unit, Filtering filtering, Wrapping wrapping) {
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
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture() {
    glDeleteTextures(1, &texture_id);
}

pTexture Texture::GetTexture(const std::string &name) {
    auto s = textures.find(name);
    if (s != textures.end()) {
        return s->second;
    }

    std::vector<unsigned char> raw_texture = FileSystem::LoadRaw("textures/" + name + ".png");
    unsigned int width, height;
    std::vector<unsigned char> image;
    lodepng::decode(image, width, height, raw_texture);

    pTexture texture = new Texture(width, height, &image[0]);
    texture->buffer.FlipVertically();

    textures[name] = texture;
    return texture;
}

} //namespace a2d