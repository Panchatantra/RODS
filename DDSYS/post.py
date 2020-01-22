import scipy as sp
import scipy.linalg as spl
import matplotlib.pyplot as plt
plt.style.use("ggplot")

plt.rcParams['font.family']='serif' #正常显示中文
# plt.rcParams['font.sans-serif']=['SimSun Roman'] #正常显示中文
plt.rcParams['font.size']=12 #字体大小
plt.rcParams['axes.unicode_minus']=False #正常显示负号
# plt.rcParams['grid.linestyle']='--'
plt.rcParams['mathtext.fontset']='cm'


def loadcsv(filename):
    return sp.loadtxt(filename, delimiter=',')

# eq = sp.loadtxt("EQ-S-1.txt")
# n = len(eq)
# eq_dt = 0.005
# t = sp.linspace(0,(n-1)*eq_dt,n)
# plt.figure(0,(12,6))
# plt.plot(t, eq)
# plt.xlabel("Time")
# plt.ylabel("Acceleration")
# plt.savefig("docs/eq.eps", bbox_inches="tight")


data = loadcsv("disp.csv")
t = data[:,0]
dsp = data[:,2]

data = loadcsv("force.csv")
sf = data[:,1]
df = data[:,2]

plt.figure(1,(12,6))
plt.plot(t, dsp)
plt.xlabel("Time")
plt.ylabel("Displacement")
# plt.savefig("docs/sdof_seismic.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_resonance_nl.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_seismic_nl.eps", bbox_inches="tight")

plt.figure(2,(6,6))
plt.plot(dsp, df)
plt.xlabel("Displacement")
plt.ylabel("Force")
# plt.savefig("docs/sdof_resonance_nl_df.eps", bbox_inches="tight")
# plt.savefig("docs/sdof_seismic_nl_df.eps", bbox_inches="tight")


plt.figure(3,(6,6))
plt.plot(dsp, sf)
plt.xlabel("Displacement")
plt.ylabel("Force")

plt.show()