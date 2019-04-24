//
// Created by selya on 28.12.2018.
//

#ifndef A2D_TEXTURE_BUFFER_HPP
#define A2D_TEXTURE_BUFFER_HPP

#include <a2d/math/vector.hpp>

#include <vector>

namespace a2d {

class TextureBuffer {
    std::vector<unsigned char> data;
    const int width;
    const int height;

public:
    TextureBuffer(const TextureBuffer &other);
    TextureBuffer(TextureBuffer &&other) noexcept;
    TextureBuffer(int width, int height, const std::vector<unsigned char> &data = std::vector<unsigned char>());
    ~TextureBuffer();

    TextureBuffer &operator=(const TextureBuffer &other);
    TextureBuffer &operator=(TextureBuffer &&other) noexcept;

    int GetWidth() const;
    int GetHeight() const;

    void Allocate();
    void Free();

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetPixel(int x, int y, float r, float g, float b, float a);
    void SetPixel(int x, int y, const Vector4f &color);

    void SetRect(int x, int y, const TextureBuffer &rect);

    void Fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void Fill(float r, float g, float b, float a);
    void Fill(const Vector4f &color);

    void FlipHorizontally();
    void FlipVertically();

    std::vector<unsigned char> &GetBuffer();
    bool IsAllocated() const;
    Vector4f GetPixel(int x, int y) const;
};

} //namespace a2d

#endif //A2D_TEXTURE_BUFFER_HPP
