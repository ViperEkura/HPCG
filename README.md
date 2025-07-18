## HPCG

### 1. 项目概述
HPCG (High Performance Conjugate Gradient) 是一个实现共轭梯度法 (Conjugate Gradient, CG) 求解大型稀疏线性方程组的项目。共轭梯度法是一种迭代方法，尤其适合求解对称正定 (SPD) 稀疏矩阵系统，常见于科学计算和工程领域。

### 2. 关键特性
- 稀疏矩阵支持：使用 std::map 存储非零元素，适合非结构化稀疏矩阵
- 并行计算：通过 OpenMP 实现关键运算的并行化
- 模块化设计：分离矩阵、向量和求解器实现，便于扩展
