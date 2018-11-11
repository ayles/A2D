//
// Created by selya on 26.10.2018.
//

#ifndef A2D_RENDER_H
#define A2D_RENDER_H

#include <A2D/math.h>
#include <A2D/core/macro.h>
#include <A2D/core/engine.h>
#include <A2D/core/components/sprite.h>

#include <set>
#include <stack>

namespace a2d {

class Renderer {
    friend class Sprite;

    friend class NativeRenderer;

public:
    Renderer() = delete;

    static a2d::Vector4f clear_color;

private:
    static int width;
    static int height;

    struct sprites_compare {
        bool operator()(const pSprite &lhs, const pSprite &rhs) const {
            std::stack<Object2D *> ls;
            std::stack<Object2D *> rs;
            ls.push(lhs->GetObject2D().get());
            rs.push(rhs->GetObject2D().get());
            while (ls.top()) {
                ls.push(ls.top()->GetParent().get());
            }
            while (rs.top()) {
                rs.push(rs.top()->GetParent().get());
            }
            while (true) {
                ls.pop();
                rs.pop();
                if (ls.top()->GetLayer() != rs.top()->GetLayer()) {
                    return ls.top()->GetLayer() < rs.top()->GetLayer();
                } else if (ls.size() <= 1 || rs.size() <= 1) {
                    // TODO compare by material
                    return lhs < rhs;
                }
            }
        }
    };

    static std::set<pSprite, sprites_compare> sprites;

    static int GetWidth();
    static int GetHeight();

    static bool Initialize();
    static bool PrepareDraw();
    static void Uninitialize();

    static void ResolutionChanged(int width, int height);
    static void AddSprite(const pSprite &sprite);
    static void RemoveSprite(const pSprite &sprite);
};

} //namespace a2d

#endif //A2D_RENDER_H
