#include <vector>
#include <algorithm>
#include "hpcg.h"

// SparseMatrix
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

SparseMatrix::SparseMatrix()
{
    this->rows = 0;
    this->cols = 0;
    this->row_ptr = nullptr;
    this->col_indices = nullptr;
    this->values = nullptr;
}

SparseMatrix::SparseMatrix(const SparseMatrix &other)
{
    if(this != &other){
        this->rows = other.rows;
        this->cols = other.cols;
        this->nnz = other.nnz;
        if(this->row_ptr != nullptr) delete[] this->row_ptr;
        if(this->col_indices != nullptr) delete[] this->col_indices;
        if(this->values != nullptr) delete[] this->values;

        this->row_ptr = new unsigned int[rows + 1];
        this->col_indices = new unsigned int[rows * cols];
        this->values = new float[rows * cols];

        std::copy(other.row_ptr, other.row_ptr + rows + 1, this->row_ptr);
        std::copy(other.col_indices, other.col_indices + rows * cols, this->col_indices);
        std::copy(other.values, other.values + rows * cols, this->values);
    }

}

SparseMatrix::SparseMatrix(unsigned int rows, unsigned int cols) 
    : rows(rows), cols(cols), nnz(0), row_ptr(nullptr), col_indices(nullptr), values(nullptr)
{
    // std::fill(row_ptr, row_ptr + rows + 1, 0);
}


SparseMatrix::~SparseMatrix() {
    rows = cols = nnz = 0;
    if(row_ptr != nullptr) delete[] row_ptr;
    if(col_indices != nullptr) delete[] col_indices;
    if(values != nullptr) delete[] values;
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix &other)
{
    if(this == &other) return *this;

    this->rows = other.rows;
    this->cols = other.cols;
    this->nnz = other.nnz;
    if(this->row_ptr != nullptr) delete[] this->row_ptr;
    if(this->col_indices != nullptr) delete[] this->col_indices;
    if(this->values != nullptr) delete[] this->values;

    this->row_ptr = new unsigned int[rows + 1];
    this->col_indices = new unsigned int[rows * cols];
    this->values = new float[rows * cols];

    std::copy(other.row_ptr, other.row_ptr + rows + 1, this->row_ptr);
    std::copy(other.col_indices, other.col_indices + rows * cols, this->col_indices);
    std::copy(other.values, other.values + rows * cols, this->values);
    
    return *this;
}

void SparseMatrix::constructFromTriplets(
    unsigned int nnz,
    unsigned int *row_indices,
    unsigned int *col_indices,
    float *values)
{
    this->nnz = nnz;
    
    std::vector<Triplet> triplets(nnz);
    for (int i = 0; i < nnz; ++i) triplets[i] = {row_indices[i], col_indices[i], values[i]};
    std::sort(triplets.begin(), triplets.end(), TripletCompare());

    row_ptr = new unsigned int[rows + 1];
    std::fill(row_ptr, row_ptr + rows + 1, 0);
    for (const Triplet& t : triplets) row_ptr[t.row + 1]++;
    for (unsigned int i = 1; i <= rows; ++i) row_ptr[i] += row_ptr[i - 1];
    
    this->col_indices = new unsigned int[nnz];
    this->values = new float[nnz];

    for (unsigned int i = 0; i < nnz; ++i) {
        this->col_indices[i] = triplets[i].col;
        this->values[i] = triplets[i].value;
    }
}

