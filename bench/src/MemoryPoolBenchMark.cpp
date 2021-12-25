#include <benchmark/benchmark.h>
#include "memory/FixedSizeMemoryPool.hpp"

using namespace GLaDOS;

struct MockKlass {
    MockKlass(uint32_t i) : _i{i} {}
    uint32_t _i;
};

static void BM_FixedSizeMemoryPoolCreation(benchmark::State& state) {
    for (auto _ : state) {
        FixedSizeMemoryPool<MockKlass> mockPool;
    }
}

BENCHMARK(BM_FixedSizeMemoryPoolCreation);

static void BM_FixedSizeMemoryPoolAllocation(benchmark::State& state) {
    FixedSizeMemoryPool<MockKlass> mockPool;
    for (auto _ : state) {
        MockKlass* t = mockPool.allocate(10);
        mockPool.deallocate(t);
    }
}

BENCHMARK(BM_FixedSizeMemoryPoolAllocation);

static void BM_WithoutMemoryPoolAllocation(benchmark::State& state) {
    for (auto _ : state) {
        MockKlass* t = new MockKlass(10);
        delete t;
    }
}

BENCHMARK(BM_WithoutMemoryPoolAllocation);