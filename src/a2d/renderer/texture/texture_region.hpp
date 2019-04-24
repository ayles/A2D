//
// Created by selya on 13.11.2018.
//

#ifndef A2D_TEXTURE_REGION_H
#define A2D_TEXTURE_REGION_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/math/vector.hpp>

namespace a2d {

class Texture;

class TextureRegion : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class SpriteBatch;
    friend class Material;

    intrusive_ptr<Texture> texture;
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

    void SetTexture(const intrusive_ptr<Texture> &texture);
    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetFlippedHorizontally(bool flip);
    void SetFlippedVertically(bool flip);

    intrusive_ptr<Texture> GetTexture() const;
    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    float GetRatio() const;
    const Vector2f &GetUVLower() const;
    const Vector2f &GetUVUpper() const;
    bool IsFlippedHorizontally() const;
    bool IsFlippedVertically() const;

    static intrusive_ptr<TextureRegion> Create();
    static intrusive_ptr<TextureRegion> Create(intrusive_ptr<Texture> texture);
    static intrusive_ptr<TextureRegion> Create(intrusive_ptr<Texture> texture, int x, int y, int width, int height);

private:
    TextureRegion();
    TextureRegion(intrusive_ptr<Texture> texture);
    TextureRegion(intrusive_ptr<Texture> texture, int x, int y, int width, int height);
    ~TextureRegion() override;

    void RecalculateUV();
};



} //namespace a2d


#endif //A2D_TEXTURE_REGION_H
