#include <benchmark/benchmark.h>
#include "utils/SIMD.h"
#include "math/Quat.h"

using namespace GLaDOS;

static void BM_SIMDFloat4Multiplication(benchmark::State& state) {
    for (auto _ : state) {
        SIMDVec4 a = SIMD_load(1, 2, 3, 4);
        SIMDVec4 b = SIMD_load(5, 6, 7, 8);
        SIMDVec4 c = SIMD_mul(a, b);
        real result[4] = { 0.f, };
        SIMD_store(result, c);
        real sum = result[0] + result[1] + result[2] + result[3];
    }
}

static void BM_SimpleQuaternionDotProduct(benchmark::State& state) {
    for (auto _ : state) {
        Quat a = {1, 2, 3, 4};
        Quat b = {5, 6, 7, 8};
        real c = Quat::dot(a, b);
    }
}

BENCHMARK(BM_SIMDFloat4Multiplication);
BENCHMARK(BM_SimpleQuaternionDotProduct);