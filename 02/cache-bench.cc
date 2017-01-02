#include <benchmark/benchmark_api.h>

void BM_empty(benchmark::State& state) {
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(state.iterations());
	}
}
BENCHMARK(BM_empty);

void BM_spin_empty(benchmark::State& state) {
	while (state.KeepRunning()) {
		for (int x = 0; x < state.range(0); ++x) {
			benchmark::DoNotOptimize(x);
		}
	}
}
BENCHMARK(BM_spin_empty)->Arg(8)->Arg(512)->Arg(8192);


BENCHMARK_MAIN()
