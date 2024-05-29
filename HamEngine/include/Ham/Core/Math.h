#pragma once

#include <Mathter/Vector.hpp>
#include <Mathter/Matrix.hpp>
#include <Mathter/Quaternion.hpp>
#include <Mathter/Utility.hpp>
#include <Mathter/Geometry.hpp>
#include <Mathter/IoStream.hpp>

#define Z_UP

// default up axis is Z
#if !defined(Z_UP) && !defined(Y_UP)
#define Z_UP
#endif

#if defined(Z_UP) && defined(Y_UP)
#error "Only one of Z_UP/Y_UP can be defined!"
#endif

namespace Ham::math::Internal {
class ZeroBuilder : public mathter::ZeroBuilder {
 public:
  ZeroBuilder() = default;
  ZeroBuilder &operator=(const ZeroBuilder &) = delete;

  template <class T, int Dim, bool Packed>
  operator mathter::Vector<T, Dim, Packed>() const
  {
    mathter::Vector<T, Dim, Packed> v;
    Set(v);
    return v;
  }

 private:
  template <class T, int Dim, bool Packed>
  void Set(mathter::Vector<T, Dim, Packed> &v) const
  {
    Fill(v, T(0));
  }
};
}  // namespace Ham::math::Internal

namespace Ham::math {

template <typename T>
inline static T zero()
{
  return (T)Ham::math::Internal::ZeroBuilder();
}
template <typename T>
inline static T identity()
{
  return (T)mathter::Identity();
}

template <int Dim>
using vec = mathter::Vector<float, Dim, false>;
using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

using bvec2 = mathter::Vector<bool, 2, false>;
using bvec3 = mathter::Vector<bool, 3, false>;
using bvec4 = mathter::Vector<bool, 4, false>;

using ivec2 = mathter::Vector<int, 2, false>;
using ivec3 = mathter::Vector<int, 3, false>;
using ivec4 = mathter::Vector<int, 4, false>;

using uvec2 = mathter::Vector<uint32_t, 2, false>;
using uvec3 = mathter::Vector<uint32_t, 3, false>;
using uvec4 = mathter::Vector<uint32_t, 4, false>;

template <int W, int H>
using mat = mathter::Matrix<float, W, H, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
using mat2 = mat<2, 2>;
using mat3 = mat<3, 3>;
using mat4 = mat<4, 4>;

using quat = mathter::Quaternion<float, false>;

template <typename T>
T clamp(T value, T min, T max)
{
  return mathter::Clamp(value, min, max);
}

template <class T, int Dim, bool Packed>
T clamp(mathter::Vector<float, Dim, Packed> value, T min, T max)
{
  return mathter::Clamp(value, min, max);
}

inline static mat4 perspective(float fov, vec2 aspect, float near, float far)
{
  float const tanHalfFovy = tan(fov / 2.0f);

  mat4 Result = zero<mat4>();
  Result(0, 0) = 1.0f / ((aspect.x / aspect.y) * tanHalfFovy);
  Result(1, 1) = 1.0f / (tanHalfFovy);
  Result(2, 2) = -(far + near) / (far - near);
  Result(3, 2) = -1.0f;
  Result(2, 3) = -(2.0f * far * near) / (far - near);
  return Result;

  // return mathter::Perspective(fov, aspect.x / aspect.y, near, far, -1.0f, 1.0f);
  // return (mat4)mathter::PerspectiveBuilder<float, 3, false>(fov, aspect, near, far);
}

template <class Scalar>
auto degrees(Scalar rad)
{
  return mathter::Rad2Deg(rad);
}

template <class Scalar>
auto radians(Scalar deg)
{
  return mathter::Deg2Rad(deg);
}

template <typename M>
auto transpose(const M &m)
{
  return mathter::Transpose(m);
}

template <typename M>
auto inverse(const M &m)
{
  return mathter::Inverse(m);
}

template <typename V>
auto normalize(const V &v)
{
  return mathter::Normalize(v);
}

template <typename V>
auto length(const V &v)
{
  return mathter::Length(v);
}

template <typename V>
auto dot(const V &a, const V &b)
{
  return mathter::Dot(a, b);
}

template <typename... Args>
inline auto cross(Args &&...args) -> decltype(mathter::Cross(std::forward<Args>(args)...))
{
  return mathter::Cross(std::forward<Args>(args)...);
}

#if defined(Z_UP)
inline static vec3 up() { return vec3(0, 0, 1); }
inline static vec3 down() { return -up(); }
inline static vec3 right() { return vec3(1, 0, 0); }
inline static vec3 left() { return -right(); }
inline static vec3 forward() { return vec3(0, 1, 0); }
inline static vec3 backward() { return -forward(); }
#elif defined(Y_UP)
inline static vec3 up() { return vec3(0, 1, 0); }
inline static vec3 down() { return -up(); }
inline static vec3 right() { return vec3(1, 0, 0); }
inline static vec3 left() { return -right(); }
inline static vec3 forward() { return vec3(0, 0, -1); }
inline static vec3 backward() { return -forward(); }
#endif

inline static mat4 lookAt(const vec3 &eye, const vec3 &target, const vec3 &up) { return (mat4)mathter::ViewBuilder<float, 3, false>(eye, target, {up}, {false, false, false}); }
inline static mat4 lookAt(const vec3 &eye, const vec3 &target) { return lookAt(eye, target, up()); }

inline static mat4 camera() { return lookAt(zero<vec3>(), forward(), up()); }

template <class T>
inline constexpr T pi = T(3.141592653589793238462643383279502884197169399375105820974944592307816406286);

inline static mat4 translate(const vec3 &v) { return (mat4)mathter::TranslationBuilder<float, 3, false>(v); }
inline static mat4 scale(const vec3 &v) { return (mat4)mathter::ScaleBuilder<float, 3, false>(v); }

inline static mat4 rotateEuler(const vec3 &angles) { return (mat4)mathter::RotationRPY(angles.x, angles.y, angles.z); }
inline static mat4 rotateAxisAngle(const vec3 &axis, float angle) { return (mat4)mathter::Rotation3DAxisAngleBuilder<float, false>(axis, angle); }

inline static void decompose(const mat4 &t, vec3 &outTranslation, mat3 &outRotation, vec3 &outScale)
{
  outTranslation.x = t(0, 3);
  outTranslation.y = t(1, 3);
  outTranslation.z = t(2, 3);

  outScale.x = length(vec3(t(0, 0), t(1, 0), t(2, 0)));
  outScale.y = length(vec3(t(0, 1), t(1, 1), t(2, 1)));
  outScale.z = length(vec3(t(0, 2), t(1, 2), t(2, 2)));

  vec3 row0 = normalize(vec3(t(0, 0), t(0, 1), t(0, 2))) / outScale.x;
  vec3 row1 = normalize(vec3(t(1, 0), t(1, 1), t(1, 2))) / outScale.y;
  vec3 row2 = normalize(vec3(t(2, 0), t(2, 1), t(2, 2))) / outScale.z;

  outRotation = mat3(
      row0.x, row0.y, row0.z,
      row1.x, row1.y, row1.z,
      row2.x, row2.y, row2.z);
}

inline static void decompose(const mat4 &t, vec3 &outTranslation, quat &outRotation, vec3 &outScale)
{
  mat3 rotation;
  decompose(t, outTranslation, rotation, outScale);
  outRotation = quat(rotation);
}

inline static vec3 euler(const quat &q)
{
  vec3 angles;

  float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
  float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
  angles.x = std::atan2(sinr_cosp, cosr_cosp);

  float sinp = 2 * (q.w * q.y - q.z * q.x);
  if (std::abs(sinp) >= 1.0f)
    angles.y = std::copysign(pi<float> / 2.0f, sinp);  // use 90 degrees if out of range
  else
    angles.y = std::asin(sinp);

  float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
  float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
  angles.z = std::atan2(siny_cosp, cosy_cosp);

  return angles;
}

template <typename T, int Dim, bool Packed>
inline static std::string to_string(const mathter::Vector<T, Dim, Packed> &vector)
{
  std::stringstream ss;
  ss << "(";
  for (int i = 0; i < Dim; i++) {
    ss << vector[i];
    if (i != Dim - 1)
      ss << ", ";
  }
  ss << ")";
  return ss.str();
}

template <typename T, int W, int H, bool Packed>
inline static std::string to_string(const mathter::Matrix<T, W, H, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, Packed> &matrix)
{
  std::stringstream ss;
  ss << "[";
  for (int i = 0; i < W; i++) {
    ss << to_string(matrix.GetColumn(i));
    if (i != W - 1)
      ss << ", ";
  }
  ss << "]";
  return ss.str();
}

inline static std::string to_string(const quat &quaternion)
{
  std::stringstream ss;
  ss << "(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
  return ss.str();
}
}  // namespace Ham::math