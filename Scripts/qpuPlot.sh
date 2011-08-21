#!/bin/sh
gnuplot << EOF
reset 
set terminal postscript eps color
set output '$1.eps'
set timestamp

set title 'QpU vs. Total Time $1'
set xlabel 'Query Per Update'
set ylabel 'Total Time (ms)'
set xrange [0:$2]

plot "$1" using 1:2 with lines linewidth 6 title "DFS", \
 "" using 1:3 with lines linewidth 6 title "DG0", \
 "" using 1:4 with lines linewidth 6 title "DG1", \
 "" using 1:5 with lines linewidth 6 title "DG2"
pause -1
#
EOF
gs -sDEVICE=jpeg -dJPEGQ=100 -dNOPAUSE -dBATCH -dSAFER -r300 -sOutputFile=${1}.jpg ${1}.eps
mogrify -trim -resize 800x600 ${1}.jpg
