# set term wxt font "Times-New-Roman, 12" size 1600,400
set term png lw 3 font "Times New Roman, 16" fontscale 3 size 3200,1200
set output "../docs/disp_frame.png"
# set term pdf font "Times-New-Roman, 10" size 5.8,1.5
# set output "disp_frame_damped.pdf"
set size ratio 0.3
set xlabel "Time"
set ylabel "Displacement"
set grid
plot "disp_frame_sap.dat" using 1:2 title "SAP2000" w l lw 1 lc rgb "blue", \
 	 "disp_frame.dat" using 1:2 title "RODS" w l lw 1 lc rgb "red"
# plot 'disp_frame_damped.dat' using 1:2 title "RODS" w l lw 0.5
# pause mouse