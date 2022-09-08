#include "Geometry.h"

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(const int& dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            E[i][j] = (i == j ? 1.f : 0.f);
        }
    }
    return E;
}

Matrix Matrix::RotX(const float& ang) {
    Matrix E = Matrix::identity(4);
    E[1][1] = std::cos(ang);
    E[1][2] = std::sin(ang);
    E[2][1] = -std::sin(ang);
    E[2][2] = std::cos(ang);

    return E;
}

Matrix Matrix::RotY(const float& ang) {
    Matrix E = Matrix::identity(4);
    E[0][0] = std::cos(ang);
    E[0][2] = std::sin(ang);
    E[2][0] = -std::sin(ang);
    E[2][2] = std::cos(ang);

    return E;
}

Matrix Matrix::RotZ(const float& ang) {
    Matrix E = Matrix::identity(4);
    E[0][0] = std::cos(ang);
    E[0][1] = std::sin(ang);
    E[1][0] = -std::sin(ang);
    E[1][1] = std::cos(ang);

    return E;
}

std::vector<float>& Matrix::operator[](const int& i) {
    assert(i >= 0 && i < rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                result.m[i][j] += m[i][k] * a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows == cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols * 2);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = m[i][j];
    for (int i = 0; i < rows; i++)
        result[i][i + cols] = 1;
    // first pass
    for (int i = 0; i < rows - 1; i++) {
        // normalize the first row
        for (int j = result.cols - 1; j >= 0; j--)
            result[i][j] /= result[i][i];
        for (int k = i + 1; k < rows; k++) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // normalize the last row
    for (int j = result.cols - 1; j >= rows - 1; j--)
        result[rows - 1][j] /= result[rows - 1][rows - 1];
    // second pass
    for (int i = rows - 1; i > 0; i--) {
        for (int k = i - 1; k >= 0; k--) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            truncate[i][j] = result[i][j + cols];
    return truncate;
}

void Triangle::RotateX(const float& ang) {
    Matrix m = Matrix::RotX(ang);

    v1 = m2v(m * v2m(v1));
    v2 = m2v(m * v2m(v2));
    v3 = m2v(m * v2m(v3));
}

void Triangle::RotateY(const float& ang) {
    Matrix m = Matrix::RotY(ang);

    v1 = m2v(m * v2m(v1));
    v2 = m2v(m * v2m(v2));
    v3 = m2v(m * v2m(v3));
}

void Triangle::RotateZ(const float& ang) {
    Matrix m = Matrix::RotZ(ang);

    v1 = m2v(m * v2m(v1));
    v2 = m2v(m * v2m(v2));
    v3 = m2v(m * v2m(v3));
}

Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0], m[1][0], m[2][0]);
}

Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}