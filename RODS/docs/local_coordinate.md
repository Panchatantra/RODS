# 局部坐标系及几何变换

## 二维

* X-Z 平面
* 两个节点为 $I(x_i, z_i)$ 和 $J(x_j, z_j)$
* 定义
$$ \Delta x = x_j - x_i \quad \Delta z = z_j - z_i $$
* 单元长度为\
$$L = \sqrt{\Delta x^2 + \Delta z^2}$$
* 以向量 $ \vec{IJ} $ 为局部 X 轴，则局部 X 轴的单位向量为
$$ \mathbf {\overline i} = \left( \frac{\Delta x}{L}, \frac{\Delta z}{L} \right)^T$$
* 局部 Y 轴与 X 轴垂直，其单位向量为
$$ \mathbf {\overline j} = \left( -\frac{\Delta z}{L}, \frac{\Delta x}{L} \right)^T$$

* 整体坐标轴的单位向量
$$ 
\begin{aligned}
\mathbf {i} = \left( 1, 0 \right)^T \\
\mathbf {j} = \left( 0, 1 \right)^T 
\end{aligned}
$$

* 局部坐标轴的方向余弦

$$ 
\begin{aligned}
\cos \left< \mathbf {\overline i}, \mathbf {i} \right> &= \mathbf {\overline i} \cdot \mathbf {i} = \frac{\Delta x}{L} \\
\cos \left< \mathbf {\overline i}, \mathbf {j} \right> &= \mathbf {\overline i} \cdot \mathbf {j} = \frac{\Delta z}{L} \\
\cos \left< \mathbf {\overline j}, \mathbf {i} \right> &= \mathbf {\overline j} \cdot \mathbf {i} = -\frac{\Delta z}{L} \\
\cos \left< \mathbf {\overline j}, \mathbf {j} \right> &= \mathbf {\overline j} \cdot \mathbf {j} = \frac{\Delta x}{L}
\end{aligned}
$$

* 坐标转换矩阵
$$ \mathbf{\lambda} = \left[ \begin{aligned}
\cos \left< \mathbf {\overline i}, \mathbf {i} \right> & \cos \left< \mathbf {\overline i}, \mathbf {j} \right> \\
\cos \left< \mathbf {\overline j}, \mathbf {i} \right> & \cos \left< \mathbf {\overline j}, \mathbf {j} \right>
\end{aligned} \right]
= \frac{1}{L} \left[ \begin{aligned}
\Delta x \quad & \Delta z \\
-\Delta z \quad & \Delta x
\end{aligned} \right]
$$

## 三维

* 两个节点为 $I(x_i, y_i, z_i)$ 和 $J(x_j, y_j, z_j)$
* 定义
$$ \begin{aligned}
\Delta x &= x_j - x_i \\
\Delta y &= y_j - y_i \\
\Delta z &= z_j - z_i \\
\end{aligned} 
 $$
* 单元长度为\
$$L = \sqrt{\Delta x^2 + \Delta y^2 + \Delta z^2}$$