slides.pdf: presentation.md img/bench.pdf
	pandoc -t beamer $< -o $@

img/bench.pdf: img/bench.svg
	inkscape --export-pdf-version=1.4 --export-pdf=$@ --export-dpi=500 --without-gui $^

img/bench.svg: img/plot.gp img/bench.log
	cd img && ./plot.gp
