#include "Geometry.h"

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            E[i][j] = (i == j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
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

void Line::bresenham(const uint32_t& color) {
    int x_ini = a.x, x_end = b.x;
    int y_ini = a.y, y_end = b.y;

    if (sunderCoen(x_ini, y_ini, x_end, y_end))
    {
        // variable que sirve para saber si es mas vertical la cuesta
        bool _steep = false;

        // comprobamos si es mas vertical que horizontal
        // en caso de serlo se debe iterar a traves de las y por lo que se cambian
        if (std::abs(x_ini - x_end) < std::abs(y_ini - y_end)) {
            std::swap(x_ini, y_ini);
            std::swap(x_end, y_end);
            _steep = true;
        }
        // comprobamos que se dibuja de izquierda a derecha para evitar problemas
        if (x_ini > x_end) {
            std::swap(x_ini, x_end);
            std::swap(y_ini, y_end);
        }

        int _dx = x_end - x_ini;
        int _dy = y_end - y_ini;

        int _pk = std::abs(_dy) * 2;
        int _pnext = 0;

        int _y = y_ini;
        for (int x = x_ini; x <= x_end; x++) {
            if (_steep) {
                Window::GetInstance().PaintPixel(_y, x, color);
            }
            else {
                Window::GetInstance().PaintPixel(x, _y, color);
            }

            _pnext += _pk;
            if (_pnext > _dx) {
                _y += (y_end > y_ini ? 1 : -1);
                _pnext -= _dx * 2;
            }
        }
    }
}

void Line::dda(const uint32_t& color)
{
    int x_ini = a.x, x_end = b.x;
    int y_ini = a.y, y_end = b.y;

    if (sunderCoen(x_ini, y_ini, x_end, y_end))
    {
        // variable que sirve para saber si es mas vertical la cuesta
        bool _steep = false;

        // comprobamos si es mas vertical que horizontal
        // en caso de serlo se debe iterar a traves de las y por lo que se cambian
        if (std::abs(x_ini - x_end) < std::abs(y_ini - y_end)) {
            std::swap(x_ini, y_ini);
            std::swap(x_end, y_end);
            _steep = true;
        }
        // comprobamos que se dibuja de izquierda a derecha para evitar problemas
        if (x_ini > x_end) {
            std::swap(x_ini, x_end);
            std::swap(y_ini, y_end);
        }

        if (x_ini - x_end == 0)
            return;

        float _m = (float)(y_end - y_ini) / (float)(x_end - x_ini);

        float _y = y_ini;

        for (int x = x_ini; x <= x_end; x++) {
            _y += _m;

            if (_steep) {
                Window::GetInstance().PaintPixel((int)_y, x, color);
            }
            else {
                Window::GetInstance().PaintPixel(x, (int)_y, color);
            }
        }
    }
}

bool Line::sunderCoen(int& x0, int& y0, int& x1, int& y1)
{
    // Computamos la region para ambos puntos de la recta
    int code1 = compute(x0, y0);
    int code2 = compute(x1, y1);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true) {
        // si ambos puntos son 0 estan dentro del rectangulo
        if ((code1 == 0) && (code2 == 0)) {
            accept = true;
            break;
        }
        // si ambos puntos estan fuera pero en la misma region damos por hecho que estan fuera
        else if (code1 & code2) {
            break;
        }
        else {
            int code_out;
            double x, y;

            // Cogemos uno de los puntos que este fuera todavia
            code_out = (code1 != 0) ? code1 : code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            // 8 = 1000
            if (code_out & 8) {
                // point is above the clip rectangle
                x = x0 + (x1 - x0) * (Window::GetInstance().GetHeight() - 1 - y0) / (y1 - y0);
                y = Window::GetInstance().GetHeight() - 1;
            }
            // 4 = 0100
            else if (code_out & 4) {
                // point is below the rectangle
                x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
                y = 0;
            }
            // 2 = 0010
            else if (code_out & 2) {
                // point is to the right of rectangle
                y = y0 + (y1 - y0) * (Window::GetInstance().GetWidth() - 1 - x0) / (x1 - x0);
                x = Window::GetInstance().GetWidth() - 1;
            }
            // 1 = 0001
            else if (code_out & 1) {
                // point is to the left of rectangle
                y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
                x = 0;
            }

            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1) {
                x0 = x;
                y0 = y;
                code1 = compute(x0, y0);
            }
            else {
                x1 = x;
                y1 = y;
                code2 = compute(x1, y1);
            }
        }
    }

    return accept;
}

int Line::compute(const int& x, const int& y)
{
    // 0000 es que esta dentro
    int code = 0000;

    // 1000 es que esta por encima
    if (y > Window::GetInstance().GetHeight() - 1)
        code |= 8;

    // abajo
    if (y < 0)
        code |= 4;

    // derecha
    if (x > Window::GetInstance().GetWidth() - 1)
        code |= 2;

    //izquierda
    if (x < 0)
        code |= 1;

    return code;
}