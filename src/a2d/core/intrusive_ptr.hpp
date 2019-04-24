//
// Created by selya on 26.10.2018.
//

#ifndef A2D_INTRUSIVE_PTR_HPP
#define A2D_INTRUSIVE_PTR_HPP

#include <a2d/core/ref_counter.hpp>
#include <a2d/core/log.hpp>

#include <string>
#include <functional>

namespace a2d {

template<class T>
class intrusive_ptr {
    union {
        T *ptr;
        ref_counter *r_ptr;
    };

public:
    typedef T element_type;

    intrusive_ptr() : r_ptr(nullptr) {}
    intrusive_ptr(const intrusive_ptr &other) : ptr(other.ptr) {
        if (r_ptr) r_ptr->add_ref();
    }

    intrusive_ptr(T *p) : ptr(p) {
        if (r_ptr) r_ptr->add_ref();
    }

    // TODO add inheritance check
    template<class U>
    intrusive_ptr(const intrusive_ptr<U> &other) : ptr(dynamic_cast<T *>(other.get())) {
        if (r_ptr) r_ptr->add_ref();
    }

    template<class U>
    intrusive_ptr(U *u) : ptr(dynamic_cast<T *>(u)) {
        if (r_ptr) r_ptr->add_ref();
    }

    T *get() const {
        return ptr;
    }

    void swap(intrusive_ptr &other) {
        ref_counter *p = other.r_ptr;
        other.r_ptr = r_ptr;
        r_ptr = p;
    }

    T &operator*() const {
#ifndef NDEBUG
        if (!ptr) LOG_TRACE("Null pointer dereference");
#endif
        return *ptr;
    }

    T *operator->() const {
#ifndef NDEBUG
        if (!ptr) LOG_TRACE("Null pointer dereference");
#endif
        return ptr;
    }

    intrusive_ptr &operator=(const intrusive_ptr &other) {
        intrusive_ptr(other).swap(*this);
        return *this;
    }

    intrusive_ptr &operator=(T *p) {
        intrusive_ptr(p).swap(*this);
        return *this;
    }

    bool operator==(const T *p) const {
        return ptr == p;
    }

    bool operator==(const intrusive_ptr<T> &other) const {
        return r_ptr == other.r_ptr;
    }

    bool operator!=(T *p) const {
        return ptr != p;
    }

    bool operator!=(const intrusive_ptr<T> &other) const {
        return r_ptr != other.r_ptr;
    }

    bool operator<(const intrusive_ptr<T> &other) const {
        return r_ptr < other.r_ptr;
    }

    bool operator>(const intrusive_ptr<T> &other) const {
        return r_ptr > other.r_ptr;
    }

    bool operator!() const {
        return r_ptr == 0;
    }

    operator bool() const {
        return r_ptr != 0;
    }

    ~intrusive_ptr() {
        if (r_ptr) r_ptr->release_ref();
    }
};

} //namespace a2d

namespace std {

template<class K>
struct hash<a2d::intrusive_ptr<K>> {
    size_t operator()(const a2d::intrusive_ptr<K> &ptr) const {
        return std::hash<K *>(ptr.get());
    }
};

}

#endif //A2D_INTRUSIVE_PTR_HPP
