#include <map>
#include <vector>

class SpareseMatrix {
private:
    int rows;
    int cols;
    std::map<int64_t, float> data;

public:
    SpareseMatrix(int rows, int cols);
    float& operator()(int row, int col);
    float operator()(int row, int col) const;
    size_t size() const;
};

class Vector {
private:
    std::vector<float> data;

public:
    Vector(int size);
    float& operator[](int index);
    float operator[](int index) const;
    size_t size() const;
};

class ConjugateGradientSolver {
private:
    float tolerance;
    int maxIter;

public:
    ConjugateGradientSolver(float tolerance, int maxIter);
    Vector solve(const SpareseMatrix& A, const Vector& b) const;
};


class ProblemSetup{

};

