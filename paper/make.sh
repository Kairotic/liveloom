pandoc --variable mainfont="Linux Libertine O" --pdf-engine=xelatex  -V geometry:margin=1in --number-sections --csl chicago-author-date.csl paper.md -o paper.pdf

#pandoc --variable mainfont="Linux Libertine O" --pdf-engine=xelatex  -V geometry:margin=1in --number-sections --csl chicago-author-date.csl --bibliography=/home/alex/Dropbox/citeulike/alex.bib paper.md -o paper.pdf
