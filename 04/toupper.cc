#include <benchmark/benchmark_api.h>
#include <chrono>
#include <string.h>

#if defined(__SSE2__) || defined(__AVX2__)
#include <immintrin.h>
#endif


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
BENCHMARK_CAPTURE(BM_toupper, toupper_noact, toupper_noact, 1<<8)->UseManualTime();

void toupper_simple(char *text) {
	while (*text) {
		if (*text >= 97 && *text <= 122) {
			*text = *text - 0x20;
		}
		text++;
	}
}
BENCHMARK_CAPTURE(BM_toupper, toupper_simple, toupper_simple, 1<<10)->UseManualTime();

static void toupper_autovec(char *text) {
	size_t len = strlen(text);
	for (size_t i = 0; i < len; i++) {
		text[i] = text[i] & ~0x20;
	}
}
BENCHMARK_CAPTURE(BM_toupper, toupper_autovec, toupper_autovec, 1<<10)->UseManualTime();

#ifdef __SSE2__
#define BLOCK_SIZE 16
static void toupper_intr_and_sse2(char *text) {
	size_t len = strlen(text);
	__m128i sub_mask = _mm_set1_epi8(~0x20);
	for (size_t i = 0; i < len-BLOCK_SIZE; i += BLOCK_SIZE) {
		__m128i str = _mm_load_si128((__m128i *) &text[i]);
		__m128i result = _mm_and_si128(str, sub_mask);
		_mm_store_si128((__m128i *) &text[i], result);
	}
	// Process the remaining chars
	for (size_t i = len - BLOCK_SIZE; i < len; i++) {
		text[i] = text[i] ^ ~0x20;
	}
}
#undef BLOCK_SIZE
BENCHMARK_CAPTURE(BM_toupper, toupper_intr_and_sse2, toupper_intr_and_sse2, 1<<10)->UseManualTime();
#endif

#ifdef __AVX2__
#define BLOCK_SIZE 32
static void toupper_intr_and_avx2(char *text) {
	size_t len = strlen(text);
	// AVX doesn't provide boolean AND, thus AVX2 is required
	// https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_and_&techs=SSE2,AVX,AVX2&expand=269
	__m256i sub_mask = _mm256_set1_epi8(~0x20);
	for (size_t i = 0; i < len-BLOCK_SIZE; i += BLOCK_SIZE) {
		__m256i str = _mm256_load_si256((__m256i *) &text[i]);
		__m256i result = _mm256_and_si256(str, sub_mask);
		_mm256_store_si256((__m256i *) &text[i], result);
	}
	// Process the remaining chars
	for (size_t i = len - BLOCK_SIZE; i < len; i++) {
		text[i] = text[i] ^ ~0x20;
	}
}
#undef BLOCK_SIZE
BENCHMARK_CAPTURE(BM_toupper, toupper_intr_and_avx2, toupper_intr_and_avx2, 1<<10)->UseManualTime();
#endif

#if defined(__SSE2__) || defined(__AVX2__)
static void toupper_intr_and(char *text) {
	size_t len = strlen(text);
#ifdef __AVX2__
#define BLOCK_SIZE 32
	// AVX doesn't provide boolean AND, thus AVX2 is required
	// https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_and_&techs=SSE2,AVX,AVX2&expand=269
	__m256i sub_mask = _mm256_set1_epi8(~0x20);
	for (size_t i = 0; i < len-BLOCK_SIZE; i += BLOCK_SIZE) {
		__m256i str = _mm256_load_si256((__m256i *) &text[i]);
		__m256i result = _mm256_and_si256(str, sub_mask);
		_mm256_store_si256((__m256i *) &text[i], result);
	}
#else
#ifdef __SSE2__
#define BLOCK_SIZE 16
	__m128i sub_mask = _mm_set1_epi8(~0x20);
	for (size_t i = 0; i < len-BLOCK_SIZE; i += BLOCK_SIZE) {
		__m128i str = _mm_load_si128((__m128i *) &text[i]);
		__m128i result = _mm_and_si128(str, sub_mask);
		_mm_store_si128((__m128i *) &text[i], result);
	}
#endif // SSE2
#endif // AVX2

	// Process the remaining chars
	for (size_t i = len - BLOCK_SIZE; i < len; i++) {
		text[i] = text[i] ^ ~0x20;
	}
}
#undef BLOCK_SIZE
BENCHMARK_CAPTURE(BM_toupper, toupper_intr_and, toupper_intr_and, 1<<10)->UseManualTime();
#endif // SSE2 || AVX2


BENCHMARK_MAIN()
