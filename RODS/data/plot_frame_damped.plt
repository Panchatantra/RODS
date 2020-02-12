# set term wxt font "Times-New-Roman, 12" size 1600,400
set term png lw 3 font "Times New Roman, 16" fontscale 3 size 3200,1200
set output "../docs/disp_frame_damped.png"
# set term pdfcairo font "Times-New-Roman, 10" size 5.8,2.2
# set output "disp_frame_damped.pdf"
set size ratio 0.3
set xlabel "Time"
set ylabel "Displacement"
set grid
plot "disp_frame_damped_sap.dat" using 1:2 skip 14 title "SAP2000" w l lw 1 lc rgb "blue", \
 	 "disp_frame_damped.dat" using 1:2 title "RODS" w l lw 1 lc rgb "red"
# pause mouse

clear
set term png lw 3 font "Times New Roman, 16" fontscale 3 size 3200,1200
set output "../docs/disp_frame_damping.png"
# set term pdfcairo font "Times-New-Roman, 10" size 5.8,2.2
# set output "disp_frame_damped.pdf"
set size ratio 0.3
set xlabel "Time"
set ylabel "Displacement"
set grid
plot "disp_frame.dat" using 1:2 title "Original Structure" w l lw 1 lc rgb "blue", \
 	 "disp_frame_damped.dat" using 1:2 title "Damped Structure" w l lw 1 lc rgb "red"

clear
set term png lw 3 font "Times New Roman, 16" fontscale 3 size 2400,2600
set output "../docs/damper.png"
# set term pdfcairo font "Times-New-Roman, 10" size 5.8,2.2
# set output "disp_frame_damped.pdf"
set size ratio 1
set xlabel "Deformation"
set ylabel "Force"
set grid
set key left top
plot "damper_sap.dat" using 2:3 skip 15 title "SAP2000" w l lw 1 lc rgb "blue", \
 	 "damper.dat" using 2:3 title "RODS" w l lw 1 lc rgb "red"