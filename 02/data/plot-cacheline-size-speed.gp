#!/usr/bin/env gnuplot

set datafile separator ","
set decimalsign "."

set terminal svg enhanced mouse size 1024,768
set output "results-cacheline-size-speed-i5-5257U.svg"

set xlabel "Size"
set logscale x

set ylabel "Speed MB/s"
set yrange [0:3e3]
#set format y "%e"

set title "Intel Core i5-5257U"
plot \
	"<(sed -n '/^[0-9]*,1,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 1", \
	"<(sed -n '/^[0-9]*,2,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 2", \
	"<(sed -n '/^[0-9]*,4,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 4", \
	"<(sed -n '/^[0-9]*,8,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 8", \
	"<(sed -n '/^[0-9]*,16,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 16", \
	"<(sed -n '/^[0-9]*,32,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 32", \
	"<(sed -n '/^[0-9]*,60,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 60", \
	"<(sed -n '/^[0-9]*,64,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 64", \
	"<(sed -n '/^[0-9]*,128,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 128", \
	"<(sed -n '/^[0-9]*,256,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 256", \
	"<(sed -n '/^[0-9]*,512,/p' results-cacheline-size-speed-i5-5257U.csv)" using 1:($7/1048576) with lines title "Stride 512", \
