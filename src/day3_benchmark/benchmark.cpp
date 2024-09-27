#include <benchmark/benchmark.h>

/* Heap vs. generating lists
what:
The original implementation uses the heap.
the alternative implementation return the list

why:
The intended is to check the return Copy elision optimization on a list.
We initialization `current` from a function.
We copy to prev.

result:
Comparing C:/Users/do2vl/Desktop/cpp/advent2023/out/build/windows-clang-debug/src/day3_benchmark/day3_benchamrk.exe to C:/Users/do2vl/Desktop/cpp/advent2023/out/build/windows-clang-debug/src/day3_benchmark/day3_benchamrk_v2.exe
Benchmark                         Time             CPU      Time Old      Time New       CPU Old       CPU New
--------------------------------------------------------------------------------------------------------------
BM_original/3                  +0.1909         -0.0243         90410        107674         85794         83705
BM_original/8                  +0.4048         -0.0034        209429        294206        205776        205078
BM_original/64                 +0.2894         +0.3070       1558430       2009443       1380522       1804361
BM_original/140                +0.2631         +0.2642       3516003       4441054       2900095       3666201

conclusion:
it is better to work with the moving pointer rather then coping a list.
Copy is done per item.

*/

void solver(int64_t max_line);

static void BM_original(benchmark::State &state) {
    for ([[maybe_unused]] auto _ : state) { solver(state.range(0)); }
}
// Register the function as a benchmark
constexpr int64_t MULPTIPLER {3};
constexpr int64_t RANGE_END {140};
BENCHMARK(BM_original)->Range(MULPTIPLER, RANGE_END);
// Run the benchmark
BENCHMARK_MAIN();
