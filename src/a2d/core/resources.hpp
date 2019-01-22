//
// Created by selya on 08.01.2019.
//

#ifndef A2D_RESOURCES_H
#define A2D_RESOURCES_H

#include <a2d/core/macro.hpp>
#include <a2d/core/engine.hpp>

#include <string>
#include <map>
#include <unordered_map>
#include <typeindex>

namespace a2d {

class Resources {

public:
    template<class T>
    static intrusive_ptr<T> Get(const std::string &name, bool try_load = true) {
        ASSERT_MAIN_THREAD
        auto iter = GetStorage<T>().find(name);
        if (iter != GetStorage<T>().end()) {
            return iter->second;
        }
        if (try_load) return LoadAndCache<T>(name);
        return nullptr;
    }

    template<class T>
    static intrusive_ptr<T> LoadAndCache(const std::string &name) {
        ASSERT_MAIN_THREAD
        auto resource = Load<T>(name);
        Put(name, resource);
        return resource;
    }

    template<class T>
    static intrusive_ptr<T> Put(const std::string &name, const intrusive_ptr<T> &resource) {
        ASSERT_MAIN_THREAD
        if (resource) GetStorage<T>()[name] = resource;
        return resource;
    }

    template<class T>
    static intrusive_ptr<T> Load(const std::string &name);

    template<class T>
    static std::map<std::string, intrusive_ptr<T>> &GetStorage() {
        ASSERT_MAIN_THREAD
        static std::map<std::string, intrusive_ptr<T>> storage;
        return storage;
    }
};

} //namespace a2d

#endif //A2D_RESOURCES_H
