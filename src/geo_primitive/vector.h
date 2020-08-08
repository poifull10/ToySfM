#pragma once
#include <cmath>
#include <functional>
#include <initializer_list>
#include <vector>

namespace tsfm
{
template <typename T, int N>
class Vector
{
public:
  Vector() : data_() {}
  Vector(const std::array<T, N> data) : data_(data) {}
  Vector(std::initializer_list<T> init)
  {
    int cnt = 0;
    for (auto e : init)
    {
      data_[cnt++] = e;
    }
  }
  ~Vector() = default;
  Vector(Vector&&) = default;
  Vector(const Vector&) = default;
  Vector& operator=(Vector&) = default;
  T operator[](int i) const { return data_[i]; }

private:
  std::array<T, N> data_;
};

template <typename T, int N>
Vector<T, N> apply(const Vector<T, N>& vec, std::function<T(T)> func)
{
  std::array<T, N> data;
  for (int i = 0; i < N; i++)
  {
    data[i] = func(vec[i]);
  }

  return Vector<T, N>(data);
};

template <typename T, int N>
T sum(const Vector<T, N>& vec)
{
  T ret = 0;
  for (int i = 0; i < N; i++)
  {
    ret += vec[i];
  }
  return ret;
}

template <typename T, int N>
double norm(const Vector<T, N>& v)
{
  const auto v_squared =
    apply(v, std::function<double(double)>([](double e) { return e * e; }));
  const auto v_sum = sum(v_squared);
  return std::sqrt(v_sum);
};

template <typename T, int N>
Vector<T, N> operator*(const Vector<T, N>& v, T val)
{
  std::array<T, N> data;
  for (int i = 0; i < N; i++)
  {
    data[i] = v[i] * val;
  }
  return Vector<T, N>(data);
}

template <typename T, int N>
Vector<T, N> operator/(const Vector<T, N>& v, T val)
{
  std::array<T, N> data;
  for (int i = 0; i < N; i++)
  {
    data[i] = v[i] / val;
  }
  return Vector<T, N>(data);
}

using Vec2 = Vector<double, 2>;
using Vec3 = Vector<double, 3>;
using Vec4 = Vector<double, 4>;
} // namespace tsfm