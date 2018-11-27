//
// Created by selya on 13.11.2018.
//

#include <a2d/core/texture.h>
#include <a2d/core.h>

#include <lodepng.h>

#include <string>
#include <memory>
#include <vector>

a2d::Texture::Texture(const a2d::TextureBuffer &texture_buffer, bool flip, bool mipmaps) :
Texture(texture_buffer.width, texture_buffer.height, texture_buffer.data, flip, mipmaps) {

}


a2d::Texture::Texture(
        int width, int height, const unsigned char *data, bool flip, bool mipmaps
) : width(width), height(height), mipmaps(mipmaps), filtering(NEAREST), wrapping(REPEAT) {
    if ((width & (width - 1)) || (height & (height - 1))) mipmaps = false;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (data) SetData(data, flip, mipmaps);
}

void a2d::Texture::SetData(const unsigned char *data, bool flip, bool mipmaps) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
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

void a2d::Texture::Bind(unsigned int texture_unit, Filtering filtering, Wrapping wrapping) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (this->filtering != filtering) {
        GLuint f;
        switch (filtering) {
            case NEAREST: f = GL_NEAREST; break;
            case LINEAR: f = GL_LINEAR; break;
            case TRILINEAR: f = GL_LINEAR_MIPMAP_LINEAR; break;
            default: return;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        this->filtering = filtering;
    }

    if (this->wrapping != wrapping) {
        GLuint w;
        switch (wrapping) {
            case EDGE: w = GL_CLAMP_TO_EDGE; break;
            case REPEAT: w = GL_REPEAT; break;
            case MIRROR: w = GL_MIRRORED_REPEAT; break;
            default: return;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, w);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, w);
        this->wrapping = wrapping;
    }
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


a2d::TextureRegion::TextureRegion() :
texture(nullptr),
offset(0), size(0), uv_lb(0), uv_rt(0), ratio(1),
filtering(Texture::Filtering::LINEAR), wrapping(Texture::Wrapping::REPEAT)
{

}


a2d::TextureRegion::TextureRegion(
        pTexture texture,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) :
texture(texture),
offset(0), size(texture->width, texture->height), uv_lb(0), uv_rt(1), ratio(std::abs((float)size.x / size.y)),
filtering(filtering), wrapping(wrapping)
{

}

a2d::TextureRegion::TextureRegion(
        pTexture texture,
        int x, int y, int width, int height,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) :
texture(texture),
offset(x, y), size(width, height),
uv_lb(Vector2f(x, y) / Vector2f(texture->width, texture->height)),
uv_rt(Vector2f(x + width, y + height) / Vector2f(texture->width, texture->height)),
ratio(std::abs((float)size.x / size.y)),
filtering(filtering), wrapping(wrapping)
{

}

void a2d::TextureRegion::Bind(unsigned int texture_unit) {
    if (!texture) return;
    texture->Bind(texture_unit, filtering, wrapping);
}

/*void a2d::TextureRegion::SetTexture(const a2d::pTexture &texture) {

}

void a2d::TextureRegion::SetX(int x) {

}

void a2d::TextureRegion::SetY(int y) {

}

void a2d::TextureRegion::SetWidth(int width) {

}

void a2d::TextureRegion::SetHeight(int height) {

}

void a2d::TextureRegion::SetFiltering(a2d::Texture::Filtering filtering) {

}

void a2d::TextureRegion::SetWrapping(a2d::Texture::Wrapping wrapping) {

}*/

const a2d::pTexture &a2d::TextureRegion::GetTexture() {
    return texture;
}

int a2d::TextureRegion::GetX() {
    return offset.x;
}

int a2d::TextureRegion::GetY() {
    return offset.y;
}

int a2d::TextureRegion::GetWidth() {
    return size.x;
}

int a2d::TextureRegion::GetHeight() {
    return size.y;
}

float a2d::TextureRegion::GetRatio() {
    return ratio;
}

a2d::Texture::Filtering a2d::TextureRegion::GetFiltering() {
    return filtering;
}

a2d::Texture::Wrapping a2d::TextureRegion::GetWrapping() {
    return wrapping;
}

a2d::TextureRegion::~TextureRegion() = default;




a2d::TextureBuffer::TextureBuffer(int width, int height) : data(new unsigned char[width * height * 4]), width(width), height(height) {

}

void a2d::TextureBuffer::SetPixel(int x, int y, float r, float g, float b, float a) {
    SetPixel(x, y,
            (unsigned char)(r * 255),
            (unsigned char)(g * 255),
            (unsigned char)(b * 255),
            (unsigned char)(a * 255)
    );
}

void a2d::TextureBuffer::SetPixel(int x, int y, const a2d::Vector4f &color) {
    SetPixel(x, y, color.x, color.y, color.z, color.w);
}

void a2d::TextureBuffer::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int offset = (y * width + x) * 4;
    data[offset] = r;
    data[offset + 1] = g;
    data[offset + 2] = b;
    data[offset + 3] = a;
}

void a2d::TextureBuffer::Fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    for (int i = 0; i < width * height * 4; i += 4) {
        data[i] = r;
        data[i + 1] = g;
        data[i + 2] = b;
        data[i + 3] = a;
    }
}

a2d::Vector4f a2d::TextureBuffer::GetPixel(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return Vector4f();
    int offset = (y * width + x) * 4;
    return a2d::Vector4f(
            data[offset] / 255.0f,
            data[offset + 1] / 255.0f,
            data[offset + 2] / 255.0f,
            data[offset + 3] / 255.0f
    );
}

a2d::TextureBuffer::~TextureBuffer() {
    delete [] data;
}
