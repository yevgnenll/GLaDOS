#ifndef GLADOS_SIMD_H
#define GLADOS_SIMD_H

#include "platform/OSTypes.h"

#if defined(PLATFORM_SIMD_SSE2)
#include <xmmintrin.h>
#elif defined(PLATFORM_SIMD_NEON)
#include <arm_neon.h>
#else
#include "math/Math.h"
#include "math/Vec4.h"
#endif

namespace GLaDOS {
    // TODO
#if defined(PLATFORM_SIMD_SSE2)
    using SIMDVec4 = __m128;

    SIMDVec4 SIMD_load(float x, float y, float z, float w) {
        return _mm_set_ps(x, y, z, w);
    }

    void SIMD_store(void* dest, SIMDVec4 src) {
        _mm_storer_ps(static_cast<float*>(dest), src);
    }

    SIMDVec4 SIMD_add(SIMDVec4 a, SIMDVec4 b) {
        return _mm_add_ps(a, b);
    }

    SIMDVec4 SIMD_sub(SIMDVec4 a, SIMDVec4 b) {
        return _mm_sub_ps(a, b);
    }

    SIMDVec4 SIMD_mul(SIMDVec4 a, SIMDVec4 b) {
        return _mm_mul_ps(a, b);
    }

    SIMDVec4 SIMD_div(SIMDVec4 a, SIMDVec4 b) {
        return _mm_div_ps(a, b);
    }

    // 역수
    SIMDVec4 SIMD_rcp(SIMDVec4 a) {
        return _mm_rcp_ps(a);
    }

    SIMDVec4 SIMD_sqrt(SIMDVec4 a) {
        return _mm_sqrt_ps(a);
    }

    SIMDVec4 SIMD_rsqrt(SIMDVec4 a) {
        return _mm_rsqrt_ps(a);
    }

    SIMDVec4 SIMD_min(SIMDVec4 a, SIMDVec4 b) {
        return _mm_min_ps(a, b);
    }

    SIMDVec4 SIMD_max(SIMDVec4 a, SIMDVec4 b) {
        return _mm_max_ps(a, b);
    }
#elif defined(PLATFORM_SIMD_NEON)
    using SIMDVec4 = float32x4_t;

    SIMDVec4 SIMD_load(float x, float y, float z, float w) {
        float values[4] = {x, y, z, w};
        return vld1q_f32(values);
    }

    void SIMD_store(void* dest, SIMDVec4 src) {
        vst1q_f32(static_cast<float*>(dest), src);
    }

    SIMDVec4 SIMD_add(SIMDVec4 a, SIMDVec4 b) {
        return vaddq_f32(a, b);
    }

    SIMDVec4 SIMD_sub(SIMDVec4 a, SIMDVec4 b) {
        return vsubq_f32(a, b);
    }

    SIMDVec4 SIMD_mul(SIMDVec4 a, SIMDVec4 b) {
        return vmulq_f32(a, b);
    }

    SIMDVec4 SIMD_div(SIMDVec4 a, SIMDVec4 b) {
        return vdivq_f32(a, b);
    }

    // 역수
    SIMDVec4 SIMD_rcp(SIMDVec4 a) {
        return vrecpeq_f32(a);
    }

    SIMDVec4 SIMD_sqrt(SIMDVec4 a) {
        return vsqrtq_f32(a);
    }

    SIMDVec4 SIMD_rsqrt(SIMDVec4 a) {
        return vrsqrteq_f32(a);
    }

    SIMDVec4 SIMD_min(SIMDVec4 a, SIMDVec4 b) {
        return vminq_f32(a, b);
    }

    SIMDVec4 SIMD_max(SIMDVec4 a, SIMDVec4 b) {
        return vmaxq_f32(a, b);
    }
#else
    using SIMDVec4 = Vec4;

    SIMDVec4 SIMD_load(float x, float y, float z, float w) {
        return {x, y, z, w};
    }

    void SIMD_store(void* dest, SIMDVec4 src) {
        std::memcpy(reinterpret_cast<SIMDVec4*>(dest), &src, sizeof(SIMDVec4));
    }

    SIMDVec4 SIMD_add(SIMDVec4 a, SIMDVec4 b) {
        return a + b;
    }

    SIMDVec4 SIMD_sub(SIMDVec4 a, SIMDVec4 b) {
        return a - b;
    }

    SIMDVec4 SIMD_mul(SIMDVec4 a, SIMDVec4 b) {
        return a * b;
    }

    SIMDVec4 SIMD_div(SIMDVec4 a, SIMDVec4 b) {
        return a / b;
    }

    SIMDVec4 SIMD_rcp(SIMDVec4 a) {
        return Vec4::inverse(a);
    }

    SIMDVec4 SIMD_sqrt(SIMDVec4 a) {
        return Vec4::sqrt(a);
    }

    SIMDVec4 SIMD_rsqrt(SIMDVec4 a) {
        return Vec4::rsqrt(a);
    }

    SIMDVec4 SIMD_min(SIMDVec4 a, SIMDVec4 b) {
        return Vec4::min(a, b);
    }

    SIMDVec4 SIMD_max(SIMDVec4 a, SIMDVec4 b) {
        return Vec4::max(a, b);
    }
#endif
}  // namespace GLaDOS

#endif  //GLADOS_SIMD_H
