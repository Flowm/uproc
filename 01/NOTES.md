## Auto-vectorization in GCC
- https://gcc.gnu.org/projects/tree-ssa/vectorization.html
- Enabled by default with -O3
- Debug reason for not applied optimization with `-fopt-info-vec-missed`
- Explicitly set vector instruction type with `-msse2` or `-avx`

### Issues
- Unvectorizable Loops
	- Message: `note: not vectorized: number of iterations cannot be computed`
	- From documentation:
```
Examples of loops that currently cannot be vectorized:
example1: uncountable loop:

while (*p != NULL) {
  *q++ = *p++;
}
```

- Control flow
	- Message: `note: not vectorized: control flow in loop.`
