#!/bin/sh

set -eu

for i in "$@"; do
    latexmk ${i}.tex
    latexmk ${i}-3up.tex
    cp ${i}.pdf ${i}-3up.pdf ../pdf
done
