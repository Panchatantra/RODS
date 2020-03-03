# 单元几何转换

[TOC]

```flow
base=>operation: 单元基本变形（向量）
local=>operation: 单元局部变形（向量）
local_coord=>operation: 局部坐标系变形（向量）
global_coord=>operation: 整体坐标系变形（向量）

base->local->local_coord->global_coord
```

## Link类单元

* 仅考虑轴向变形

### 二维

* 整体坐标系变形向量：
$$\mathbf{u} = \left(\begin{matrix}
u_1 \\
v_1 \\
u_2 \\
v_2 \\
\end{matrix}\right)$$

* 局部坐标系变形向量：
$$\mathbf{\hat u} = 
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\hat u_2 \\
\hat v_2 \\
\end{matrix}\right) = 
\left[\begin{matrix}
l_{x\hat{x}} & l_{x\hat{z}} & 0 & 0 \\
l_{z\hat{x}} & l_{z\hat{z}} & 0 & 0 \\
0 & 0 & l_{x\hat{x}} & l_{x\hat{z}} \\
0 & 0 & l_{z\hat{x}} & l_{z\hat{z}} \\
\end{matrix}\right]
\left(\begin{matrix}
u_1 \\
v_1 \\
u_2 \\
v_2 \\
\end{matrix}\right)=
\mathbf{T_g}\mathbf{u}
$$
其中，$l_{u \hat v}$ 意为整体 $u$ 轴单位向量和局部 $\hat v$ 轴单位向量的点乘积。

* 单元局部变形向量：
$$\mathbf{\hat u_e} = \left(\begin{matrix}
\hat u_1\\
\hat u_2
\end{matrix}\right)=
\left[\begin{matrix}
1 & 0 & 0 & 0 \\
0 & 0 & 1 & 0
\end{matrix}\right]
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\hat u_2 \\
\hat v_2 \\
\end{matrix}\right)=
\mathbf{T_2}\mathbf{\hat u}
$$

* 单元基本变形：
$$\Delta u = 
\left[\begin{matrix}
-1 \quad 1
\end{matrix}\right]
\left(\begin{matrix}
\hat u_1\\
\hat u_2
\end{matrix}\right)=
\mathbf{T_1}\mathbf{\hat u_e}
$$

* 单元基本变形与整体坐标系变形的关系：
$$
\Delta u = \mathbf{T_1 T_2 T_g u} =
\mathbf{Tu}
$$
其中
$$
\mathbf{T} = \begin{bmatrix}
-l_{x\hat{x}} & -l_{x\hat{z}} &
 l_{x\hat{x}} &  l_{x\hat{z}}
\end{bmatrix}
$$

### 三维

* 整体坐标系变形向量：
$$\mathbf{u} = \left(\begin{matrix}
u_1 \\
v_1 \\
w_1 \\
u_2 \\
u_2 \\
w_2 \\
\end{matrix}\right)$$

* 局部坐标系变形向量：
$$\mathbf{\hat u} = 
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\hat w_1 \\
\hat u_2 \\
\hat v_2 \\
\hat w_2 \\
\end{matrix}\right) = 
\left[\begin{matrix}
l_{x\hat{x}} & l_{x\hat{y}} & l_{x\hat{z}} & 0 & 0 & 0 \\
l_{y\hat{x}} & l_{y\hat{y}} & l_{y\hat{z}} & 0 & 0 & 0 \\
l_{z\hat{x}} & l_{z\hat{y}} & l_{z\hat{z}} & 0 & 0 & 0 \\
0 & 0 & 0 & l_{x\hat{x}} & l_{x\hat{y}} & l_{x\hat{z}} \\
0 & 0 & 0 & l_{y\hat{x}} & l_{y\hat{y}} & l_{y\hat{z}} \\
0 & 0 & 0 & l_{z\hat{x}} & l_{z\hat{y}} & l_{z\hat{z}} \\
\end{matrix}\right]
\left(\begin{matrix}
u_1 \\
v_1 \\
w_1 \\
u_2 \\
v_2 \\
w_2 \\
\end{matrix}\right)=
\mathbf{T_g}\mathbf{u}
$$
其中，$l_{u \hat v}$ 意为整体 $u$ 轴单位向量和局部 $\hat v$ 轴单位向量的点乘积。

* 单元局部变形向量：
$$\mathbf{\hat u_e} = \left(\begin{matrix}
\hat u_1\\
\hat u_2
\end{matrix}\right)=
\left[\begin{matrix}
1 & 0 & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & 1 & 0 & 0
\end{matrix}\right]
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\hat w_1 \\
\hat u_2 \\
\hat v_2 \\
\hat w_2 \\
\end{matrix}\right)=
\mathbf{T_2}\mathbf{\hat u}
$$

* 单元基本变形：
$$\Delta u = 
\left[\begin{matrix}
-1 \quad 1
\end{matrix}\right]
\left(\begin{matrix}
\hat u_1\\
\hat u_2
\end{matrix}\right)=
\mathbf{T_1}\mathbf{\hat u_e}
$$

* 单元基本变形与整体坐标系变形的关系：
$$
\Delta u = \mathbf{T_1 T_2 T_g u} = \mathbf{Tu}
$$
其中
$$
\mathbf{T} = \begin{bmatrix}
-l_{x\hat{x}} & -l_{x\hat{y}} & -l_{x\hat{z}} &
 l_{x\hat{x}} & l_{x\hat{y}}  &  l_{x\hat{z}}
\end{bmatrix}
$$

## Frame单元

### 二维

* 整体坐标系变形向量：
$$\mathbf{u} = \left(\begin{matrix}
u_1 \\
v_1 \\
\theta_1 \\
u_2 \\
v_2 \\
\theta_2 \\
\end{matrix}\right)$$

* 局部坐标系变形向量：
$$\mathbf{\hat u} = 
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\theta_1 \\
\hat u_2 \\
\hat v_2 \\
\theta_2 \\
\end{matrix}\right) = 
\left[\begin{matrix}
l_{x\hat{x}} & l_{x\hat{z}} & 0 & 0 & 0 & 0 \\
l_{z\hat{x}} & l_{z\hat{z}} & 0 & 0 & 0 & 0 \\
0 & 0 & 1 & 0 & 0 & 0 \\
0 & 0 & 0 & l_{x\hat{x}} & l_{x\hat{z}} & 0 \\
0 & 0 & 0 & l_{z\hat{x}} & l_{z\hat{z}} & 0 \\
0 & 0 & 0 & 0 & 0 & 1 \\
\end{matrix}\right]
\left(\begin{matrix}
u_1 \\
v_1 \\
\theta_1 \\
u_2 \\
v_2 \\
\theta_2 \\
\end{matrix}\right)=
\mathbf{T_g}\mathbf{u}
$$
其中，$l_{u \hat v}$ 意为整体 $u$ 轴单位向量和局部 $\hat v$ 轴单位向量的点乘积。

* 单元局部变形向量：
$$\mathbf{\hat u_e} = \left(\begin{matrix}
\Delta u \\
\Delta v \\
\theta_1 \\
\theta_2 \\
\end{matrix}\right)=
\begin{bmatrix}
-1 & 0  & 0 & 1 & 0 & 0\\
0  & -1 & 0 & 0 & 1 & 0\\
0  & 0  & 1 & 0 & 0 & 0\\
0  & 0  & 0 & 0 & 0 & 1\\
\end{bmatrix}
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\theta_1 \\
\hat u_2 \\
\hat v_2 \\
\theta_2 \\
\end{matrix}\right)=
\mathbf{T_2}\mathbf{\hat u}
$$

* 单元基本变形：
$$\mathbf{\Delta u} = 
\left(\begin{matrix}
\Delta u \\
\overline \theta_1 \\
\overline \theta_2 \\
\end{matrix}\right)=
\left[\begin{matrix}
1 & 0 & 0 & 0 \\
0 & \frac{1}{L} & 1 & 0 \\
0 & \frac{1}{L} & 1 & 0 \\
\end{matrix}\right]
\left(\begin{matrix}
\Delta u \\
\Delta v \\
\theta_1 \\
\theta_2 \\
\end{matrix}\right)=
\mathbf{T_1}\mathbf{\hat u_e}
$$

* 单元基本变形与整体坐标系变形的关系：
$$
\Delta u = \mathbf{T_1 T_2 T_g u} =
\mathbf{Tu}
$$
其中
$$
\mathbf{T} = \begin{bmatrix}
-l_{x\hat{x}} & -l_{x\hat{z}} & 0 & l_{x\hat{x}} & l_{x\hat{z}} & 0\\
-\frac{l_{z\hat{x}}}{L} & -\frac{l_{z\hat{z}}}{L} & 1 &
\frac{l_{z\hat{x}}}{L} & \frac{l_{z\hat{z}}}{L} & 0\\
-\frac{l_{z\hat{x}}}{L} & -\frac{l_{z\hat{z}}}{L} & 0 &
\frac{l_{z\hat{x}}}{L} & \frac{l_{z\hat{z}}}{L} & 1
\end{bmatrix}
$$

### 三维

* 整体坐标系变形向量：
$$\mathbf{u} = \left(\begin{matrix}
u_1 \\
v_1 \\
w_1 \\
\theta_{x1} \\
\theta_{y1} \\
\theta_{z1} \\
u_2 \\
v_2 \\
w_2 \\
\theta_{x2} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right)$$

* 局部坐标系变形向量：
$$\mathbf{\hat u} = 
\left(\begin{matrix}
\hat u_1 \\
\hat v_1 \\
\hat w_1 \\
\theta_{x1} \\
\theta_{y1} \\
\theta_{z1} \\
\hat u_2 \\
\hat v_2 \\
\hat w_2 \\
\theta_{x2} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right) = 
\left[\begin{matrix}
\boldsymbol{\lambda} & \mathbf{O} & \mathbf{O} & \mathbf{O}\\
\mathbf{O} & \boldsymbol{\lambda} & \mathbf{O} & \mathbf{O}\\
\mathbf{O} & \mathbf{O} & \boldsymbol{\lambda} & \mathbf{O}\\
\mathbf{O} & \mathbf{O} & \mathbf{O} & \boldsymbol{\lambda}\\
\end{matrix}\right]
\left(\begin{matrix}
u_1 \\
v_1 \\
w_1 \\
\theta_{x1} \\
\theta_{y1} \\
\theta_{z1} \\
u_2 \\
v_2 \\
w_2 \\
\theta_{x2} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right)=
\mathbf{T_g}\mathbf{u}
$$
其中
$$
\boldsymbol{\lambda} = \begin{bmatrix}
l_{x\hat{x}} & l_{x\hat{y}} & l_{x\hat{z}} \\
l_{y\hat{x}} & l_{y\hat{y}} & l_{y\hat{z}} \\
l_{z\hat{x}} & l_{z\hat{y}} & l_{z\hat{z}} \\
\end{bmatrix}
$$
其中，$l_{u \hat v}$ 意为整体 $u$ 轴单位向量和局部 $\hat v$ 轴单位向量的点乘积。

* 单元局部变形：
$$\mathbf{\Delta u} = 
\left(\begin{matrix}
\Delta u \\
\Delta v \\
\Delta w \\
\Delta \theta_{x} \\
\theta_{y1} \\
\theta_{z1} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right)=
\left[\begin{matrix}
-1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 \\
0 & -1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 \\
0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 \\
0 & 0 & 0 & -1 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 \\
0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 \\
0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 \\
\end{matrix}\right]
\left(\begin{matrix}
u_1 \\
v_1 \\
w_1 \\
\theta_{x1} \\
\theta_{y1} \\
\theta_{z1} \\
u_2 \\
v_2 \\
w_2 \\
\theta_{x2} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right)=
\mathbf{T_1}\mathbf{\hat u_e}
$$

* 单元基本变形：
$$\mathbf{\Delta u} = 
\left(\begin{matrix}
\Delta u \\
\Delta \theta_{x} \\
\overline \theta_{y1} \\
\overline \theta_{y2} \\
\overline \theta_{z1} \\
\overline \theta_{z2} \\
\end{matrix}\right)=
\left[\begin{matrix}
1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & 1 & 0 & 0 & 0 & 0 \\
0 & 0 & \frac{1}{L} & 0 & 1 & 0 & 0 & 0 \\
0 & 0 & \frac{1}{L} & 0 & 0 & 0 & 1 & 0 \\
0 & -\frac{1}{L} & 0 & 0 & 0 & 1 & 0 & 0 \\
0 & -\frac{1}{L} & 0 & 0 & 0 & 0 & 0 & 1 \\
\end{matrix}\right]
\left(\begin{matrix}
\Delta u \\
\Delta v \\
\Delta w \\
\Delta \theta_{x} \\
\theta_{y1} \\
\theta_{z1} \\
\theta_{y2} \\
\theta_{z2} \\
\end{matrix}\right)=
\mathbf{T_1}\mathbf{\hat u_e}
$$
