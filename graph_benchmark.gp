set terminal png size 1280,800 font 'Helvetica,16'
set output "plot_benchmark.png"
set title "benchmark"
set xlabel "N"
set ylabel "time [microsecs]"

plot "data_benchmark.txt" u 1:2 w l t "std::map" , "data_benchmark.txt" u 1:3 w l t "bst", "data_benchmark.txt" u 1:4 w l t "balanced bst"
