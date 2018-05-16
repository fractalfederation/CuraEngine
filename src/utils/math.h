/** Copyright (C) 2016 Ultimaker - Released under terms of the AGPLv3 License */
#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#include <cmath>

namespace cura
{


static const float sqrt2 = std::sqrt(2.0f);

template<typename T> inline T square(const T& a) { return a * a; }

inline unsigned int round_divide(unsigned int dividend, unsigned int divisor) //!< Return dividend divided by divisor rounded to the nearest integer
{
    return (dividend + divisor / 2) / divisor;
}
inline unsigned int round_up_divide(unsigned int dividend, unsigned int divisor) //!< Return dividend divided by divisor rounded to the nearest integer
{
    return (dividend + divisor - 1) / divisor;
}

}//namespace cura
#endif // UTILS_MATH_H

