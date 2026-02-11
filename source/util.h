#pragma once
#include <stdint.h>

namespace vargason::bigsequencer {
    template <typename T>
    T lerp(T min, T max, double value) {
        double temp = min + (max - min) * value;
        if (temp > 0) {
            temp += 0.5;
        }
        else {
            temp -= 0.5;
        }
        return (T)temp;
    }

    template <typename V>
    double ilerp(V min, V max, V value) {
        return (double)(value - min) / (max - min);
    }
}