#pragma once

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>

#include <ruff/core/random.hpp>

namespace ruff::core
{
    template<typename T>
    class Tensor
    {
    private:
        size_t m_rows;
        size_t m_cols;
        std::vector<T> m_data;
    public:
        Tensor(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_data(rows * cols, 0) {};
		    Tensor(std::initializer_list<T> data) : m_rows(data.size()), m_cols(1), m_data(data) {};
        Tensor(std::vector<T> input) : m_rows(input.size()), m_cols(1), m_data(input)
        {
        };
				void Fill(T val);
        void Randomize(T min, T max);

	  public:
		  	Tensor<T> Transpose() const;
    public:
        [[nodiscard]] constexpr inline size_t rows() const { return m_rows; };
        [[nodiscard]] constexpr inline size_t cols() const { return m_cols; };

        T& operator()(int i1, int i2) { return m_data[i1 * m_cols + i2]; };
        T operator()(int i1, int i2) const { return m_data.at(i1 * m_cols + i2); };

        T& operator[](int idx) { return m_data[idx]; }
        T operator[](int idx) const { return m_data[idx]; }

        typename std::vector<T>::iterator begin() { return m_data.begin(); };
        typename std::vector<T>::iterator end() { return m_data.end(); };

        const std::vector<T>& GetData() const { return m_data; };
        size_t Size() const { return cols() * rows(); };
        void Resize(size_t rows, size_t cols);

        Tensor<T> operator*(const Tensor<T>& other) const;
				Tensor<T> operator*(T other) const;

				Tensor<T> operator+(const Tensor<T>& other) const;
				Tensor<T>& operator+=(const Tensor<T>& other);

				Tensor<T> operator-(const Tensor<T>& other) const;
				Tensor<T>& operator-=(const Tensor<T>& other);
    };
};
#include <ruff/core/structs/tensor.tpp>