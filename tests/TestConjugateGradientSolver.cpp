#include <iostream>
#include "hpcg.h"

int main() {
    // 1. 创建稀疏矩阵
    SpareseMatrix A(3, 3);
    A(0, 0) = 2.0f;  // 对角矩阵
    A(1, 1) = 2.0f;
    A(2, 2) = 2.0f;

    // 2. 创建右侧向量 b
    Vector b(3);
    b.data[0] = 1.0f;
    b.data[1] = 2.0f;
    b.data[2] = 3.0f;

    // 3. 初始化求解器
    ConjugateGradientSolver solver(1e-5f, 1000);

    // 4. 求解 Ax = b
    Vector x = solver.solve(A, b);

    // 5. 输出结果
    std::cout << "Solution x:" << std::endl;
    for (int i = 0; i < x.size; ++i) {
        std::cout << "x[" << i << "] = " << x.data[i] << std::endl;
    }

    return 0;
}