ftail = 'variant4.log'

set terminal png size 1920,1080
set output "bench.png"

#set xrange [0:2048]
set yrange [0:0.6]
set xlabel "Length"
set ylabel "Time"

plot \
	ftail using 1:3 with lines title "verbose", \
	ftail using 1:4 with lines title "simple", \
	ftail using 1:5 with lines title "autovec", \
	ftail using 1:6 with lines title "avx2",
