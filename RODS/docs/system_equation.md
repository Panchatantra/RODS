# 系统方程

## 方程的基本形式
### 动力方程
$$
\boldsymbol{M \ddot{u}}(t) +
\boldsymbol{C  \dot{u}}(t) +
\boldsymbol{K_0      u}(t) =
-\boldsymbol{M_p E \ddot{u}_g}(t)
-\boldsymbol{q}(t) + \boldsymbol{Q} + \boldsymbol{p}(t)
$$

| 符号  |  含义  |
| :---: | ----- |
|$\boldsymbol{u}(t)$|动力自由度位移向量|
|$\boldsymbol{\dot u}(t)$|动力自由度速度向量|
|$\boldsymbol{\ddot u}(t)$|动力自由度加速度向量|
|$\boldsymbol{M}$|惯性矩阵|
|$\boldsymbol{C}$|阻尼矩阵|
|$\boldsymbol{K_0}$|初始线弹性刚度矩阵|
|$\boldsymbol{M_p}$|物理质量矩阵|
|$\boldsymbol{E}$|地面运动指示向量|
|$\boldsymbol{\ddot{u}_g}(t)$|地面运动加速度历程|
|$\boldsymbol{q}(t)$|非线性力向量|
|$\boldsymbol{Q}$|静力荷载向量|
|$\boldsymbol{p}(t)$|动力荷载向量|

### 静力方程

$$
\boldsymbol{K_0      u}(t) =
-\boldsymbol{q}(t) + \boldsymbol{Q} + \boldsymbol{p}(t)
$$

## 模态分析

### 实模态
$$
\boldsymbol{K \phi}
 = \omega^2\boldsymbol{M \phi} 
$$

对质量矩阵进行Cholesky分解

$$
\boldsymbol{M} = \boldsymbol{LL}^T
$$

其中，$\boldsymbol{L}$ 为下三角矩阵。

$$
\boldsymbol{K \phi}
 = \omega^2\boldsymbol{LL}^T\boldsymbol{\phi} 
$$

$$
\boldsymbol{L}^{-1}\boldsymbol{K \phi}
 = \omega^2\boldsymbol{L}^T\boldsymbol{\phi} 
$$

$$
\boldsymbol{L}^{-1}\boldsymbol{K} (\boldsymbol{L}^T)^{-1}\boldsymbol{L}^T\boldsymbol{\phi}
 = \omega^2\boldsymbol{L}^T\boldsymbol{\phi} 
$$

令 $\boldsymbol{\varphi} = \boldsymbol{L}^T\boldsymbol{\phi} $

$$
\boldsymbol{L}^{-1}\boldsymbol{K} (\boldsymbol{L}^{-1})^T\boldsymbol{\varphi}
 = \omega^2\boldsymbol{\varphi} 
$$

## 动力方程求解方法

### 隐式方法
#### Newmark-β 法

#### 非线性方程的迭代解法
##### Newton 迭代

### 显式方法
#### 状态空间积分法

#### 中心差分法

## 静力方程求解方法

### 力控制加载
### 位移控制加载
### 弧长法