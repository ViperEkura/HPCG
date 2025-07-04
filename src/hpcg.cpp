#include "hpcg.h"

// SpareseMatrix
SpareseMatrix::SpareseMatrix(int rows, int cols) :
    rows(rows),
    cols(cols){}

float& SpareseMatrix::operator()(int row, int col){
    int64_t index = (int64_t)(row) * cols + col;
    return data[index];
}

float SpareseMatrix::operator()(int row, int col) const{
    int64_t index = (int64_t)(row) * cols + col;
    auto it = data.find(index);
    if (it != data.end()) 
        return it->second;
    
    return 0.0f; 
}

size_t SpareseMatrix::size() const{
    return (size_t)(rows) * cols;
}

// Vector
Vector::Vector(int size) :
    data(std::vector<float>(size, 0.0f)){}

float& Vector::operator[](int index){
    return data[index];
}
float Vector::operator[](int index) const {
    return data[index];
}

size_t Vector::size() const {
    return data.size();
}

//ConjugateGradientSolver

ConjugateGradientSolver::ConjugateGradientSolver(float tolerance, int maxIter) :
    maxIter(maxIter),
    tolerance(tolerance){}

Vector ConjugateGradientSolver::solve(const SpareseMatrix& A, const Vector& b) const{
     Vector x(b.size());
}
