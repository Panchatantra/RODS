import numpy as np
import matplotlib.pyplot as plt
plt.style.use("ggplot")

plt.rcParams['font.family']='serif' #正常显示中文
# plt.rcParams['font.sans-serif']=['SimSun Roman'] #正常显示中文
plt.rcParams['font.size']=12 #字体大小
plt.rcParams['axes.unicode_minus']=False #正常显示负号
# plt.rcParams['grid.linestyle']='--'
plt.rcParams['mathtext.fontset']='cm'

data = np.loadtxt("top_disp_wall.txt")
t = data[1:,0]
dsp = data[1:,1]

data = np.loadtxt("top_disp_wall_sap.txt")
t_s = data[:,0]
dsp_s = data[:,1]

plt.figure("结构顶点位移响应",(12,4))
plt.plot(t, dsp, label="RODS")
plt.plot(t_s, dsp_s, label="SAP2000", lw=1)
plt.xlabel("Time")
plt.ylabel("Displacement")
plt.legend()
# plt.savefig("docs/sdof_seismic.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_resonance_nl.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_seismic_nl.eps", bbox_inches="tight")

plt.show()