#include <cmath>
#include <omp.h>
#include "hpcg.h"

//ConjugateGradientSolver

float DDOT(const Vector& a, const Vector& b) {
    float result = 0.0f;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < a.size; ++i) 
    {
        result += a.data[i] * b.data[i];
    }
    return result;
}
void WAXPBY(const Vector& a, const Vector& b, Vector& result, float alpha=1, float beta=1)
{
    #pragma omp parallel for
    for (int i = 0; i < result.size; ++i) 
    {
        result.data[i] = alpha * a.data[i] + beta * b.data[i];
    }
}

void SpMV(const SpareseMatrix& A, const Vector& x, Vector& result) {
    std::fill(result.data, result.data + result.size, 0.0f);
    
    #pragma omp parallel for
    for (int row = 0; row < A.rows; ++row) 
    {
        for (int idx = A.row_ptr[row]; idx < A.row_ptr[row + 1]; ++idx) 
        {
            int col = A.col_indices[idx];
            result.data[row] += A.values[idx] * x.data[col];
        }
    }
}

ConjugateGradientSolver::ConjugateGradientSolver(float tolerance, int maxIter)
{
    this->maxIter = maxIter;
    this->tolerance = tolerance;
}

Vector ConjugateGradientSolver::solve(const SpareseMatrix& A, const Vector& b) const {
    // init x
    Vector x(b.size);
    std::fill(x.data, x.data + x.size, 0.0f);

    // init r = b - A*x
    Vector r(b.size);
    Vector Ax(b.size);
    SpMV(A, x, Ax);
    WAXPBY(b, Ax, r, 1.0f, -1.0f);
    
    // init p = r
    Vector p = r;
    
    float rsold = DDOT(r, r);
    if (rsold < tolerance * tolerance) return x;

    Vector Ap(b.size);
    
    // iteration
    for (int iter = 0; iter < maxIter; ++iter) {
        SpMV(A, p, Ap); // Ap = A*p
        float pAp = DDOT(p, Ap);
        if (pAp == 0) break;

        float alpha = rsold / pAp; 
        WAXPBY(p, x, x, alpha, 1.0f); // x += alpha * p
        WAXPBY(r, Ap, r, 1.0f, -alpha); //  r -= alpha * Ap
        float rsnew = DDOT(r, r); 
        if (rsnew < tolerance * tolerance) break; 
         
        float beta = rsnew / rsold; 
        WAXPBY(r, p, p, 1.0f, beta); //  p = r + beta * p
        
        rsold = rsnew;
    }
    
    return x;
}