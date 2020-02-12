set term pdf font "Times-New-Roman, 10" size 5.8,1.5
set size ratio 0.2
set output "disp_inerter_system.pdf"
set xlabel "Time"
set ylabel "Displacement"
set grid
plot "disp0.dat" using 1:2 title "Original Structure" w l lw 0.5, \
 	 "disp.dat" using 1:2 title "Damped Structure" w l lw 0.5