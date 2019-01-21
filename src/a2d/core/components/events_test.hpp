//
// Created by selya on 21.11.2018.
//

#ifndef A2D_EVENTS_TEST_H
#define A2D_EVENTS_TEST_H

#include <a2d/core/component.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {

class EventsTest : public Component {
    void Initialize() override  { Engine::GetLogger()->info("Initialize");  };
    void Update() override      { Engine::GetLogger()->info("Update");      };
    void PostUpdate() override  { Engine::GetLogger()->info("PostUpdate");  };
    void OnDestroy() override   { Engine::GetLogger()->info("OnDestroy");   };
    void OnPause() override     { Engine::GetLogger()->info("OnPause");     };
    void OnResume() override    { Engine::GetLogger()->info("OnResume");    };
};

} //namespace a2d

#endif //A2D_EVENTS_TEST_H
