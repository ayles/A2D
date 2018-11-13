//
// Created by selya on 13.11.2018.
//

#include <a2d/core/texture.h>

#include <string>
#include <memory>

a2d::Texture::Texture(
        int width, int height, const unsigned char *data, bool flip, bool mipmaps
) : width(width), height(height) {
    if ((width & (width - 1)) || (height & (height - 1))) mipmaps = false;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

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