#include <benchmark/benchmark_api.h>
#include <chrono>

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

void BM_toupper(benchmark::State& state, void (*toupper_func)(char*), unsigned long size) {
	const int ratio = 50;
	char* const arr = new char[size]();
	create_string(arr, size, ratio);
	//print_string("STR:", arr);
	char* text;

	while (state.KeepRunning()) {
		state.PauseTiming();
		create_string(arr, size, ratio);
		text = arr; // Reset text pointer to be used in algorithm
		state.ResumeTiming();

		// Pause during seems to add 150ns overhead, thus use manual timing
		auto start = std::chrono::high_resolution_clock::now();
		toupper_func(text);
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		state.SetIterationTime(elapsed.count());
	}

	//print_string("END:", arr);
	delete[] arr;
}

// BENCHMARK FUNCTIONS
void toupper_noact(char *text) {
	(void) text;
}

void toupper_simple(char *text) {
	while (*text) {
		if (*text >= 97 && *text <= 122) {
			*text = *text - 0x20;
		}
		text++;
	}
}

// REGISTER BENCHMARK FUNCTIONS
BENCHMARK_CAPTURE(BM_toupper, toupper_noact, toupper_noact, 1<<8)->UseManualTime();
BENCHMARK_CAPTURE(BM_toupper, toupper_simple, toupper_simple, 1<<8)->UseManualTime();


BENCHMARK_MAIN()
