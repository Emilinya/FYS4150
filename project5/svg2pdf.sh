#!/bin/sh

# simple script to convert svgs to pdfs that can be used with latex
FILES="./imgs/problem4/*.svg ./imgs/problem7/*.svg ./imgs/problem8/*.svg ./imgs/problem9/*.svg"
for f in $FILES
do
    echo "Processing $f..."
    NEW_NAME=$(echo $(echo "$f" | sed -e 's/svg/pdf/g') | sed -e 's/imgs/imgs\/pdfimgs/g')
    NEW_FOLDER=$(echo "$NEW_NAME" | sed -e 's/\/[^\/]*\.pdf//g')

    mkdir -p $NEW_FOLDER
    rsvg-convert -f pdf -o $NEW_NAME $f
done
