//
// Created by selya on 28.02.2019.
//

#ifndef A2D_TEXTURE_ATLAS_HPP
#define A2D_TEXTURE_ATLAS_HPP

#include <a2d/core/intrusive_ptr.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/math/vector.hpp>

#include <memory>
#include <climits>
#include <set>

namespace a2d {

class TextureBuffer;
class Texture;
class TextureRegion;

class TextureAtlas : public ref_counter {
    struct AtlasNode {
        Vector2i origin;
        Vector2i size;
        bool empty;

        std::unique_ptr<AtlasNode> left;
        std::unique_ptr<AtlasNode> right;

        AtlasNode(const Vector2i &origin, const Vector2i &size);
        AtlasNode *InsertData(const TextureBuffer &data, const Vector2i &texture_size);
    };

    AtlasNode root_node = AtlasNode(Vector2i(), Vector2i(INT_MAX));
    intrusive_ptr<Texture> texture;
    std::set<intrusive_ptr<TextureRegion>> managed_regions;
    int max_texture_size;

    void ResizeTexture(const Vector2i &size);

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(TextureAtlas)

    intrusive_ptr<Texture> GetTexture() const;
    intrusive_ptr<TextureRegion> InsertTextureBuffer(const TextureBuffer &data);

    static intrusive_ptr<TextureAtlas> Create(int max_texture_size = 1024 * 16);

private:
    explicit TextureAtlas(int max_texture_size);
};

}

#endif //A2D_TEXTURE_ATLAS_HPP
