//
// Created by Alexander on 2019-01-04.
//

#include <a2d/a2d.hpp>
#include <root_component.hpp>

namespace a2d {
    class NativeConnector {
    public:
        static bool Initialize() {
            if (!a2d::Audio::Initialize()) return false;
            if (!a2d::Engine::Initialize()) return false;
            if (!a2d::Renderer::Initialize()) return false;
            if (!a2d::Input::Initialize()) return false;
            if (!a2d::Physics::Initialize()) return false;
            a2d::Engine::GetRoot()->AddComponent<a2d::RootComponent>();
            return true;
        }

        static bool Step() {
            return a2d::Physics::Step() &&
                   a2d::Engine::Update() &&
                   a2d::Engine::PostUpdate() &&
                   a2d::Engine::PreDraw() &&
                   a2d::Renderer::Draw() &&
                   a2d::Engine::PostDraw();
        }

        static void Uninitialize() {
            a2d::Physics::Uninitialize();
            a2d::Input::Uninitialize();
            a2d::Engine::Uninitialize();
            a2d::Renderer::Uninitialize();
            a2d::Audio::Uninitialize();
        }

        static void ResolutionChanged(int width, int height, int framebuffer_width, int framebuffer_height) {
            a2d::Renderer::ResolutionChanged(width, height, framebuffer_width, framebuffer_height);
        }

        static void OnPause() {
            a2d::Engine::OnPause();
        }

        static void OnResume() {
            a2d::Engine::OnResume();
        }

        static void OnTouch(int touches_count, int touch_index, a2d::Input::TouchEvent touch_event, float touch_x, float touch_y) {
            a2d::Input::TouchCallback(touches_count, touch_index, touch_event, touch_x, touch_y);
        }
    };
}

enum TouchPhase {
    BEGAN = 0,
    MOVED,
    STATIONARY,
    ENDED,
    CANCELLED
};

extern "C" {
    void a2d_initialize() {
        a2d::NativeConnector::Initialize();
    }

    void a2d_step() {
        a2d::NativeConnector::Step();
    }

    void a2d_uninitialize() {
        a2d::NativeConnector::Uninitialize();
    }

    void a2d_resolution_changed(int width, int height, int framebuffer_width, int framebuffer_height) {
        a2d::NativeConnector::ResolutionChanged(width, height, framebuffer_width, framebuffer_height);
    }
    
    void a2d_touch_event(int touches_count, int touch_index, int touch_phase, float touch_x, float touch_y) {
        switch (touch_phase) {
            case TouchPhase::BEGAN: a2d::NativeConnector::OnTouch(touches_count, touch_index, a2d::Input::TouchEvent::TOUCH_BEGAN, touch_x, touch_y); break;
            case TouchPhase::MOVED:
            case TouchPhase::STATIONARY: a2d::NativeConnector::OnTouch(touches_count, touch_index, a2d::Input::TouchEvent::TOUCH_MOVED, touch_x, touch_y); break;
            case TouchPhase::ENDED:
            case TouchPhase::CANCELLED: a2d::NativeConnector::OnTouch(touches_count, touch_index, a2d::Input::TouchEvent::TOUCH_ENDED, touch_x, touch_y); break;
            default: break;
        }
    }
}
