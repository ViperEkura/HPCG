#include <cmath>
#include "hpcg.h"

// SpareseMatrix
SpareseMatrix::SpareseMatrix(int rows, int cols) :
    _rows(rows),
    _cols(cols){}

float& SpareseMatrix::operator()(int row, int col){
    int64_t index = (int64_t)(row) * _cols + col;
    return _data[index];
}

float SpareseMatrix::operator()(int row, int col) const{
    int64_t index = (int64_t)(row) * _cols + col;
    auto it = _data.find(index);
    if (it != _data.end()) 
        return it->second;
    
    return 0.0f; 
}

size_t SpareseMatrix::size() const{
    return (size_t)(_rows) * _cols;
}

size_t SpareseMatrix::rows() const
{
    return this->_rows;
}

size_t SpareseMatrix::cols() const{
    return this->_cols;
}

std::map<int64_t, float> SpareseMatrix::fetchData() const
{
    return _data;
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

Vector ConjugateGradientSolver::solve(const SpareseMatrix& A, const Vector& b) const {
    Vector x(b.size());              // 初始化解向量 x
    Vector r = b;                    // 初始残差 r = b - A*x0 (x0为0)
    Vector p = r;                    // 初始搜索方向 p = r
    float r_dot_r = 0.0f;

    // 计算初始残差的范数平方
    for (size_t i = 0; i < r.size(); ++i) {
        r_dot_r += r[i] * r[i];
    }

    for (int iter = 0; iter < maxIter; ++iter) {
        // 计算 Ap = A * p (利用稀疏矩阵特性优化)
        Vector Ap(A.rows());
        std::map<int64_t, float> data = A.fetchData();
        for (const auto& entry : data) {
            int64_t idx = entry.first;
            int i = idx / A.cols();
            int j = idx % A.cols();
            Ap[i] += entry.second * p[j];
        }

        // 计算 p^T * Ap
        float p_dot_Ap = 0.0f;
        for (size_t i = 0; i < p.size(); ++i) {
            p_dot_Ap += p[i] * Ap[i];
        }

        if (p_dot_Ap == 0.0f) break;  // 防止除以零

        // 计算步长 alpha
        float alpha = r_dot_r / p_dot_Ap;

        // 更新解 x 和残差 r
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        // 计算新残差的范数平方
        float new_r_dot_r = 0.0f;
        for (size_t i = 0; i < r.size(); ++i) {
            new_r_dot_r += r[i] * r[i];
        }

        // 检查收敛条件
        if (std::sqrt(new_r_dot_r) < tolerance) {
            break;
        }

        // 计算 beta
        float beta = new_r_dot_r / r_dot_r;

        // 更新搜索方向 p
        for (size_t i = 0; i < p.size(); ++i) {
            p[i] = r[i] + beta * p[i];
        }

        r_dot_r = new_r_dot_r;
    }

    return x;
}