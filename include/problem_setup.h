#ifndef PROBLEM_SETUP_H
#define PROBLEM_SETUP_H

#include "hpcg.h"

struct BoundaryType
{

};


class ProblemSetup {
private:
    int nx, ny, nz;
    float tolerance;
    int maxIter;
    BoundaryType boundaryType;

public:
    ProblemSetup(int nx, int ny, int nz);
    
    SpareseMatrix generateMatrix() const;
    Vector generateRHS() const;
    void configureSolver(ConjugateGradientSolver& solver) const;
};

#endif