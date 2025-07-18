#include <iostream>
#include <random>
#include <chrono>
#include <cmath> 
#include "hpcg.h"

struct Coord3D {
    int i, j, k;
};


void generate_system(int grid_size, SpareseMatrix& A, Vector& b, std::vector<Coord3D>& coords) {
    const unsigned int total_nodes = grid_size * grid_size * grid_size;
    A = SpareseMatrix(total_nodes, total_nodes);
    b = Vector(total_nodes);
    
    coords.clear();
    coords.reserve(total_nodes);
    
    std::vector<unsigned int> row_indices;
    std::vector<unsigned int> col_indices;
    std::vector<float> values;
    row_indices.reserve(total_nodes * 7);
    col_indices.reserve(total_nodes * 7);
    values.reserve(total_nodes * 7);
    const float L = static_cast<float>(grid_size - 1);  

    for (int k = 0; k < grid_size; ++k) {
        for (int j = 0; j < grid_size; ++j) {
            for (int i = 0; i < grid_size; ++i) {
                coords.push_back({i, j, k});
                const unsigned int node_idx = k * grid_size * grid_size + j * grid_size + i;
                
                if (i == 0 || i == grid_size-1 || 
                    j == 0 || j == grid_size-1 || 
                    k == 0 || k == grid_size-1) 
                {
                    row_indices.push_back(node_idx);
                    col_indices.push_back(node_idx);
                    values.push_back(1.0f);
                    b.data[node_idx] = 1.0f;
                }
                else 
                {
                    row_indices.push_back(node_idx);
                    col_indices.push_back(node_idx);
                    values.push_back(-6.0f);
                    
                    const int neighbors[6][3] = 
                    {
                        {i-1, j, k}, {i+1, j, k},
                        {i, j-1, k}, {i, j+1, k},
                        {i, j, k-1}, {i, j, k+1}
                    };
                    
                    for (int n = 0; n < 6; ++n) 
                    {
                        const int ni = neighbors[n][0];
                        const int nj = neighbors[n][1];
                        const int nk = neighbors[n][2];
                        const unsigned int neighbor_idx = nk * grid_size * grid_size + nj * grid_size + ni;
                        
                        row_indices.push_back(node_idx);
                        col_indices.push_back(neighbor_idx);
                        values.push_back(1.0f);
                    }
                    
                    const float x = static_cast<float>(i) / L;
                    const float y = static_cast<float>(j) / L;
                    const float z = static_cast<float>(k) / L;
                    const float sin_term = sin(M_PI * x) * sin(M_PI * y) * sin(M_PI * z);
                    const float f_val = (3.0f * M_PI * M_PI) / (L * L) * sin_term;
                    b.data[node_idx] = f_val;
                }
            }
        }
    }

    A.constructFromTriplets(
        static_cast<unsigned int>(values.size()),
        row_indices.data(),
        col_indices.data(),
        values.data()
    );
}

int main() {
    const int N = 25; 
    const unsigned long long total_nodes = static_cast<unsigned long long>(N) * N * N;
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<Coord3D> coords;
    SpareseMatrix A;
    Vector b;
    generate_system(N, A, b, coords);
    
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