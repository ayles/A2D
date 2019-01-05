# A2D

Small cross-platform 2D game engine.

# Structure

A2D consists of 3 main parts:

- **Engine** - common engine code.
- **Platfrom projects** - projects for building for a specific platform.
- **Core project** - custom project. You should write your code in it.

For now A2D have no project generating utility so any contributions in
this direction will be appreciated.

# Use (Build & Run)

For simple demonstration just clone this repo and build any of 
available platform projects.

For creating your own game with A2D clone this repo and edit/write code
in `project_core`. Then build any of platform projects.

# Supported platforms

- **Windows** (MSVC)
- **Linux** (tested partially)
- **macOS** (tested partially)
- **Android** 
- **IOS** (in progress)

# Getting Started

For starting your own project you should implement `a2d::RootComponent` class
and store it in the `root_component.hpp` in the root of your project source tree.

`RootComponent` will be created on the `Engine.root` after engine initialization.

**Sample code:**

```cpp
namespace a2d {

class RootComponent : public Component {
    pObject2D text;
    
    // Initialize event is called after component creation
    void Initialize() override {
        // Create and set pixel camera
        auto camera_object = Engine::GetRoot()->AddChild(new Object2D);
        Engine::SetCamera(camera_object->AddComponent<PixelCamera>());
        
        // Create text
        text = Engine::GetRoot()->AddChild(new Object2D);
        text->AddComponent<Text>()->SetFont(
            new BitmapFont(FileSystem::LoadRaw("fonts/impact.ttf"), 96)
        );
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.5f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }
    
    // Update event is called each frame
    void Update() override {
        // Make it spin
        text->rotation += Engine::GetDeltaTime();
    }
}

}
```

# Future plans

- Integrate Box2D
- Add IOS bindings
- Add platform-specific settings such as screen orientation (landscape/portrait) and other

# Contact & Contribute

This project is small, so all contributions are welcome.

You can contact with me by email <selya.main@gmail.com>.