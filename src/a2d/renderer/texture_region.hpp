//
// Created by selya on 13.11.2018.
//

#ifndef A2D_TEXTURE_REGION_H
#define A2D_TEXTURE_REGION_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/renderer/texture.hpp>

namespace a2d {

DECLARE_SMART_POINTER(TextureRegion)

class TextureRegion : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class SpriteBatch;
    friend class Material;

    pTexture texture;
    Vector2i offset;
    Vector2i size;
    Vector2f uv_lower;
    Vector2f uv_upper;
    float ratio;

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

    void SetTexture(const pTexture &texture);
    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFlippedHorizontally(bool flip);
    void SetFlippedVertically(bool flip);

    pTexture GetTexture() const;
    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetRatio() const;
    const Vector2f &GetUVLower() const;
    const Vector2f &GetUVUpper() const;
    bool IsFlippedHorizontally() const;
    bool IsFlippedVertically() const;

    int Bind();

    static pTextureRegion Create();
    static pTextureRegion Create(pTexture texture);
    static pTextureRegion Create(pTexture texture, int x, int y, int width, int height);

private:
    TextureRegion();
    TextureRegion(pTexture texture);
    TextureRegion(pTexture texture, int x, int y, int width, int height);
    ~TextureRegion() override;

    void RecalculateUV();
};



} //namespace a2d


#endif //A2D_TEXTURE_REGION_H
