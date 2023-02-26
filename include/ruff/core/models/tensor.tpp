#pragma once
//#define RUFF_MULTITHREADED _Pragma("omp parallel for")
#define RUFF_MULTITHREADED

namespace ruff::core
{
template<typename T>
void Tensor<T>::Fill(T val)
{
RUFF_MULTITHREADED
		for(size_t i = 0; i < m_data.size(); ++i)
		{
				(*this)[i] = val;
		}

}
template<typename T>
void Tensor<T>::Randomize(T min, T max)
{
		for(auto it = begin(); it != end(); ++it)
		{
				*it = core::Rand(min, max);
		}
}


template<typename T>
void Tensor<T>::Resize(size_t rows, size_t cols)
{
		m_rows = rows;
		m_cols = cols;
		m_data.resize(rows * cols);
}
template<typename T>
Tensor<T> Tensor<T>::Transpose() const
{
		Tensor<T> output(cols(), rows());
RUFF_MULTITHREADED
		for(size_t i = 0; i < rows(); ++i)
		{
RUFF_MULTITHREADED
				for(size_t j = 0; j < cols(); ++j)
				{
						output(j, i) = (*this)(i, j);
				}
		}
		return output;
}

template<typename T>
T Tensor<T>::Dot(const Tensor<T>& other) const
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "used to find the dot product");

		T sum;
		for(size_t i = 0; i < m_data.size(); ++i)
		{
				sum += (*this)[i] + other[i];
		}
		return sum;
}

template<typename T>
Tensor<T> Tensor<T>::Multiply(const Tensor<T>& other) const
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "elementwise multiplied");
		Tensor<T> out = *this;

RUFF_MULTITHREADED
		for(size_t i = 0; i < m_data.size(); ++i)
		{
				out[i] *= other[i];
		}
		return out;
}

template<typename T>
Tensor<T> Tensor<T>::operator*(const Tensor<T>& other) const
{
		TensorAssert(other, cols() == other.rows(), "multiplied");

		Tensor<T> output(rows(), other.cols());
RUFF_MULTITHREADED
		for(size_t i = 0; i < rows(); ++i)
		{
RUFF_MULTITHREADED
				for(size_t j = 0; j < other.cols(); ++j)
				{
RUFF_MULTITHREADED
						for(size_t k = 0; k < cols(); ++k)
						{
								output(i, j) += (*this)(i, k) * other(k, j);
						}
				}
		}
		return output;
}
template<typename T>
Tensor<T> Tensor<T>::operator*(T other) const
{
		Tensor<T> output = *this;
		for(auto it = output.begin(); it != output.end(); ++it)
		{
				*it *= other;
		}
		return output;
}
template<typename T>
Tensor<T> Tensor<T>::operator+(const Tensor<T>& other) const
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "added");

		Tensor<T> output = *this;
RUFF_MULTITHREADED
		for(size_t i = 0; i < m_rows*m_cols; ++i)
		{
				output[i] += other[i];
		}
		return output;
}
template<typename T>
Tensor<T>& Tensor<T>::operator+=(const Tensor<T>& other)
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "added");
RUFF_MULTITHREADED
		for(size_t i = 0; i < m_rows*m_cols; ++i)
		{
				(*this)[i] += other[i];
		}
		return *this;
}

template<typename T>
Tensor<T> Tensor<T>::operator-(const Tensor<T>& other) const
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "subtracted");

		Tensor<T> output = *this;
RUFF_MULTITHREADED
		for(size_t i = 0; i < m_rows*m_cols; ++i)
		{
				output[i] -= other[i];
		}
		return output;
}
template<typename T>
Tensor<T>& Tensor<T>::operator-=(const Tensor<T>& other)
{
		TensorAssert(other, rows() == other.rows() && cols() == other.cols(), "subtracted");
RUFF_MULTITHREADED
		for(size_t i = 0; i < m_rows*m_cols; ++i)
		{
				(*this)[i] -= other[i];
		}
		return *this;
}

template<typename T>
std::string Tensor<T>::ToString() const
{
		std::string msg = "[";
		for(size_t i = 0; i < rows(); ++i)
		{
				for(size_t j = 0; j < cols(); ++j)
				{
						 msg += std::to_string((*this)(i, j));
						 if(j != cols() - 1)
								 msg += ", ";
				}
				if(i != rows() - 1)
						msg += "\n ";
		}
		msg += "]";
		return msg;
}

template<typename T>
Json::Value Tensor<T>::ToJSON() const
{
		Json::Value root;
		root["rows"] = rows();
		root["cols"] = cols();
		Json::Value data;
		for(size_t i = 0; i < Size(); ++i)
		{
				data.append(m_data[i]);
		}
		root["data"] = data;
		return root;
}
};
