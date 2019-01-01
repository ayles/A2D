//
// Created by selya on 13.11.2018.
//

#ifndef A2D_TEXTURE_REGION_H
#define A2D_TEXTURE_REGION_H

#include <a2d/core/macro.h>
#include <a2d/core/ref_counter.h>
#include <a2d/math/vector.h>
#include <a2d/graphics/texture.h>

namespace a2d {

DECLARE_SMART_POINTER(TextureRegion)

class TextureRegion : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class SpriteBatch;

    pTexture texture;
    Vector2i offset;
    Vector2i size;
    Vector2f uv_lb;
    Vector2f uv_rt;
    float ratio;
    Texture::Filtering filtering;
    Texture::Wrapping wrapping;

    // 7
    // 6
    // 5
    // 4
    // 3
    // 2
    // 1 - vertical flip flag
    // 0 - horizontal flip flag
    unsigned char flags;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(TextureRegion)
    TextureRegion();
    TextureRegion(pTexture texture,
                  Texture::Filtering = Texture::Filtering::LINEAR, Texture::Wrapping = Texture::Wrapping::REPEAT);
    TextureRegion(pTexture texture, int x, int y, int width, int height,
                  Texture::Filtering = Texture::Filtering::LINEAR, Texture::Wrapping = Texture::Wrapping::REPEAT);
    ~TextureRegion() override;

    void SetTexture(const pTexture &texture);
    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFiltering(Texture::Filtering filtering);
    void SetWrapping(Texture::Wrapping wrapping);
    void SetFlippedHorizontally(bool flip);
    void SetFlippedVertically(bool flip);

    pTexture GetTexture() const;
    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetRatio() const;
    Texture::Filtering GetFiltering() const;
    Texture::Wrapping GetWrapping() const;
    bool IsFlippedHorizontally() const;
    bool IsFlippedVertically() const;

    void Bind(unsigned int texture_unit = 0);

private:
    void RecalculateUV();
};



} //namespace a2d


#endif //A2D_TEXTURE_REGION_H
