//
// Created by selya on 10.11.2018.
//

#include <a2d/core/renderer.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/camera.h>

a2d::Vector4f a2d::Renderer::clear_color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
int a2d::Renderer::width = 1;
int a2d::Renderer::height = 1;
std::set<a2d::pSprite, a2d::Renderer::sprites_compare> a2d::Renderer::sprites;

int a2d::Renderer::GetWidth() {
    return width;
}

int a2d::Renderer::GetHeight() {
    return height;
}

bool a2d::Renderer::Initialize() {
    return true;
}

bool a2d::Renderer::PrepareDraw()  {
    if (a2d::Engine::camera) {
        float half_height = a2d::Engine::camera->GetHeight() * 0.5f;
        float ratio = a2d::Engine::camera->GetAspectRatio();
        a2d::Engine::camera->SetOrtho2D(-half_height * ratio, half_height * ratio, -half_height,
                                        half_height);
    }
    return true;
}

void a2d::Renderer::Uninitialize() {

}

void a2d::Renderer::ResolutionChanged(int width, int height) {
    Renderer::width = width;
    Renderer::height = height;
}

void a2d::Renderer::AddSprite(const a2d::pSprite &sprite) {
    sprites.insert(sprite);
}

void a2d::Renderer::RemoveSprite(const a2d::pSprite &sprite) {
    sprites.erase(sprite);
}

bool a2d::Renderer::sprites_compare::operator()(const a2d::pSprite &lhs, const a2d::pSprite &rhs) const {
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