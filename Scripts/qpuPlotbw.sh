#!/bin/sh
gnuplot << EOF
reset 
set terminal postscript eps "Helvetica" 20
set output '$1-bw.eps'

set title 'QpU vs. Total Time'
set xlabel 'Query Per Update'
set ylabel 'Total Time (ms)'
set xrange [0:$2]

set style line 1 lt 1 lw 5
set style line 2 lt 2 lw 5
set style line 3 lt 3 lw 5
set style line 4 lt 6 lw 5
set style line 5 lt 1 lw 3
set style line 6 lt 2 lw 3
set style line 7 lt 3 lw 3
set style line 8 lt 6 lw 3

plot "$1" using 1:2 with lines ls 1 title "DFS", \
 "" using 1:3 with lines ls 2 title "DG0", \
 "" using 1:4 with lines ls 3 title "DG1", \
 "" using 1:5 with lines ls 4 title "DG2"
pause -1
#
EOF
gs -sDEVICE=jpeg -dJPEGQ=100 -dNOPAUSE -dBATCH -dSAFER -r300 -sOutputFile=${1}.jpg ${1}.eps
mogrify -trim -resize 800x600 ${1}.jpg
