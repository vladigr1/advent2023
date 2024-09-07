#include <benchmark/benchmark.h>

/* Heap vs. generating lists
what:
The original implementation uses the heap.
the alternative implementation return the list

why:
The intended is to check the return Copy elision optimization on a list.
We initialization `current` from a function.
We copy to prev.
*/

void solver(int64_t max_line);

static void BM_original(benchmark::State &state) {
    for ([[maybe_unused]] auto _ : state) { solver(state.range(0)); }
}
// Register the function as a benchmark
BENCHMARK(BM_original)->Range(3, 140);
// Run the benchmark
BENCHMARK_MAIN();
