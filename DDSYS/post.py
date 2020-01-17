import scipy as sp
import scipy.linalg as spl
import matplotlib.pyplot as plt
plt.style.use("ggplot")

plt.rcParams['font.sans-serif']=['SimSun Roman'] #正常显示中文
plt.rcParams['font.size']=16 #字体大小
plt.rcParams['axes.unicode_minus']=False #正常显示负号
# plt.rcParams['grid.linestyle']='--'
plt.rcParams['mathtext.fontset']='cm'


def loadcsv(filename):
    return sp.loadtxt(filename, delimiter=',')

data = loadcsv("disp.csv")
t = data[:,0]
dsp = data[:,2]

data = loadcsv("force.csv")
df = data[:,2]

plt.figure(1,(12,6))
plt.plot(t, dsp)

plt.figure(2,(6,6))
plt.plot(dsp, df)

plt.show()