#pragma once
#include <concepts>
#include <limits>
#include <random>

namespace ruff::security {
enum class GENERATOR_TYPE
{
    SIMPLE,
    FILE
};

template<typename T> requires std::is_integral<T>::value
T Prime(T min=0, T max=std::numeric_limits<T>::max())
{
    T test;
    bool isPrime = true;
    do
    {
        test = std::rand()%max + min;
        if(test < 2) isPrime = false;
        if(test == 2) return test;
        if(test % 2 == 0) isPrime = false;
        for(int i = 3; (i*i)<=test; i+=2) isPrime = false;
    }while(!isPrime);
    return test;
}


};