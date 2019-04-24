//
// Created by selya on 25.04.19.
//

#include <a2d/renderer/texture/texture_atlas.hpp>
#include <a2d/renderer/texture/texture_buffer.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/texture/texture.hpp>

namespace a2d {

TextureAtlas::AtlasNode::AtlasNode(const Vector2i &origin, const Vector2i &size) : origin(origin), size(size), empty(true) {}

TextureAtlas::AtlasNode *TextureAtlas::AtlasNode::InsertData(const TextureBuffer &data, const Vector2i &texture_size) {
    // Algorithm from https://straypixels.net/texture-packing-for-fonts/
    if (!empty) {
        // The node is filled, not gonna fit anything else here
        return nullptr;
    } else if (left && right) {
        // Non-leaf, try inserting to the left and then to the right
        auto l_res = left->InsertData(data, texture_size);
        if (l_res) return l_res;
        return right->InsertData(data, texture_size);
    } else {
        // This is an unfilled leaf - let's see if we can fill it
        Vector2i actual_size(size);

        if (origin.x + size.x == INT_MAX) {
            actual_size.x = texture_size.x - origin.x;
        }
        if (origin.y + size.y == INT_MAX) {
            actual_size.y = texture_size.y - origin.y;
        }

        Vector2i data_dimensions(data.GetWidth(), data.GetHeight());
        if (size.x == data_dimensions.x && size.y == data_dimensions.y) {
            // Perfect size - just pack into this node
            empty = false;
            return this;
        } else if (actual_size.x < data_dimensions.x || actual_size.y < data_dimensions.y) {
            // Not big enough
            return nullptr;
        } else {
            // Large enough - split until we get a perfect fit
            AtlasNode *left;
            AtlasNode *right;

            // Determine how much space we'll have left if we split each way
            Vector2i remain = actual_size - data_dimensions;

            // Split the way that will leave the most room
            bool vertical_split = remain.x < remain.y;
            if (remain == 0) {
                // Edge case - we are are going to hit the border of
                // the texture atlas perfectly, split at the border instead
                vertical_split = size.x < size.y;
            }

            if (vertical_split) {
                // Split vertically (left is top)
                left = new AtlasNode(origin, Vector2i(size.x, data_dimensions.y));
                right = new AtlasNode(Vector2i(origin.x, origin.y + data_dimensions.y),
                                      Vector2i(size.x, size.y - data_dimensions.y));
            } else {
                // Split horizontally
                left = new AtlasNode(origin, Vector2i(data_dimensions.x, size.y));
                right = new AtlasNode(Vector2i(origin.x + data_dimensions.x, origin.y),
                                      Vector2i(size.x - data_dimensions.x, size.y));
            }

            this->left = std::unique_ptr<AtlasNode>(left);
            this->right = std::unique_ptr<AtlasNode>(right);

            return left->InsertData(data, texture_size);
        }
    }
}

void TextureAtlas::ResizeTexture(const Vector2i &size) {
    if (!texture) {
        texture = Texture::Create(size.x, size.y);
        return;
    }
    auto new_texture = Texture::Create(size.x, size.y);
    new_texture->GetBuffer().SetRect(0, 0, texture->GetBuffer());
    texture->Unload();
    texture->GetBuffer().Free();
    new_texture.swap(texture);
    for (auto &texture_region : managed_regions) {
        texture_region->SetTexture(texture);
    }
}

intrusive_ptr<Texture> TextureAtlas::GetTexture() const {
    return texture;
}

intrusive_ptr<TextureRegion> TextureAtlas::InsertTextureBuffer(const TextureBuffer &data) {
    if (!texture) ResizeTexture(Vector2i(32));
    AtlasNode *node;
    do {
        node = root_node.InsertData(data, Vector2i(texture->GetWidth(), texture->GetHeight()));
        if (node) break;
        if (texture->GetHeight() * 2 > max_texture_size) return nullptr;
        ResizeTexture(Vector2i(texture->GetWidth() * 2, texture->GetHeight() * 2));
    } while (true);
    texture->GetBuffer().SetRect(node->origin.x, node->origin.y, data);
    auto texture_region = TextureRegion::Create(
            texture, node->origin.x, node->origin.y, data.GetWidth(), data.GetHeight());
    managed_regions.emplace(texture_region);
    return texture_region;
}

intrusive_ptr<TextureAtlas> TextureAtlas::Create(int max_texture_size) {
    return new TextureAtlas(max_texture_size);
}

TextureAtlas::TextureAtlas(int max_texture_size) : max_texture_size(max_texture_size) {}
}
