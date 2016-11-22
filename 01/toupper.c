#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "options.h"

#include <immintrin.h>

int debug = 0;
double *results;
double *ratios;
unsigned long *sizes;

int no_sz = 1, no_ratio = 1, no_version = 1;

static inline double gettime(void) {
	struct timeval time;
	struct timezone zone;

	if (0 != gettimeofday(&time, &zone)) {
		perror("gettimeofday");
	}

	return time.tv_sec + (time.tv_usec / 1e6);
}

static void toupper_verbose(char *text) {
	int i;
	for (i = 0; text[i] != 0; i++) {
		char c = text[i];

		if (c >= 97 && c <= 122) {
			char newchar = c + ('A' - 'a');
			text[i] = newchar;
		}
	}
}

static void toupper_simple(char *text) {
	while (*text) {
		if (*text >= 97 && *text <= 122) {
			*text = *text - 0x20;
		}
		text++;
	}
}

static void toupper_autovec(char *text) {
	size_t len = strlen(text);
	for (int i = 0; i < len; i++) {
		text[i] = text[i] & (0xFF^0x20);
	}
}

static void toupper_intr_and(char *text) {
	size_t len = strlen(text);

#ifdef __AVX2__
	// AVX doesn't provide boolean AND
	// https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_and_&techs=SSE2,AVX,AVX2&expand=269
	// TODO: Fallback to SSE2 in case AVX2 isn't available
	__m256i sub_mask = _mm256_set1_epi8(0xFF^0x20);
	__m256i str = _mm256_load_si256((int *) text);
	__m256i result = _mm256_and_si256(str, sub_mask); // AVX2 only
	memcpy(text, &result, 256); // TODO: Directly store the data in the correct place

	// TODO: Loop over all values
	// TODO: Handle remaining chars with AVX2 mask
#endif
}

/*****************************************************************/

// align at 16byte boundaries
void *mymalloc(unsigned long int size) {
	void *addr = malloc(size + 32);
	return (void *)((unsigned long int)addr / 16 * 16 + 16);
}

char createChar(int ratio) {
	char isLower = rand() % 100;

	// upper case=0, lower case=1
	if (isLower < ratio)
		isLower = 0;
	else
		isLower = 1;

	char letter = rand() % 26 + 1;  // a,A=1; b,B=2; ...

	return 0x40 + isLower * 0x20 + letter;
}

char *init(unsigned long int sz, int ratio) {
	int i      = 0;
	char *text = (char *)mymalloc(sz + 1);
	srand(1);  // ensures that all strings are identical
	for (i = 0; i < sz; i++) {
		char c  = createChar(ratio);
		text[i] = c;
	}
	text[i] = '\0';
	return text;
}

/*
 * ******************* Run the different versions **************
 */

typedef void (*toupperfunc)(char *text);

void run_toupper(int size, int ratio, int version, toupperfunc f, const char *name) {

	double start, stop;
	int index;

	index = ratio;
	index += size * no_ratio;
	index += version * no_sz * no_ratio;

	char *text = init(sizes[size], ratios[ratio]);

	if (debug)
		printf("Before: %.40s...\n", text);

	start = gettime();
	(*f)(text);
	stop           = gettime();
	results[index] = stop - start;

	if (debug)
		printf("After:  %.40s...\n", text);

	// the text will memleak here, but a free() won't work because of the funny
	// memory alignment.
}

struct _toupperversion {
	const char *name;
	toupperfunc func;
} toupperversion[] = {
	{"verbose", toupper_verbose},
	{"simple", toupper_simple},
	{"autovec", toupper_autovec},
	{"intr_and", toupper_intr_and},
	{0, 0}
};

void run(int size, int ratio) {
	int v;
	for (v = 0; toupperversion[v].func != 0; v++) {
		run_toupper(size, ratio, v, toupperversion[v].func, toupperversion[v].name);
	}
}

void printresults() {
	int i, j, k, index;
	printf("%s\n", OPTS);

	for (j = 0; j < no_sz; j++) {
		for (k = 0; k < no_ratio; k++) {
			printf("Size: %ld \tRatio: %f \tRunning time:", sizes[j], ratios[k]);

			for (i = 0; i < no_version; i++) {
				index = k;
				index += j * no_ratio;
				index += i * no_sz * no_ratio;
				printf("\t%s: %f", toupperversion[i].name, results[index]);
			}
			printf("\n");
		}
	}
}

int main(int argc, char *argv[]) {
	unsigned long int min_sz = 800000, max_sz = 0, step_sz = 10000;
	int min_ratio = 50, max_ratio = 0, step_ratio = 1;
	int arg, i, j, v;
	int no_exp;

	for (arg = 1; arg < argc; arg++) {
		if (0 == strcmp("-d", argv[arg])) {
			debug = 1;
		}
		if (0 == strcmp("-l", argv[arg])) {
			min_sz = atoi(argv[arg + 1]);
			if (arg + 2 >= argc)
				break;
			if (0 == strcmp("-r", argv[arg + 2]))
				break;
			if (0 == strcmp("-d", argv[arg + 2]))
				break;
			max_sz  = atoi(argv[arg + 2]);
			step_sz = atoi(argv[arg + 3]);
		}
		if (0 == strcmp("-r", argv[arg])) {
			min_ratio = atoi(argv[arg + 1]);
			if (arg + 2 >= argc)
				break;
			if (0 == strcmp("-l", argv[arg + 2]))
				break;
			if (0 == strcmp("-d", argv[arg + 2]))
				break;
			max_ratio  = atoi(argv[arg + 2]);
			step_ratio = atoi(argv[arg + 3]);
		}
	}

	for (v = 0; toupperversion[v].func != 0; v++)
		no_version = v + 1;

	if (0 == max_sz)
		no_sz = 1;
	else
		no_sz = (max_sz - min_sz) / step_sz + 1;

	if (0 == max_ratio)
		no_ratio = 1;
	else
		no_ratio = (max_ratio - min_ratio) / step_ratio + 1;

	no_exp  = v * no_sz * no_ratio;
	results = (double *)malloc(sizeof(double[no_exp]));
	ratios  = (double *)malloc(sizeof(double[no_ratio]));
	sizes   = (long *)malloc(sizeof(long[no_sz]));

	for (i = 0; i < no_sz; i++)
		sizes[i] = min_sz + i * step_sz;

	for (i = 0; i < no_ratio; i++)
		ratios[i] = min_ratio + i * step_ratio;

	for (i = 0; i < no_sz; i++)
		for (j = 0; j < no_ratio; j++)
			run(i, j);

	printresults();
	return 0;
}
