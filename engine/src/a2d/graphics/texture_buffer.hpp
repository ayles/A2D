//
// Created by selya on 28.12.2018.
//

#ifndef A2D_TEXTURE_BUFFER_H
#define A2D_TEXTURE_BUFFER_H

#include <a2d/core/macro.hpp>
#include <a2d/math/vector.hpp>

namespace a2d {

class TextureBuffer {
    unsigned char *data;
    int width;
    int height;

public:
    TextureBuffer(const TextureBuffer &other);
    TextureBuffer(TextureBuffer &&other) noexcept;
    TextureBuffer(int width, int height, const unsigned char *data = nullptr);
    ~TextureBuffer();

    TextureBuffer &operator=(const TextureBuffer &other);
    TextureBuffer &operator=(TextureBuffer &&other) noexcept;

    int GetWidth() const;
    int GetHeight() const;
    void SetWidth(int width);
    void SetHeight(int height);

    void Allocate(bool fill_with_zeroes = false);
    void Free();

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetPixel(int x, int y, float r, float g, float b, float a);
    void SetPixel(int x, int y, const Vector4f &color);

    void Fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void Fill(float r, float g, float b, float a);
    void Fill(const Vector4f &color);

    void FlipHorizontally();
    void FlipVertically();

    unsigned char *GetBuffer();

    Vector4f GetPixel(int x, int y) const;
};

} //namespace a2d

#endif //A2D_TEXTURE_BUFFER_H
