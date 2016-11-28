variant4.pdf: bench.svg
	inkscape --export-pdf-version=1.4 --export-pdf=$@ --export-dpi=500 --without-gui $^

bench.svg: plot.gp variant4.log
	./plot.gp
