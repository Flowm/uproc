#!/usr/bin/env gnuplot

set datafile separator ","
set decimalsign "."

set terminal svg enhanced mouse size 1024,768
set output "results-cacheline-stride-speed-i5-5257U.svg"

set xlabel "Stride"
#set xrange [0:256]
set xrange [0:1024]

set ylabel "Speed B/s"
set yrange [0:2.5e9]
#set format y "%e"


set title "Intel Core i5-5257U"
plot \
	"<(sed -n '/^32,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "32K", \
	"<(sed -n '/^64,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "64K", \
	"<(sed -n '/^128,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "128K", \
	"<(sed -n '/^256,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "256K", \
	"<(sed -n '/^512,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "512K", \
	"<(sed -n '/^1024,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "1M", \
	"<(sed -n '/^2048,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "2M", \
	"<(sed -n '/^4096,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "4M", \
	"<(sed -n '/^8192,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "8M", \
	"<(sed -n '/^16384,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "16M", \
	"<(sed -n '/^32768,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "32M", \
	"<(sed -n '/^65536,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines title "64M", \


#set title "Intel Core i5-5257U acsplines"
#set samples 768
#plot \
#	"<(sed -n '/^32,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "32K", \
#	"<(sed -n '/^64,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "64K", \
#	"<(sed -n '/^128,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "128K", \
#	"<(sed -n '/^256,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "256K", \
#	"<(sed -n '/^512,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "512K", \
#	"<(sed -n '/^1024,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "1M", \
#	"<(sed -n '/^2048,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "2M", \
#	"<(sed -n '/^4096,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "4M", \
#	"<(sed -n '/^8192,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "8M", \
#	"<(sed -n '/^16384,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "16M", \
#	"<(sed -n '/^32768,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "32M", \
#	"<(sed -n '/^65536,/p' results-cache-bench-i5-5257U.csv)" using 2:7 with lines smooth acsplines title "64M", \
