#include <cmath>
#include "hpcg.h"


//ConjugateGradientSolver
ConjugateGradientSolver::ConjugateGradientSolver(float tolerance, int maxIter) :maxIter(maxIter), tolerance(tolerance){}

Vector ConjugateGradientSolver::solve(const SpareseMatrix& A, const Vector& b) const {
    // return x;
}