reset
set term png enhanced font 'Verdana,10'
set ylabel "time (sec)"
set xlabel "size"
set xtics 10000
set title "Time Comparison (random input)"
set output "result.png"

plot [:][-1:]'merge-sort_result.txt' \
    using 1:2 with linespoints linewidth 2 title 'merge'
