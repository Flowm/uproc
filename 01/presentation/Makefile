slides.pdf: presentation.md img/variant4.pdf img/variant4.log
	pandoc -t beamer $^ -o $@

img/variant4.pdf: img/variant4.svg
	inkscape --export-pdf-version=1.4 --export-pdf=$@ --export-dpi=500 --without-gui $^

img/variant4.svg: img/plot.gp img/variant4.log
	./img/plot.gp
