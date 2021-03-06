# 开发工具

## 编译器

### Windows

二选一

* Visual Studio 2017/2019 C++开发套装
    * Reshaper C++插件（建议）

* Mingw-w64-GCC
    * MSYS2 环境（建议）

### Linux

* GNU GCC

## 文本编辑器

* Sublime Text 3
* Visual Studio Code

## 版本控制

* git

## 注释生成文档

* doxygen

## 文档撰写

* Markdown（首选）
* LaTeX 或 TeXmacs（公式较多的理论文档）
* pandoc（文档格式转换）
* graphviz（流程图、UML图）
* gnuplot（曲线图绘制）

# 依赖库

* C++ 矩阵运算库 armadillo
* 高性能矩阵运算库（二选一）
    * Intel MKL
        * 需要安装 Intel Parallel Studio
    * OpenBLAS

# 编译过程

1. 启动命令行窗口，建立开发文件夹，例如
> mkdir  ~/Develop
2. 下载 armadillo，解压至开发文件夹
3. 用 git 获取 RODS 最新代码
> git clone https://e.coding.net/dynamics/RODS.git
4. 编译
    * 若使用 Visual Studio：打开RODS.sln，点击生成解决方案即可。
    * 若使用 GCC（Linux 或 MSYS2 环境）：
        > cd RODS && make -j n (n is your CPU number)