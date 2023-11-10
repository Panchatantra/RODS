# RODS Interface

RODS 的多语言接口。计划支持 Python、Matlab、Julia、Lua。

## Python

### 实现原理

用 ctypes 调用 RODS 的动态链接库。

接口定义文件为 RODS.py


## Matlab

### 实现原理

用 loadlibrary 函数调用 RODS 的动态链接库。

## Julia

### 实现原理

用 @ccall宏或 ccall 函数调用 RODS 的动态链接库。

## Lua

编译成支持 Lua 语言代码的可执行文件。