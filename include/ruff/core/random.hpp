#pragma once
#include <random>

namespace ruff::core
{
    static std::random_device random_device;
    static std::mt19937 rng_alg(random_device());

    template<typename T>
    T Rand(T min, T max)
    {
        using dist_t = std::conditional_t<
                std::is_integral_v<T>,
                std::conditional_t<
                    std::is_signed_v<T>,
                    std::uniform_int_distribution<long long>,
                    std::uniform_int_distribution<unsigned long long>>,
                std::uniform_real_distribution<T>>;

        dist_t dist(min, max);
        return static_cast<T>(dist(rng_alg));
    }

};