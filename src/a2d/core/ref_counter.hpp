//
// Created by selya on 26.10.2018.
//

#ifndef A2D_REF_COUNTER_HPP
#define A2D_REF_COUNTER_HPP

namespace a2d {

class ref_counter {
    template<class T>
    friend class intrusive_ptr;

public:
    ref_counter(const ref_counter &) = delete;
    const ref_counter &operator=(const ref_counter &) = delete;

protected:
    ref_counter() : _ref_counter(0) {}
    virtual ~ref_counter() = default;

    int _ref_counter;

    void add_ref() {
        ++_ref_counter;
    }

    void release_ref() {
        if (0 == --_ref_counter)
            delete this;
    }
};

} //namespace a2d

#endif //A2D_REF_COUNTER_HPP
