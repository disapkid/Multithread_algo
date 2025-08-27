#include "accumulate.hpp"
#include "sort.hpp"
#include "for_each.hpp"
#include <random>
#include <benchmark/benchmark.h>

std::vector<int> create_data(int size) {
    std::vector<int> vec(size);
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0,10000);
    for(auto& i : vec) i = dist(rng);
    return vec;
};

static void BM_my_multithread_sort(benchmark::State& state) {
    for(auto _ : state) {
        auto vec = create_data(state.range(0));
        benchmark::DoNotOptimize(vec);
        multithread_sort(vec.begin(), vec.end());
        benchmark::ClobberMemory();
    };
};

BENCHMARK(BM_my_multithread_sort)-> Arg(1 << 30);

static void BM_my_async_sort(benchmark::State& state) {
    for(auto _ : state) {
        auto vec = create_data(state.range(0));
        benchmark::DoNotOptimize(vec);
        async_sort(vec.begin(), vec.end());
        benchmark::ClobberMemory();
    };
};

BENCHMARK(BM_my_async_sort)-> Arg(1 << 30);

static void BM_std_sort(benchmark::State& state) {
    for(auto _ : state) {
        auto vec = create_data(state.range(0));
        benchmark::DoNotOptimize(vec);
        std::sort(vec.begin(), vec.end());
        benchmark::ClobberMemory();
    };
};

BENCHMARK(BM_std_sort)-> Arg(1 << 30);

BENCHMARK_MAIN();