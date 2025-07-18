#include <iostream>
#include <random>
#include <chrono>
#include "hpcg.h"

SpareseMatrix generate_large_matrix(int n) {
    std::vector<unsigned int> row_indices;
    std::vector<unsigned int> col_indices;
    std::vector<float> values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.5f, 1.5f);

    for (int i = 0; i < n; ++i) {
        // 对角线元素
        row_indices.push_back(i);
        col_indices.push_back(i);
        values.push_back(4.0f + dist(gen));

        // 上下边元素
        if (i > 0) {
            row_indices.push_back(i);
            col_indices.push_back(i - 1);
            values.push_back(-1.0f);
        }
        if (i < n - 1) {
            row_indices.push_back(i);
            col_indices.push_back(i + 1);
            values.push_back(-1.0f);
        }

        // 跨行元素
        if (i > n / 2) {
            row_indices.push_back(i);
            col_indices.push_back(i - n / 2);
            values.push_back(-0.3f);
        }
        if (i < n - n / 2) {
            row_indices.push_back(i);
            col_indices.push_back(i + n / 2);
            values.push_back(-0.3f);
        }
    }

    SpareseMatrix A(n, n);
    A.constructFromTriplets(
        row_indices.size(), 
        row_indices.data(), 
        col_indices.data(), 
        values.data()
    );
    return A;
}
int main() {
    const int N = 20000;  // 10,000 x 10,000 
    auto start = std::chrono::high_resolution_clock::now();
    

    SpareseMatrix A = generate_large_matrix(N);
    std::cout << "Matrix generated (nnz = " << N * N << ")" << std::endl;
    
    Vector b(N);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        b.data[i] = (i % 10 == 0) ? 1.0f : 0.0f;  // 每10个元素一个脉冲
    }
    
    ConjugateGradientSolver solver(1e-6f, 100);
    auto solve_start = std::chrono::high_resolution_clock::now();
    Vector x = solver.solve(A, b);
    auto solve_end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(solve_end - solve_start);
    std::cout << "Solve completed in " << duration.count() << " ms" << std::endl;
    

    std::cout << "\nFirst 10 elements of solution:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << "x[" << i << "] = " << x.data[i] << std::endl;
    }
    
    std::cout << "\nLast 10 elements of solution:" << std::endl;
    for (int i = N-10; i < N; ++i) {
        std::cout << "x[" << i << "] = " << x.data[i] << std::endl;
    }
    
    return 0;
}