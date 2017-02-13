#include <benchmark/benchmark_api.h>

// HELPER FUNCTIONS
char create_char(int ratio) {
	bool lowercase = rand() % 100 < ratio;
	char letter = rand() % 26 + 1; // a,A=1; b,B=2; ...

	return 0x40 + lowercase * 0x20 + letter;
}

char* create_string(char* text, unsigned long size, int ratio) {
	srand(1); // Ensure all strings are identical

	unsigned long i = 0;
	for (i = 0; i < size-1; i++) {
		text[i] = create_char(ratio);
	}
	text[i] = '\0';

	return text;
}

void print_string(const char* prefix, const char* string) {
	printf("%s: %.10s\n", prefix, string);
}

// BENCHMARK TARGETS
void BM_toupper_simple(benchmark::State& state) {
	const unsigned long size = state.range(0);
	const int ratio = 50; //state.range(1);
	char* const arr = new char[size]();
	create_string(arr, size, ratio);
	//print_string("STR:", arr);
	char* text;

	while (state.KeepRunning()) {
		state.PauseTiming();
		create_string(arr, size, ratio);
		text = arr; // Reset text pointer to be used in algorithm
		state.ResumeTiming();

		while (*text) {
			if (*text >= 'a' && *text <= 'z') {
				*text = *text - 0x20;
			}
			text++;
		}
	}

	//print_string("END:", arr);
	delete[] arr;
}
//BENCHMARK(BM_toupper_simple)->RangeMultiplier(2)->Range(1<<8, 1<<14);
BENCHMARK(BM_toupper_simple)->Arg(1<<10);


BENCHMARK_MAIN()
