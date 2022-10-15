#!/bin/sh

# simple script to convert svgs to pdfs that can be used with latex
FILES="./imgs/*.svg ./imgs/misc/*.svg"
for f in $FILES
do
  echo "Processing $f..."
  NEW_NAME=$(echo $(echo "$f" | sed -e 's/svg/pdf/g') | sed -e 's/imgs/imgs\/pdfimgs/g')
  rsvg-convert -f pdf -o $NEW_NAME $f
done
