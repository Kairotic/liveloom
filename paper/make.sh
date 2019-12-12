pandoc --variable mainfont="Linux Libertine O" --pdf-engine=xelatex  -V geometry:margin=1in --number-sections --csl harvard.csl --bibliography=/home/alex/Dropbox/citeulike/alex.bib --bibliography=extra.bib --bibliography=extra2.bib --bibliography=extra3.bib --bibliography=extra4.bib paper.md -o paper.odt

#pandoc --variable mainfont="Linux Libertine O" --pdf-engine=xelatex  -V geometry:margin=1in --number-sections --csl chicago-author-date.csl --bibliography=/home/alex/Dropbox/citeulike/alex.bib paper.md -o paper.pdf
