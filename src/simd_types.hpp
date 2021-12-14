#ifndef SIMD_TYPES_HPP_2021_ADA
#define SIMD_TYPES_HPP_2021_ADA

// Really shitty SIMD lib that only supports 32 bit floats on x64.

#include "common.hpp"
#include <bit>
#include <xmmintrin.h>

// This needs some memcpy bullshit to be defined behavior so... whatever
F32 BoolToF32Mask(bool a)
{
  static_assert(sizeof(I32) == sizeof(F32));
  I32 x = a ? 0xFFFFFFFF : 0;
  F32 y;
  memcpy(&y, &x, sizeof(y));
  return y;
}

class F32_4xMask
{
  friend class F32_4x;

  private:
  __m128 inner;
  // At least on x86, this should only be called with 0 or 0xFFFFFFFFF in all
  // the lanes that are cared about since no distinction between
  F32_4xMask(__m128 i): inner(i) {};

  public:
  F32_4xMask(bool a) noexcept: inner(_mm_set1_ps(BoolToF32Mask(a)))
  {
  }

  F32_4xMask(bool a, bool b, bool c, bool d):
      inner(_mm_setr_ps(BoolToF32Mask(a),
                        BoolToF32Mask(b),
                        BoolToF32Mask(c),
                        BoolToF32Mask(d)))
  {
  }

  bool all() const noexcept
  {
    for(auto f: inner.m128_i32)
    {
      if(f == 0)
      {
        return false;
      }
    }
    return true;
  }

  bool any() const noexcept
  {
    for(auto f: inner.m128_i32)
    {
      if(f != 0)
      {
        return true;
      }
    }
    return false;
  }

  bool none() const noexcept
  {
    for(auto f: inner.m128_i32)
    {
      if(f != 0)
      {
        return false;
      }
    }
    return true;
  }

  F32_4xMask operator!() const noexcept
  {
    return F32_4xMask(_mm_andnot_ps(inner, _mm_set1_ps(1)));
  }
  F32_4xMask operator&(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_and_ps(inner, m.inner));
  }
  F32_4xMask operator&&(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_and_ps(inner, m.inner));
  }
  F32_4xMask operator|(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_or_ps(inner, m.inner));
  }
  F32_4xMask operator||(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_or_ps(inner, m.inner));
  }
  F32_4xMask operator^(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_xor_ps(inner, m.inner));
  }

  F32_4xMask operator&(bool m) const noexcept
  {
    return F32_4xMask(_mm_and_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }
  F32_4xMask operator&&(bool m) const noexcept
  {
    return F32_4xMask(_mm_and_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }
  F32_4xMask operator|(bool m) const noexcept
  {
    return F32_4xMask(_mm_or_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }
  F32_4xMask operator||(bool m) const noexcept
  {
    return F32_4xMask(_mm_or_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }
  F32_4xMask operator^(bool m) const noexcept
  {
    return F32_4xMask(_mm_xor_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }

  F32_4xMask &operator&=(F32_4xMask m) noexcept
  {
    *this = *this & m;
    return *this;
  }
  F32_4xMask &operator|=(F32_4xMask m) noexcept
  {
    *this = *this | m;
    return *this;
  }

  F32_4xMask &operator&=(bool m) noexcept
  {
    *this = *this & m;
    return *this;
  }
  F32_4xMask &operator|=(bool m) noexcept
  {
    *this = *this | m;
    return *this;
  }

  F32_4xMask andNot(F32_4xMask m) const noexcept
  {
    return F32_4xMask(_mm_andnot_ps(inner, m.inner));
  }
  F32_4xMask andNot(bool m) const noexcept
  {
    return F32_4xMask(_mm_andnot_ps(inner, _mm_set1_ps(BoolToF32Mask(m))));
  }

  // We could wrap this in a wrapper type to allow setting through this but...
  // pain
  template<Integral T>
  bool operator[](T index) const noexcept
  {
    return inner.m128_f32[index] != 0;
  }

  void set(size_t index, bool value) noexcept
  {
    inner.m128_f32[index] = BoolToF32Mask(value);
  }
};

class F32_4x
{
  private:
  __m128 inner;
  F32_4x(__m128 m) noexcept: inner(m)
  {
  }

  public:
  F32_4x(F32 a) noexcept: inner(_mm_set1_ps(a))
  {
  }
  F32_4x(F32 a, F32 b, F32 c, F32 d) noexcept: inner(_mm_setr_ps(a, b, c, d))
  {
  }
  F32_4x() = default;

  F32_4x operator+(F32_4x l) const noexcept
  {
    return F32_4x(_mm_add_ps(inner, l.inner));
  }

  F32_4x operator+(F32 f) const noexcept
  {
    return F32_4x(_mm_add_ps(inner, _mm_set1_ps(f)));
  }

  F32_4x &operator+=(F32_4x l) noexcept
  {
    *this = *this + l;
    return *this;
  }

  F32_4x &operator+=(F32 f) noexcept
  {
    *this = *this + f;
    return *this;
  }

  F32_4x operator-(F32_4x l) const noexcept
  {
    return F32_4x(_mm_sub_ps(inner, l.inner));
  }

  F32_4x operator-(F32 f) const noexcept
  {
    return F32_4x(_mm_sub_ps(inner, _mm_set1_ps(f)));
  }

  F32_4x &operator-=(F32_4x l) noexcept
  {
    *this = *this - l;
    return *this;
  }

  F32_4x &operator-=(F32 f) noexcept
  {
    *this = *this - f;
    return *this;
  }

  F32_4x operator/(F32_4x l) const noexcept
  {
    return F32_4x(_mm_div_ps(inner, l.inner));
  }

  F32_4x operator/(F32 f) const noexcept
  {
    return F32_4x(_mm_div_ps(inner, _mm_set1_ps(f)));
  }

  F32_4x &operator/=(F32_4x l) noexcept
  {
    *this = *this / l;
    return *this;
  }

  F32_4x &operator/=(F32 f) noexcept
  {
    *this = *this / f;
    return *this;
  }

  F32_4x operator*(F32_4x l) const noexcept
  {
    return F32_4x(_mm_mul_ps(inner, l.inner));
  }

  F32_4x operator*(F32 f) const noexcept
  {
    return F32_4x(_mm_mul_ps(inner, _mm_set1_ps(f)));
  }

  F32_4x &operator*=(F32_4x l) noexcept
  {
    *this = *this * l;
    return *this;
  }

  F32_4x &operator*=(F32 f) noexcept
  {
    *this = *this * f;
    return *this;
  }

  F32_4xMask operator>(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmpgt_ps(inner, l.inner));
  }

  F32_4xMask operator>=(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmpge_ps(inner, l.inner));
  }

  F32_4xMask operator<=(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmple_ps(inner, l.inner));
  }

  F32_4xMask operator<(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmplt_ps(inner, l.inner));
  }

  F32_4xMask operator==(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmpeq_ps(inner, l.inner));
  }

  F32_4xMask operator!=(F32_4x l) const noexcept
  {
    return F32_4xMask(_mm_cmpneq_ps(inner, l.inner));
  }

  F32_4xMask operator>(F32 l) const noexcept
  {
    return F32_4xMask(_mm_cmpgt_ps(inner, _mm_set1_ps(l)));
  }

  F32_4xMask operator>=(F32 l) const noexcept
  {
    auto lExpanded = _mm_set1_ps(l);
    return F32_4xMask(_mm_cmpge_ps(inner, lExpanded));
  }

  F32_4xMask operator<=(F32 l) const noexcept
  {
    return F32_4xMask(_mm_cmple_ps(inner, _mm_set1_ps(l)));
  }

  F32_4xMask operator<(F32 l) const noexcept
  {
    return F32_4xMask(_mm_cmplt_ps(inner, _mm_set1_ps(l)));
  }

  F32_4xMask operator==(F32 l) const noexcept
  {
    return F32_4xMask(_mm_cmpeq_ps(inner, _mm_set1_ps(l)));
  }

  F32_4xMask operator!=(F32 l) const noexcept
  {
    return F32_4xMask(_mm_cmpneq_ps(inner, _mm_set1_ps(l)));
  }

  // if the mask for a lane i is set, a[i] will be in ret[i], else b[i] will be
  // in ret[i]
  // TODO: is there a better way to do this?
  static F32_4x select(F32_4xMask shuf_mask, F32_4x a, F32_4x b)
  {
    auto a_masked = _mm_and_ps(shuf_mask.inner, a.inner);
    auto b_masked = _mm_andnot_ps(shuf_mask.inner, b.inner);
    return F32_4x(_mm_or_ps(a_masked, b_masked));
  }

  template<Integral T>
  F32 &operator[](T i) noexcept
  {
    return inner.m128_f32[i];
  }

  template<Integral T>
  F32 &operator[](T i) const noexcept
  {
    return inner.m128_f32[i];
  }

  const F32 *begin() const
  {
    return &inner.m128_f32[0];
  }
  const F32 *end() const
  {
    return &inner.m128_f32[4];
  }

  F32 *begin()
  {
    return &inner.m128_f32[0];
  }
  F32 *end()
  {
    return &inner.m128_f32[4];
  }

  const F32 *cbegin() const
  {
    return &inner.m128_f32[0];
  }
  const F32 *cend() const
  {
    return &inner.m128_f32[4];
  }
};

#endif
