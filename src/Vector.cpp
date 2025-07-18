#include <memory>
#include "hpcg.h"


// Vector
Vector::Vector(int size)
{
    this->size = size;
    this->data = new float[size];
}

Vector::Vector(const Vector& other)
{
    if (this != &other) 
    {
        this->size = other.size;
        this->data = new float[other.size];
        std::copy(other.data, other.data + other.size, this->data);
    }
}

Vector::~Vector()
{
    if (this->data != nullptr)
    {
        delete[] this->data;
        this->data = nullptr;
    }
}

Vector& Vector::operator=(const Vector& other)
{
    if (this == &other) return *this;
    if (this->data != nullptr) delete[] this->data;
    
    this->size = other.size;
    this->data = new float[other.size];
    std::copy(other.data, other.data + other.size, this->data);

    return *this;
}

Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this == &other) return *this; 

    this->size = other.size;
    this->data = other.data;
    other.size = 0;
    other.data = nullptr;

    return *this;
}