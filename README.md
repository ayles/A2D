# A2D

Small cross-platform 2D game engine.

[![Build status](https://ci.appveyor.com/api/projects/status/kuhmfco62gwb6kco?svg=true)](https://ci.appveyor.com/project/ayles/a2d)
[![Build Status](https://travis-ci.com/ayles/A2D.svg?branch=master)](https://travis-ci.com/ayles/A2D)
[![CodeFactor](https://www.codefactor.io/repository/github/ayles/a2d/badge)](https://www.codefactor.io/repository/github/ayles/a2d)

# Getting started (Build & Run)

For simple demonstration clone this repo and use `tools/generator.py` to
generate project:

    python3 generator.py <project name> [-d destination]

Entry point of game is `a2d::RootComponent` component located in
`root_component.hpp`.

It will be created on the `a2d::Engine::root` after game launch.

Then you can build one of projects located in `<project folder>/projects/`.

# Supported platforms

- **Windows** (MSVC)
- **Linux**
- **macOS**
- **Android** 
- **iOS**

# Sample

For starting your own project create `a2d::RootComponent` class
and store it in the `root_component.hpp`.

**Sample code:**

```cpp
// root_component.hpp

namespace a2d {

class RootComponent : public Component {
public:
    pObject2D text;

    // Initialize called on component creation
    void Initialize() override {
        auto camera = Object2D::Create();
        Engine::SetCamera(camera->AddComponent<PixelCamera>());

        text = Object2D::Create();
        text->AddComponent<Text>()->SetFont(BitmapFont::Create(FileSystem::LoadRaw("fonts/impact.ttf"), 48));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.0f);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }

    // Update called each frame
    void Update() override {
        text->SetRotation(text->GetRotation() + Engine::GetDeltaTime());
    }
};

}
```

# Future plans

- Rework audio
- Add support for texture atlas creation on-the-fly
- And more...

# Contact & Contribute

This project is small, so all contributions are welcome.

You can contact with me by email <selya.main@gmail.com>.