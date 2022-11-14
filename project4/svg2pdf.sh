#!/bin/sh

# simple script to convert svgs to pdfs that can be used with latex
FILES="./imgs/*.svg"
for f in $FILES
do
  echo "Processing $f..."
  NEW_NAME=$(echo $(echo "$f" | sed -e 's/svg/pdf/g') | sed -e 's/imgs/imgs\/pdfimgs/g')
  mkdir -p "imgs/pdfimgs"

  rsvg-convert -f pdf -o $NEW_NAME $f
done
