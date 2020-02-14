set terminal png size 1280,800 font 'Helvetica,16'
set output "image.png"
set title "benchmark"
set xlabel "N"
set ylabel "time"
plot "out.txt" u 1:2 w l t "bst" #, "out.txt" u 1:3 w l t "map"
