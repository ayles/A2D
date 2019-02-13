//
// Created by selya on 28.12.2018.
//

#include <a2d/renderer/texture_buffer.hpp>
#include <a2d/core/log.hpp>

#include <algorithm>


namespace a2d {

TextureBuffer::TextureBuffer(const TextureBuffer &other) : width(other.width), height(other.height) {
    if (!other.data.empty()) {
        data = other.data;
    }
}

TextureBuffer::TextureBuffer(TextureBuffer &&other) noexcept : width(other.width), height(other.height) {
    data.swap(other.data);
}

TextureBuffer::TextureBuffer(int width, int height, const std::vector<unsigned char> &data) :
width(width), height(height) {
    this->data = data;
}

TextureBuffer::~TextureBuffer() {
    Free();
}

TextureBuffer &TextureBuffer::operator=(const a2d::TextureBuffer &other) {
    Free();
    if (!other.data.empty()) {
        Allocate();
        std::copy(other.data.begin(), other.data.begin() + data.size(), data.begin());
    }
    return *this;
}

TextureBuffer &TextureBuffer::operator=(a2d::TextureBuffer &&other) noexcept {
    if (other.data.size() == data.size()) {
        data.swap(other.data);
    } else {
        *this = other;
    }
    return *this;
}

int TextureBuffer::GetWidth() const {
    return width;
}

int TextureBuffer::GetHeight() const {
    return height;
}

void TextureBuffer::Allocate() {
    if (data.empty()) {
        data.resize(width * height * 4);
    }
}

void TextureBuffer::Free() {
    std::vector<unsigned char>().swap(data);
}

void TextureBuffer::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        LOG_TRACE("Pixel out of bounds");
        return;
    }
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

bool TextureBuffer::IsAllocated() const {
    return !data.empty();
}

std::vector<unsigned char> &TextureBuffer::GetBuffer() {
    return data;
}

Vector4f a2d::TextureBuffer::GetPixel(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        LOG_TRACE("Pixel out of bounds");
        return Vector4f();
    }
    if (!IsAllocated()) {
        LOG_TRACE("Buffer is not allocated");
        return Vector4f();
    }
    int offset = (y * width + x) * 4;
    return a2d::Vector4f(
            data[offset] / 255.0f,
            data[offset + 1] / 255.0f,
            data[offset + 2] / 255.0f,
            data[offset + 3] / 255.0f
    );
}

void TextureBuffer::FlipHorizontally() {
    if (!IsAllocated()) return;

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
    if (!IsAllocated()) return;

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

