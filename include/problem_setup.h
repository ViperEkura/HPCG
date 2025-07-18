#ifndef PROBLEM_SETUP_H
#define PROBLEM_SETUP_H

#include "hpcg.h"

struct BoundaryType
{
    // Dirichlet boundary condition flag for each face
    bool left, right, front, back, top, bottom;
    BoundaryType() : left(true), right(true), front(true), back(true), top(true), bottom(true) {}
};

class ProblemSetup {
private:
    int nx, ny, nz;
    float tolerance;
    int maxIter;
    BoundaryType boundaryType;

public:
    ProblemSetup(int nx, int ny, int nz);
    SparseMatrix generateMatrix() const;
    Vector generateRHS() const;
};
#endif