#ifndef HPCG_H
#define HPCG_H

 // CSR Sparse Matrix
struct SpareseMatrix {
    unsigned int rows;
    unsigned int cols;
    unsigned int nnz;
    unsigned int* row_ptr;
    unsigned int* col_indices;
    float* values;
    SpareseMatrix();
    SpareseMatrix(unsigned int rows, unsigned int cols);
    ~SpareseMatrix();
    void constructFromTriplets(unsigned int nnz, 
        unsigned int* row_indices, unsigned int* col_indices, float* values);
};

struct Vector {
    float* data;
    int size;

    Vector(int size);
    Vector(const Vector& other);
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
};

class ConjugateGradientSolver {
private:
    float tolerance;
    int maxIter;
public:
    ConjugateGradientSolver(float tolerance, int maxIter);
    Vector solve(const SpareseMatrix& A, const Vector& b) const;
};

#endif