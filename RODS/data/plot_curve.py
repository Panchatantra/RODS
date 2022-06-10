import numpy as np
import matplotlib.pyplot as plt
plt.style.use("ggplot")

plt.rcParams['font.family']='serif' #正常显示中文
# plt.rcParams['font.sans-serif']=['SimSun Roman'] #正常显示中文
plt.rcParams['font.size']=12 #字体大小
plt.rcParams['axes.unicode_minus']=False #正常显示负号
# plt.rcParams['grid.linestyle']='--'
plt.rcParams['mathtext.fontset']='cm'

data = np.loadtxt("disp0.dat")
t = data[:,0]
dsp = data[:,2]

# data = np.loadtxt("disp_frame_4d.dat")
# t_d = data[:,0]
# dsp_d = data[:,1]

plt.figure(1,(12,6))
plt.plot(t, dsp)
# plt.plot(t_d, dsp_d)
plt.xlabel("Time")
plt.ylabel("Displacement")
# plt.savefig("docs/sdof_seismic.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_resonance_nl.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_seismic_nl.eps", bbox_inches="tight")

plt.show()