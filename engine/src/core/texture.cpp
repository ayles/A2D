//
// Created by selya on 13.11.2018.
//

#include <a2d/core/texture.h>
#include <a2d/core.h>

#include <lodepng.h>

#include <string>
#include <memory>
#include <vector>

a2d::Texture::Texture(
        int width, int height, const unsigned char *data, bool flip, bool mipmaps
) : width(width), height(height) {
    if ((width & (width - 1)) || (height & (height - 1))) mipmaps = false;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);

    auto final_data = new GLubyte[width * height * 4];

    if (flip) {
        for (int x = 0; x < width * 4; ++x) {
            for (int y = 0; y < height; ++y) {
                final_data[y * width * 4 + x] = data[(height - y - 1) * width * 4 + x];
            }
        }
    } else {
        std::memcpy(final_data, data, width * height * 4);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, final_data);
    if (mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    delete [] final_data;
}

void a2d::Texture::Bind(unsigned int texture_unit) const {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void a2d::Texture::Unbind(unsigned int texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}


a2d::Texture::~Texture() {
    glDeleteTextures(1, &texture_id);
}

std::map<std::string, a2d::pTexture> a2d::Texture::textures = std::map<std::string, pTexture>();

a2d::pTexture a2d::Texture::GetTexture(const std::string &name) {
    auto s = textures.find(name);
    if (s != textures.end()) {
        return s->second;
    }

    std::vector<unsigned char> raw_texture = a2d::FileSystem::LoadRaw("textures/" + name + ".png");
    unsigned int width, height;
    std::vector<unsigned char> image;
    lodepng::decode(image, width, height, raw_texture);

    pTexture texture = new a2d::Texture(width, height, &image[0], true);

    textures[name] = texture;
    return texture;
}

a2d::TextureRegion::TextureRegion() : texture(nullptr), x(0), y(0), width(0), height(0) {

}

a2d::TextureRegion::TextureRegion(pTexture texture) :
texture(texture), x(0), y(0), width(0), height(0),
offset(0), size(1) {

}

a2d::TextureRegion::TextureRegion(pTexture texture, int x, int y, int width, int height) :
texture(texture), x(x), y(y), width(width), height(height),
offset(Vector2f(x, y) / Vector2f(texture->width, texture->height)),
size(Vector2f(width, height) / Vector2f(texture->width, texture->height)){

}

a2d::TextureRegion::~TextureRegion() = default;