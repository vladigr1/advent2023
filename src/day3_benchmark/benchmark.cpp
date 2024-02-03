#include <benchmark/benchmark.h>

void solver(int max_line);

static void BM_original(benchmark::State &state) {
    for ([[maybe_unused]] auto _ : state) { solver(state.range(0)); }
}
// Register the function as a benchmark
BENCHMARK(BM_original)->Range(3, 140);
// Run the benchmark
BENCHMARK_MAIN();
