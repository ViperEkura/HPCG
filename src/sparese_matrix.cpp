#include <vector>
#include <algorithm>
#include "hpcg.h"

// SpareseMatrix
struct Triplet {
    unsigned int row;
    unsigned int col;
    float value;
};

struct TripletCompare {
    bool operator()(const Triplet& a, const Triplet& b) const {
        if (a.row != b.row) return a.row < b.row;
        return a.col < b.col;
    }
};

SpareseMatrix::SpareseMatrix()
{
    this->rows = 0;
    this->cols = 0;
    this->row_ptr = nullptr;
    this->col_indices = nullptr;
    this->values = nullptr;
}

SpareseMatrix::SpareseMatrix(unsigned int rows, unsigned int cols) 
{
    this->rows = rows;
    this->cols = cols;
    this->row_ptr = new unsigned int[rows + 1];
    this->col_indices = new unsigned int[rows * cols];
    this->values = new float[rows * cols];
}


SpareseMatrix::~SpareseMatrix() {
    rows = cols = nnz = 0;
    if(row_ptr != nullptr) delete[] row_ptr;
    if(col_indices != nullptr) delete[] col_indices;
    if(values != nullptr) delete[] values;
}

void SpareseMatrix::constructFromTriplets(
    unsigned int nnz, 
    const unsigned int* row_indices, 
    const unsigned int* col_indices, 
    const float* values
)
{
    this->nnz = nnz;
    
    std::vector<Triplet> triplets(nnz);
    for (int i = 0; i < nnz; ++i) triplets[i] = {row_indices[i], col_indices[i], values[i]};
    std::sort(triplets.begin(), triplets.end(), TripletCompare());

    row_ptr = new unsigned int[rows + 1];
    for (const Triplet& t : triplets) row_ptr[t.row + 1]++;
    for (unsigned int i = 1; i <= rows; ++i) row_ptr[i] += row_ptr[i - 1];
    
    this->col_indices = new unsigned int[nnz];
    this->values = new float[nnz];

    for (unsigned int i = 0; i < nnz; ++i) {
        this->col_indices[i] = triplets[i].col;
        this->values[i] = triplets[i].value;
    }
}

