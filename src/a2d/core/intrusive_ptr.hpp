//
// Created by selya on 26.10.2018.
//

#ifndef A2D_INTRUSIVE_PTR_H
#define A2D_INTRUSIVE_PTR_H

#include <a2d/core/ref_counter.hpp>

namespace a2d {

template<class T>
class intrusive_ptr {
    union {
        T *ptr;
        ref_counter *r_ptr;
    };

public:
    typedef T element_type;

    intrusive_ptr() : ptr(nullptr) {}
    intrusive_ptr(const intrusive_ptr &other) : ptr(other.ptr) {
        if (ptr) r_ptr->add_ref();
    }

    intrusive_ptr(T *p) : ptr(p) {
        if (ptr) r_ptr->add_ref();
    }

    template<class U>
    intrusive_ptr(const intrusive_ptr<U> &other) : ptr(dynamic_cast<T *>(other.get())) {
        if (ptr) r_ptr->add_ref();
    }

    template<class U>
    intrusive_ptr(U *u) : ptr(dynamic_cast<T *>(u)) {
        if (ptr) r_ptr->add_ref();
    }

    T *get() const {
        return ptr;
    }

    void swap(intrusive_ptr &other) {
        T *p = other.ptr;
        other.ptr = ptr;
        ptr = p;
    }

    T &operator*() const {
        return *ptr;
    }

    T *operator->() const {
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
        return ptr == other.ptr;
    }

    bool operator!=(T *p) const {
        return ptr != p;
    }

    bool operator!=(const intrusive_ptr<T> &other) const {
        return ptr != other.ptr;
    }

    bool operator<(const intrusive_ptr<T> &other) const {
        return ptr < other.ptr;
    }

    bool operator>(const intrusive_ptr<T> &other) const {
        return ptr > other.ptr;
    }

    bool operator!() const {
        return ptr == 0;
    }

    operator bool() const {
        return ptr != 0;
    }

    ~intrusive_ptr() {
        if (ptr) r_ptr->release_ref();
    }
};

} //namespace a2d

#endif //A2D_INTRUSIVE_PTR_H
