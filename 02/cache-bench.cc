#include <benchmark/benchmark_api.h>

void BM_empty(benchmark::State& state) {
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(state.iterations());
	}
}
BENCHMARK(BM_empty);

static void BM_cache_line_length_args(benchmark::internal::Benchmark* b) {
	for (int i = 1<<5; i <= 1<<12; i <<= 1)
		for (int j = 1; j <= 1024; ++j)
			b->Args({i, j});
}

void BM_cache_line_length(benchmark::State& state) {
	const int size = state.range(0)*1024;
	char* arr = new char[size]();
	const int stride = state.range(1);
	int x = 0, y = 0;

	while (state.KeepRunning()) {
		for (int i = 0; i < size; i+= stride) {
			x += arr[i];
		}
		benchmark::ClobberMemory();
		benchmark::DoNotOptimize(y += x);
	}

	state.SetBytesProcessed(static_cast<int64_t>(state.iterations())*size/stride);
	delete[] arr;
}
//BENCHMARK(BM_cache_line_length)->Args({1024,1});
//BENCHMARK(BM_cache_line_length)->Args({1024,8});
BENCHMARK(BM_cache_line_length)->Apply(BM_cache_line_length_args);


BENCHMARK_MAIN()
