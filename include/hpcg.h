#include <map>
#include <vector>

class SpareseMatrix {
private:
    int _rows;
    int _cols;
    std::map<int64_t, float> _data;

public:
    SpareseMatrix(int rows, int cols);
    float& operator()(int row, int col);
    float operator()(int row, int col) const;
    size_t size() const;
    size_t rows() const;
    size_t cols() const;
    std::map<int64_t, float> fetchData() const;
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

