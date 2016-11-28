% Microprocessor optimization
% Florian Mauracher `<florian.mauracher@tum.de>`
% Jonas Jelten `<jelten@in.tum.de>`


# What

* gcc auto vectorization
* avx2 intrinsics


# Approach

* Basic C implementation first
* Optimize and simplify it
* Leveraging gcc optimizations


# Auto-vectorization in GCC

* `-ftree-vectorize`
* Enabled by default with `-O3`
* See reasons with `-ftree-vectorizer-verbose=2 -fopt-info-vec-missed -fdump-tree-vect`
* Allow instructions by `-march=native`, `-msse2` or `-mavx`
* https://gcc.gnu.org/projects/tree-ssa/vectorization.html


# Issues

* Unvectorizable Loops
* Control flow (= Branch) ("`control flow in loop`")
* Unknown iteration count ("`number of iterations cannot be computed`")

Reason:

``` cpp
// uncountable loop:
while (*p != NULL) {
	*q++ = *p++;
}
```

# Simple function

``` cpp
void toupper_verbose(char *text) {
	int i;
	for (i = 0; text[i] != 0; i++) {
		char c = text[i];

		if (c >= 97 && c <= 122) {
			char newchar = c + ('A' - 'a');
			text[i] = newchar;
		}
	}
}
```


# Same function

``` cpp
void toupper_simple(char *text) {
	while (*text) {
		if (*text >= 97 && *text <= 122) {
			*text = *text - 0x20;
		}
		text++;
	}
}
```


# Same machinecode

``` nasm
  toupper:
            jmp    test:
  loop:     lea    edx, [rax-0x61]
            cmp    dl, 0x19
            ja     addone:
            sub    eax, 0x20
            mov    [rdi], al
  addone:   add    rdi, 0x1
  test:     movzx  eax, [rdi]
            test   al, al
            jne    loop:
            ret
```


# Autovectorized function

``` cpp
void toupper_autovec(char *text) {
	size_t len = strlen(text);
	for (int i = 0; i < len; i++) {
		text[i] = text[i] & ~0x20;
	}
}
```


# AVX2 variant

``` cpp
void toupper_intr_and(char *text) {
	size_t len = strlen(text);
	__m256i sub_mask = _mm256_set1_epi8(~0x20);
	for (int i = 0; i < len-32; i += 32) {
		__m256i str = _mm256_load_si256(&text[i]);
		__m256i result = _mm256_and_si256(str, sub_mask);
		_mm256_store_si256((__m256i *) &text[i], result);
	}
	for (int i = len; i < len; i++) {
		text[i] = text[i] ^ ~0x20;
	}
}
```


# Performance measuring

![](img/variant4.pdf)
