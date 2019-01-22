//
// Created by selya on 28.12.2018.
//

#include <a2d/graphics/texture_buffer.hpp>

#include <algorithm>


namespace a2d {

TextureBuffer::TextureBuffer(const TextureBuffer &other) :
data(nullptr), width(other.width), height(other.height) {
    if (other.data) {
        Allocate();
        std::copy(other.data, other.data + size_t(width * height) * 4, data);
    }
}

TextureBuffer::TextureBuffer(TextureBuffer &&other) noexcept :
data(other.data), width(other.width), height(other.height) {
    other.data = nullptr;
}

TextureBuffer::TextureBuffer(int width, int height, const unsigned char *data) :
data(nullptr), width(width), height(height) {
    if (data) {
        Allocate();
        std::copy(data, data + size_t(width * height) * 4, this->data);
    }
}

TextureBuffer::~TextureBuffer() {
    Free();
}

TextureBuffer &TextureBuffer::operator=(const a2d::TextureBuffer &other) {
    Free();
    SetWidth(other.width);
    SetHeight(other.height);
    if (other.data) {
        Allocate();
        std::copy(other.data, other.data + size_t(width * height) * 4, data);
    }
    return *this;
}

TextureBuffer &TextureBuffer::operator=(a2d::TextureBuffer &&other) noexcept {
    Free();
    SetWidth(other.width);
    SetHeight(other.height);
    data = other.data;
    other.data = nullptr;
    return *this;
}

int TextureBuffer::GetWidth() const {
    return width;
}

int TextureBuffer::GetHeight() const {
    return height;
}

void TextureBuffer::SetWidth(int width) {
    if (this->width != width) Free();
    this->width = width;
}

void TextureBuffer::SetHeight(int height) {
    if (this->height != height) Free();
    this->height = height;
}

void TextureBuffer::Allocate(bool fill_with_zeroes) {
    if (!data) {
        data = new unsigned char[width * height * 4];
        if (fill_with_zeroes) std::fill(this->data, this->data + size_t(width * height) * 4, 0);
    }
}

void TextureBuffer::Free() {
    delete [] data;
    data = nullptr;
}

void TextureBuffer::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    ASSERT(x >= 0 && x < width && y >= 0 && y < height)
    Allocate();
    int offset = (y * width + x) * 4;
    data[offset] = r;
    data[offset + 1] = g;
    data[offset + 2] = b;
    data[offset + 3] = a;
}

void TextureBuffer::SetPixel(int x, int y, float r, float g, float b, float a) {
    SetPixel(x, y,
             (unsigned char)(r * 255),
             (unsigned char)(g * 255),
             (unsigned char)(b * 255),
             (unsigned char)(a * 255)
    );
}

void TextureBuffer::SetPixel(int x, int y, const Vector4f &color) {
    SetPixel(x, y,
             (unsigned char)(color.x * 255),
             (unsigned char)(color.y * 255),
             (unsigned char)(color.z * 255),
             (unsigned char)(color.w * 255)
    );
}

void TextureBuffer::Fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Allocate();
    for (int i = 0; i < width * height * 4; i += 4) {
        data[i] = r;
        data[i + 1] = g;
        data[i + 2] = b;
        data[i + 3] = a;
    }
}

void TextureBuffer::Fill(float r, float g, float b, float a) {
    Fill(
            (unsigned char)(r * 255),
            (unsigned char)(g * 255),
            (unsigned char)(b * 255),
            (unsigned char)(a * 255)
    );
}

void TextureBuffer::Fill(const Vector4f &color) {
    Fill(
            (unsigned char)(color.x * 255),
            (unsigned char)(color.y * 255),
            (unsigned char)(color.z * 255),
            (unsigned char)(color.w * 255)
    );
}

unsigned char *TextureBuffer::GetBuffer() {
    return data;
}

Vector4f a2d::TextureBuffer::GetPixel(int x, int y) const {
    ASSERT(x >= 0 && x < width && y >= 0 && y < height)
    int offset = (y * width + x) * 4;
    return a2d::Vector4f(
            data[offset] / 255.0f,
            data[offset + 1] / 255.0f,
            data[offset + 2] / 255.0f,
            data[offset + 3] / 255.0f
    );
}

void TextureBuffer::FlipHorizontally() {
    if (!data) return;

    for (int x = 0; x < width / 2; ++x) {
        for (int y = 0; y < height; ++y) {
            int i1 = (y * width + (width - x - 1)) * 4;
            int i2 = (y * width + x) * 4;

            unsigned char temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1] = data[i2];
            data[i2] = temp;
        }
    }
}

void TextureBuffer::FlipVertically() {
    if (!data) return;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height / 2; ++y) {
            int i1 = ((height - y - 1) * width + x) * 4;
            int i2 = (y * width + x) * 4;

            unsigned char temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1++] = data[i2];
            data[i2++] = temp;

            temp = data[i1];
            data[i1] = data[i2];
            data[i2] = temp;
        }
    }
}

} //namespace a2d

