#!/bin/sh

# simple script to convert svgs to pdfs that can be used with latex
FILES="./imgs/schrodinger_intiializer_test/*.svg ./imgs/schrodinger_solver_test/*.svg ./imgs/double_slit/*.svg ./imgs/triple_slit/*.svg"
for f in $FILES
do
    echo "Processing $f..."
    NEW_NAME=$(echo $(echo "$f" | sed -e 's/svg/pdf/g') | sed -e 's/imgs/imgs\/pdfimgs/g')
    NEW_FOLDER=$(echo "$NEW_NAME" | sed -e 's/\/[^\/]*\.pdf//g')

    mkdir -p $NEW_FOLDER
    rsvg-convert -f pdf -o $NEW_NAME $f
done
