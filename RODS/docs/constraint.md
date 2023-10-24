# 约束条件

## 线性约束

$$
\begin{array}{c}
a_{11}u_{1} + a_{12}u_{2} + ... + a_{1n} u_{n} = c_1 \\
a_{21}u_{1} + a_{22}u_{2} + ... + a_{2n} u_{n} = c_2 \\
...\\
a_{m1}u_{1} + a_{m2}u_{2} + ... + a_{mn} u_{n} = c_m \\
\end{array}
$$

写成矩阵形式

$$
\boldsymbol{A u} = \boldsymbol{c} 
$$

$$
\boldsymbol{A} = \begin{pmatrix}
    a_{11} & a_{12} & \cdots & a_{1n}\\
    a_{21} & a_{22} & \cdots & a_{2n}\\
    \vdots & \vdots & \ddots & \vdots \\
    a_{m1} & a_{m2} & \cdots & a_{mn}\\
\end{pmatrix}_{m \times n}
$$

相应的速度和加速度约束条件

$$
\boldsymbol{A \dot{u}} = \boldsymbol{0} 
$$

$$
\boldsymbol{A \ddot{u}} = \boldsymbol{0}
$$

## 惩罚因子法

惩罚权重矩阵

$$
\boldsymbol{W} = \begin{pmatrix}
    w_1 \\
        & w_2\\
        &    & \ddots\\
        &    &  & w_m\\
\end{pmatrix}
$$


刚度矩阵修改为

$$
\boldsymbol{K}^* = \boldsymbol{K} + \boldsymbol{A}^T \boldsymbol{W}_u \boldsymbol{A}
$$

荷载向量修改为
$$
\boldsymbol{q}^* = \boldsymbol{q} + \boldsymbol{W} \boldsymbol{A}^T \boldsymbol{c}
$$

## Lagrange 乘子法

增加拉格朗日乘子作为未知量

静力方程变为

$$
\begin{pmatrix}
    \boldsymbol{K} & \boldsymbol{A}^T \\
    \boldsymbol{A} & \boldsymbol{0}
\end{pmatrix}
\begin{pmatrix}
    \boldsymbol{u}  \\
    \boldsymbol{\lambda} 
\end{pmatrix} = 
\begin{pmatrix}
    \boldsymbol{q}  \\
    \boldsymbol{c}
\end{pmatrix}
$$

# 自由度缩减法

指定活动自由度和从属自由度，对应向量分别记为 $\boldsymbol{u}_a$ 和 $\boldsymbol{u}_s$

将自由度重新排列

$$
\boldsymbol{u} = \begin{pmatrix}
    \boldsymbol{u}_a\\
    \boldsymbol{u}_s
\end{pmatrix}
$$

约束方程改写为

$$
\boldsymbol{A u} = \begin{pmatrix}
    \boldsymbol{A}_{sa} &
    \boldsymbol{A}_{ss}
\end{pmatrix}
\begin{pmatrix}
    \boldsymbol{u}_a\\
    \boldsymbol{u}_s
\end{pmatrix} = \boldsymbol{c}
$$

$$
\boldsymbol{A}_{sa} \boldsymbol{u}_a + \boldsymbol{A}_{ss} \boldsymbol{u}_s = \boldsymbol{c}
$$

$$
\boldsymbol{u}_s = \boldsymbol{A}_{ss}^{-1}(\boldsymbol{c} - \boldsymbol{A}_{sa} \boldsymbol{u}_a)
$$

当 $\boldsymbol{c} = \boldsymbol{0}$ 时，
$$
\boldsymbol{u}_s = -\boldsymbol{A}_{ss}^{-1} \boldsymbol{A}_{sa} \boldsymbol{u}_a
$$

$$
\boldsymbol{u} = \begin{pmatrix}
    \boldsymbol{u}_a\\
    \boldsymbol{u}_s
\end{pmatrix} = \begin{pmatrix}
    \boldsymbol{u}_a\\
    -\boldsymbol{A}_{ss}^{-1} \boldsymbol{A}_{sa} \boldsymbol{u}_a
\end{pmatrix} = \begin{pmatrix}
    \boldsymbol{I}_{aa}\\
    -\boldsymbol{A}_{ss}^{-1} \boldsymbol{A}_{sa}
\end{pmatrix} \boldsymbol{u}_a = \boldsymbol{T u}_a
$$

考虑约束条件后，原问题自由度由 $n$ 缩减为 $n-m$，缩减后矩阵及荷载向量

$$
\boldsymbol{M}^* = \boldsymbol{T}^T\boldsymbol{MT}
$$

$$
\boldsymbol{K}^* = \boldsymbol{T}^T\boldsymbol{KT}
$$

$$
\boldsymbol{C}^* = \boldsymbol{T}^T\boldsymbol{CT}
$$

$$
\boldsymbol{p}^* = \boldsymbol{T}^T\boldsymbol{p}
$$

当 $\boldsymbol{c} \neq \boldsymbol{0}$ 时，

$$
\boldsymbol{p}^* = \boldsymbol{T}^T\left[\boldsymbol{p} - \left(\boldsymbol{M+C+K} \right)\begin{pmatrix}
    \boldsymbol{0}\\
    \boldsymbol{A}_{ss}^{-1} \boldsymbol{c}
\end{pmatrix}\right]
$$

## 刚性隔板约束

主节点的面内位移 $u_{m,x}, u_{m,y}, r_{m, z}$
任意从节点的面内位移 $u_{i,x}, u_{i,y}, r_{i, z}$
任意从节点与主节点的距离 $\Delta x_i, \Delta y_i$

$$
\begin{array}{ccl}
    u_{i,x} &=& u_{m,x} - r_{m, z}*\Delta y_i\\
    u_{i,y} &=& u_{m,y} + r_{m, z}*\Delta x_i\\
    r_{i,z} &=& r_{m, z}\\
\end{array}
$$

$$
T_i = \begin{pmatrix}
   1 & 0 & -\Delta y_i\\
   0 & 1 & \Delta x_i\\
   0 & 0 & 1\\
\end{pmatrix}
$$