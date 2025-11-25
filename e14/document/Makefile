all: fsim.pdf gsim4.pdf e14.pdf


e14.pdf : e14.tex
	platex e14.tex && platex e14.tex && dvipdfmx e14.dvi

gsim4.pdf : gsim4.tex
	platex gsim4.tex && platex gsim4.tex &&  dvipdfmx gsim4.dvi

fsim.pdf : fsim.tex
	platex fsim.tex && platex fsim.tex && dvipdfmx fsim.dvi 

clean:
	rm *.log *.aux *.dvi *.pdf

