#pragma once
#include <concepts>
namespace ruff::core
{
    template<typename T>
    concept NumericType = requires(T param)
    {
        requires std::is_integral_v<T> || std::is_floating_point_v<T>;
        requires !std::is_same_v<bool, T>;
        requires std::is_arithmetic_v<decltype(param +1)>;
        requires !std::is_pointer_v<T>;
    };

    template<typename T> requires NumericType<T>
    T MapToRange(T val, T min, T max, T new_min, T new_max)
    {
        return new_min + ((1.0 * new_max - new_min) / (max - min)) * (val - min);
    };

}