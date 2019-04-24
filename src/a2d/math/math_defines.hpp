//
// Created by selya on 27.12.2018.
//

#ifndef A2D_MATH_DEFINES_HPP
#define A2D_MATH_DEFINES_HPP

#include <type_traits>
#include <cmath>

#define A2D_MATH_ENABLE_IF_FLOATING_POINT \
template<class U = T, class = typename std::enable_if<std::is_floating_point<U>::value>::type>

#define A2D_MATH_INLINE inline

#define A2D_MATH_SIN(t)             std::sin(t)
#define A2D_MATH_COS(t)             std::cos(t)
#define A2D_MATH_SQRT(t)            std::sqrt(t)
#define A2D_MATH_ROUND(t)           std::round(t)
#define A2D_MATH_CEIL(t)            std::ceil(t)
#define A2D_MATH_FLOOR(t)           std::floor(t)
#define A2D_MATH_ATAN2(y, x)        std::atan2(y, x)
#define A2D_MATH_ACOS(t)            std::acos(t)

#endif //A2D_MATH_DEFINES_HPP
