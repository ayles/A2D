//
// Created by selya on 26.10.2018.
//

#ifndef A2D_COMPONENT_H
#define A2D_COMPONENT_H

#include <a2d/core/macro.h>
#include <a2d/core/ref_counter.h>

namespace a2d {

class Object2D;

DECLARE_SMART_POINTER(Component)

class Component : public ref_counter {
public:
    friend Object2D;

    virtual SMART_POINTER(Object2D) GetObject2D();

    virtual bool IsActive();
    virtual void SetActive(bool active);

protected:
    bool is_active;
    SMART_POINTER(Object2D) object_2d;

    virtual void Initialize() {};
    virtual void OnEnable() {};
    virtual void Start() {};
    virtual void LayerHierarchyAboutToChange() {};
    virtual void LayerHierarchyChanged() {};
    virtual void Update() {};
    virtual void PostUpdate() {};
    virtual void PreDraw() {};
    virtual void PostDraw() {};
    virtual void OnQuit() {};
    virtual void OnDisable() {};
    virtual void OnDestroy() {};

    ~Component() override {};
    Component();
};

} // namespace a2d

#endif //A2D_COMPONENT_H
