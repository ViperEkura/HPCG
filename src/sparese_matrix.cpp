#include "hpcg.h"

// SpareseMatrix
SpareseMatrix::SpareseMatrix(int rows, int cols) :rows(rows), cols(cols){}

float &SpareseMatrix::operator()(int row, int col)
{
    int index = row * cols + col;
    return data[index];
}

float SpareseMatrix::operator()(int row, int col) const {
    int index = row * cols + col;
    auto it = data.find(index);
    if (it != data.end()) return it->second;
    return 0.0f;
}

