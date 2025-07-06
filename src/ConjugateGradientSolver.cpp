#include <cmath>
#include "hpcg.h"

//ConjugateGradientSolver

float dot(const Vector& a, const Vector& b) {
    float result = 0.0f;
    for (int i = 0; i < a.size; ++i) {
        result += a.data[i] * b.data[i];
    }
    return result;
}

ConjugateGradientSolver::ConjugateGradientSolver(float tolerance, int maxIter) :maxIter(maxIter), tolerance(tolerance){}

Vector ConjugateGradientSolver::solve(const SpareseMatrix& A, const Vector& b) const 
{
    //init x0
    Vector x0(b.size);
    
    //init r0 = b - Ax0
    Vector r0 = b;
    for (int i = 0; i < A.rows; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < A.cols; ++j) {
            sum += A(i, j) * x0.data[j];
        }
        r0.data[i] -= sum;
    }

    //init p0 = r0
    Vector p0 = r0;

    float rsold = dot(r0, r0);
    if (rsold < tolerance) return x0;

    for (int k = 0; k < maxIter; ++k) {
        // Compute alpha_k
        float Ap_dot_p = 0.0f;
        for (int i = 0; i < A.rows; ++i) {
            float sum = 0.0f;
            for (int j = 0; j < A.cols; ++j) {
                sum += A(i, j) * p0.data[j];
            }
            Ap_dot_p += p0.data[i] * sum;
        }
        float alpha = rsold / Ap_dot_p;

        // Update x_k+1 = x_k + alpha_k * p_k
        for (int i = 0; i < x0.size; ++i) {
            x0.data[i] += alpha * p0.data[i];
        }

        // Update r_k+1 = r_k - alpha_k * A * p_k
        for (int i = 0; i < A.rows; ++i) {
            float sum = 0.0f;
            for (int j = 0; j < A.cols; ++j) {
                sum += A(i, j) * p0.data[j];
            }
            r0.data[i] -= alpha * sum;
        }

        // Compute beta_k and update p_k+1
        float rsnew = dot(r0, r0);
        if (sqrt(rsnew) < tolerance) break;
        float beta = rsnew / rsold;
        for (int i = 0; i < p0.size; ++i) {
            p0.data[i] = r0.data[i] + beta * p0.data[i];
        }

        rsold = rsnew;
    }

    return x0;
}