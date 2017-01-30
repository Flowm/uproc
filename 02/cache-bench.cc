#include <benchmark/benchmark_api.h>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>


void BM_empty(benchmark::State& state) {
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(state.iterations());
	}
}
//BENCHMARK(BM_empty);


/*
 * Cacheline length
 */
static void BM_cacheline_length_stride_speed_args(benchmark::internal::Benchmark* b) {
	// from 32 to 4096 by *= 2
	for (int size = 1<<5; size <= 1<<12; size <<= 1)
		// from 1 to 1024 by += 1
		for (int stride = 1; stride <= 1024; stride++)
			// arguments: (size in kb, stride in byte)
			b->Args({size*1024, stride});
}

static void BM_cacheline_length_size_speed_args(benchmark::internal::Benchmark* b) {
	for (int size = 1<<0; size <= 1<<20; size <<= 1) {
		for (int stride = 1<<0; stride <= 1<<9; stride <<= 1)
			b->Args({size*1024, stride});
		b->Args({size*1024, 60});
	}
}

void BM_cacheline_length(benchmark::State& state) {
	const int size = state.range(0);
	char* arr = new char[size]();
	const int stride = state.range(1);
	int x = 0, y = 0;

	while (state.KeepRunning()) {
		for (int i = 0; i < size; i += stride) {
			x += arr[i];
		}

		// tell optimizer that everything may change:
		benchmark::ClobberMemory();

		// and do not remove this totally unneccessary statement:
		benchmark::DoNotOptimize(y += x);
	}

	state.SetBytesProcessed(static_cast<int64_t>(state.iterations())*size/stride);
	delete[] arr;
}

//BENCHMARK(BM_cacheline_length)->Apply(BM_cacheline_length_stride_speed_args);
//BENCHMARK(BM_cacheline_length)->Apply(BM_cacheline_length_size_speed_args);


/*
 * Cache size
 */
// 64 is the size of a cacheline
constexpr int cacheline_size = 64;

// number of random accesses
constexpr int accesses = 256;


union cacheline {
	cacheline *next;
	char dummydata[cacheline_size];
};


void BM_cache_size(benchmark::State& state) {
	// size of a cache line * n
	const int n = state.range(0);

	auto cachelines = std::make_unique<cacheline[]>(n);

	// calculate the order in which the cachelines should be accessed
	// start with an incrementing order
	std::vector<unsigned int> order(n);
	std::iota(std::begin(order), std::end(order), 0);

	// shuffle the order randomly.
	std::random_device rd;
	std::shuffle(order.begin(), order.end(), std::mt19937(rd()));
	// now the 'order' vector describes in which order the cache lines
	// should reference each other.
	// order[0] says: which cachline will run first
	// order[1] says: which cacheline will run second
	// so cacheline[order[0]].next = order[1]
	//    cacheline[order[1]].next = order[2]
	//    ...
	//    cacheline[order[n - 1]].next = order[0]
	//
	// initialize the cacheline pointer to order[0].
	for (int i = 0; i < n; i++) {
		int next_idx = order[(i + 1) % n];
		cachelines[order[i]].next = &cachelines[next_idx];
	}

	register cacheline *current_cacheline = &cachelines[order[0]];
	int sum = 0;

	while (state.KeepRunning()) {
		// do the random access
		for (register int i = 0; i < accesses; i++) {
			current_cacheline = current_cacheline->next;
		}

		benchmark::DoNotOptimize(sum += current_cacheline->dummydata[0]);
	}

	state.SetBytesProcessed(
		static_cast<int64_t>(state.iterations()) * (
			sizeof(cacheline*) * accesses + sizeof(char)
		)
	);
}

static void cache_size_args(benchmark::internal::Benchmark* b) {
	double stepsize = pow(2.0, 0.1);
	double maxsize = 1e6;

	int last = 0;
	for (double n = 1; n < maxsize; n *= stepsize) {
		if (last == int(n)) {
			continue;
		}
		last = int(n);
		b->Arg(last);
	}
}

BENCHMARK(BM_cache_size)->Apply(cache_size_args);
//BENCHMARK(BM_cache_size)->RangeMultiplier(2)->Range(1, 1<<22);


BENCHMARK_MAIN()
