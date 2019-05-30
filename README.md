# A2D

Small cross-platform 2D game engine.

[![Build status](https://ci.appveyor.com/api/projects/status/kuhmfco62gwb6kco?svg=true)](https://ci.appveyor.com/project/ayles/a2d)
[![Build Status](https://travis-ci.com/ayles/A2D.svg?branch=master)](https://travis-ci.com/ayles/A2D)
[![CodeFactor](https://www.codefactor.io/repository/github/ayles/a2d/badge)](https://www.codefactor.io/repository/github/ayles/a2d)

# Getting started (Build & Run)

To start simple project follow these steps:
 
1. Clone repo
 
       git clone https://github.com/ayles/A2D.git
        
2. Use `generate.py` to generate project files:

       python3 A2D/tools/generate.py MySimpleProject -d .
        
3. Optionally edit `MySimpleProject/src/root_component.hpp`

4. Build desktop project:

       cd MySimpleProject/projects/proj.desktop
       mkdir build
       cd build
       cmake ..
       cmake --build .
       
5. Run:

       ./MySimpleProject
 
# Supported platforms

- **Windows** (MSVC)
- **Linux**
- **macOS**
- **Android** 
- **iOS**

# Sample

```cpp
// root_component.hpp

namespace a2d {

class RootComponent : public Component {
public:
    intrusive_ptr<Object2D> text;
    intrusive_ptr<Camera> camera;

    void Initialize() override {
        camera = Object2D::Create()->AddComponent<Camera>();
        Renderer::SetMainCamera(camera);

        text = Object2D::Create();
        text->AddComponent<Text>()->SetFont(Resources::Get<BitmapFont>("impact"));
        text->GetComponent<Text>()->SetOrigin(0.5f, 0.0f);
        text->GetComponent<Text>()->SetFontSize(24);
        text->GetComponent<Text>()->SetText(U"Hello, A2D!");
    }

    void Update() override {
        text->SetRotation(text->GetRotation() + Engine::GetDeltaTime());
        camera->SetHeight(Renderer::GetHeight());
    }
};

}
```

# Future plans

- Rework audio
- Add support for texture atlas creation on-the-fly
- Add Emscripten support
- And more...

# Contact & Contribute

This project is small, so all contributions are welcome.

You can contact with me by email <selya.main@gmail.com>.