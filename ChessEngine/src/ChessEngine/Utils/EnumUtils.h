#pragma once

#include <type_traits>

template<typename T>
using Underlying = std::underlying_type<T>::type;

template<typename T>
constexpr Underlying<T> underlying(T t)
{
	return Underlying<T>(t);
}

template<typename T>
struct TruthValue
{
	T Value;

	constexpr TruthValue(T value) : Value(value) {}
	constexpr operator T() const { return Value; }
	constexpr explicit operator bool() const { return underlying(Value); }
};

template<typename T>
constexpr TruthValue<T> operator&(T left, T right)
{
	return T(underlying(left) & underlying(right));
}

template<typename T>
constexpr T operator|(T left, T right)
{
	return T(underlying(left) | underlying(right));
}

template<typename T>
constexpr T operator^(T left, T right)
{
	return T(underlying(left) ^ underlying(right));
}

template<typename T>
constexpr T operator~(T value)
{
	return T(~underlying(value));
}

template<typename T>
constexpr T operator&=(T left, T right)
{
	left = left & right;
	return left;
}

template<typename T>
constexpr T operator|=(T& left, T right)
{
	left = left | right;
	return left;
}

template<typename T>
constexpr T operator^=(T& left, T right)
{
	left = left ^ right;
	return left;
}
