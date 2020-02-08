set term pdf font "Times-New-Roman, 12" size 6,2
set size ratio 0.25
set output "disp_frame.pdf"
set xlabel "Time"
set ylabel "Displacement"
plot 'disp_frame.dat' using 1:4 title "RODS" w l lw 0.5, \
 	 "disp_frame_sap.dat" using 1:2 title "SAP2000" w l lw 0.5

