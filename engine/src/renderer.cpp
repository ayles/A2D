//
// Created by selya on 10.11.2018.
//

#include <A2D/renderer.h>

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
    float half_height = a2d::Engine::camera->size / 2.0f;
    float ratio = (float)a2d::Renderer::width / a2d::Renderer::height;
    a2d::Engine::camera->SetOrtho2D(-half_height * ratio, half_height * ratio, -half_height, half_height);
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