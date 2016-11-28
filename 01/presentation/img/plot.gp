#!/usr/bin/env gnuplot

ftail = 'variant4.log'

set terminal svg
set output "variant4.svg"

set yrange [0:5]
set xlabel "Length"
set ylabel "Time"

plot \
	ftail using 1:3 with lines title "verbose", \
	ftail using 1:4 with lines title "simple", \
	ftail using 1:5 with lines title "autovec", \
	ftail using 1:6 with lines title "avx2",
