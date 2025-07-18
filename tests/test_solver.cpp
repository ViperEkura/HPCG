#include <iostream>
#include <random>
#include <chrono>
#include <cmath> 

#include "problem_setup.h"
#include "hpcg.h"

struct Coord3D {
    int i, j, k;
};

std::vector<Coord3D> generateCoords(unsigned int nx, unsigned int ny, unsigned int nz)
{
    std::vector<Coord3D> coords;
    coords.reserve(nx * ny * nz);
    
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                coords.push_back({i, j, k});
            }
        }
    }
    
    return coords;
}

int main() {
    const int N = 25; 
    const unsigned long long total_nodes = static_cast<unsigned long long>(N) * N * N;
    auto start = std::chrono::high_resolution_clock::now();
    

    ProblemSetup setup(N, N, N);
    std::vector<Coord3D> coords = generateCoords(N, N, N);
    SparseMatrix A = setup.generateMatrix();
    Vector b = setup.generateRHS();

    printf("Matrix generated (nnz = %llu)\n", A.nnz);
    printf("Matrix size: %llu x %llu\n", total_nodes, total_nodes);
    
    ConjugateGradientSolver solver(1e-6f, 5000);
    auto solve_start = std::chrono::high_resolution_clock::now();
    Vector x = solver.solve(A, b);
    auto solve_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(solve_end - solve_start);
    printf("Solve completed in %lld ms\n", duration.count());
    
    printf("\nFirst 10 elements of solution (with coordinates):\n");
    for (int idx = 0; idx < 10 && idx < total_nodes; ++idx) {
        const auto& c = coords[idx];
        printf("x[%d]: (%d, %d, %d) = %f\n", idx, c.i, c.j, c.k, x.data[idx]);
    }
    
    printf("\nLast 10 elements of solution (with coordinates):\n");
    int start_idx = (total_nodes >= 10) ? total_nodes - 10 : 0;
    for (int idx = start_idx; idx < total_nodes; ++idx) {
        const auto& c = coords[idx];
        printf("x[%d]: (%d, %d, %d) = %f\n", idx, c.i, c.j, c.k, x.data[idx]);
    }
    
    return 0;
}