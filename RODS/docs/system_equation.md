# 系统方程

## 方程的基本形式
### 动力方程
$$
\mathbf{M \ddot{u}}(t) +
\mathbf{C  \dot{u}}(t) +
\mathbf{K_0      u}(t) =
-\mathbf{M_p E \ddot{u}_g}(t)
-\mathbf{q}(t) + \mathbf{Q} + \mathbf{P}(t)
$$

| 符号  |  含义  |
| :---: | ----- |
|$\mathbf{u}(t)$|动力自由度位移向量|
|$\mathbf{\dot u}(t)$|动力自由度速度向量|
|$\mathbf{\ddot u}(t)$|动力自由度加速度向量|
|$\mathbf{M}$|惯性矩阵|
|$\mathbf{C}$|阻尼矩阵|
|$\mathbf{K_0}$|初始线弹性刚度矩阵|
|$\mathbf{M_p}$|物理质量矩阵|
|$\mathbf{E}$|地面运动指示向量|
|$\mathbf{\ddot{u}_g}(t)$|地面运动加速度历程|
|$\mathbf{q}(t)$|非线性力向量|
|$\mathbf{Q}$|静力荷载向量|
|$\mathbf{P}(t)$|动力荷载向量|

### 静力方程

$$
\mathbf{K_0      u}(t) =
-\mathbf{q}(t) + \mathbf{Q} + \mathbf{P}(t)
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