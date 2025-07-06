#include <map>
#include <vector>

struct SpareseMatrix {
    int rows;
    int cols;
    std::map<int, float> data;
    SpareseMatrix(int rows, int cols);

    float& operator()(int row, int col);
    float operator()(int row, int col) const;
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

class ProblemSetup{

};