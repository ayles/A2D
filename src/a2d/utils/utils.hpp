//
// Created by selya on 13.02.2019.
//

#ifndef A2D_UTILS_HPP
#define A2D_UTILS_HPP

#include <functional>

namespace a2d {
namespace utils {

#if SIZEOF_VOID_P == 4

template<class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#elif SIZEOF_VOID_P == 8

template<class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    const size_t m = 0xc6a4a7935bd1e995;
    const int r = 47;

    std::hash<T> hasher;

    size_t k = hasher(v);

    k *= m;
    k ^= k >> r;
    k *= m;

    seed ^= k;
    seed *= m;

    seed += 0xe6546b64;
}

#endif

}
}

#endif //A2D_UTILS_HPP
