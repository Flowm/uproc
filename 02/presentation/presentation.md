% Cache measurement
% Florian Mauracher `<florian.mauracher@tum.de>`
% Jonas Jelten `<jelten@in.tum.de>`

# what

* google benchmark suite
* https://github.com/google/benchmark

# cacheline size

``` cpp
void BM_cacheline_length(benchmark::State& state) {
	const int size = state.range(0);
	char* arr = new char[size]();
	const int stride = state.range(1);
	int x = 0, y = 0;

	while (state.KeepRunning()) {
		for (int i = 0; i < size; i += stride) {
			x += arr[i];
		}

		benchmark::DoNotOptimize(y += x);
	}
	state.SetBytesProcessed(
		state.iterations()*size/stride);
}
```

# cacheline results
![](../data/cacheline-size-speed-i5-5257U.png)

# easiest method

```
gcc '-###' -e -v -march=native empty.h 2>&1
```

# shuffling the memory blocks

``` cpp
union cacheline {
	cacheline *next;
	char dummydata[cacheline_size];
};

auto cachelines = std::make_unique<cacheline[]>(n);

std::vector<unsigned int> order;
std::iota(std::begin(order), std::end(order), 0);

std::shuffle(order.begin(), order.end(),
             std::mt19937(std::random_device{}));

for (int i = 0; i < n; i++) {
	int next_idx = order[(i + 1) % n];
	cachelines[order[i]].next = &cachelines[next_idx];
}
```

# perform random accesses

``` cpp
while (state.KeepRunning()) {
	for (register int i = 0; i < accesses; i++) {
		current_cacheline = current_cacheline->next;
	}
}
```

# bench results

![](img/cache-sizes.png)


# lstopo

![](img/lstopo.png)
