#include <vector>
#include <cmath>

#include "hpcg.h"
#include "problem_setup.h"

ProblemSetup::ProblemSetup(int nx, int ny, int nz) 
    : nx(nx), ny(ny), nz(nz){}


SparseMatrix ProblemSetup::generateMatrix() const {
    const unsigned int total_nodes = nx * ny * nz;
    SparseMatrix A(total_nodes, total_nodes);
    
    std::vector<unsigned int> row_indices;
    std::vector<unsigned int> col_indices;
    std::vector<float> values;
    row_indices.reserve(total_nodes * 7);
    col_indices.reserve(total_nodes * 7);
    values.reserve(total_nodes * 7);

    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                const unsigned int node_idx = k * ny * nx + j * nx + i;
                
                if (i == 0 || i == nx - 1 || 
                    j == 0 || j == ny - 1 || 
                    k == 0 || k == nz - 1) 
                {
                    row_indices.push_back(node_idx);
                    col_indices.push_back(node_idx);
                    values.push_back(1.0f);
                }
                else 
                {
                    row_indices.push_back(node_idx);
                    col_indices.push_back(node_idx);
                    values.push_back(-6.0f);
                    
                    const int neighbors[6][3] = {
                        {i-1, j, k}, {i+1, j, k},
                        {i, j-1, k}, {i, j+1, k},
                        {i, j, k-1}, {i, j, k+1}
                    };
                    
                    for (int n = 0; n < 6; ++n) 
                    {
                        const int ni = neighbors[n][0];
                        const int nj = neighbors[n][1];
                        const int nk = neighbors[n][2];
                        const unsigned int neighbor_idx = nk * ny * nx + nj * nx + ni;
                        
                        row_indices.push_back(node_idx);
                        col_indices.push_back(neighbor_idx);
                        values.push_back(1.0f);
                    }
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
    
    return A;
}

Vector ProblemSetup::generateRHS() const {
    const int grid_size = nx;
    const unsigned int total_nodes = grid_size * grid_size * grid_size;
    Vector b(total_nodes);
    
    const float L = static_cast<float>(grid_size - 1);
    const float h = 1.0f / L;

    for (int k = 0; k < grid_size; ++k) {
        for (int j = 0; j < grid_size; ++j) {
            for (int i = 0; i < grid_size; ++i) {
                const unsigned int node_idx = k * grid_size * grid_size + j * grid_size + i;
                
                if (i == 0 || i == grid_size-1 || 
                    j == 0 || j == grid_size-1 || 
                    k == 0 || k == grid_size-1) 
                {
                    b.data[node_idx] = 1.0f;
                }
                else 
                {
                    const float x = static_cast<float>(i) / L;
                    const float y = static_cast<float>(j) / L;
                    const float z = static_cast<float>(k) / L;
                    const float sin_term = sin(M_PI * x) * sin(M_PI * y) * sin(M_PI * z);
                    const float f_val = sin_term * 3.0f * M_PI * M_PI * h * h;
                    b.data[node_idx] = f_val;
                }
            }
        }
    }
    
    return b;
}
