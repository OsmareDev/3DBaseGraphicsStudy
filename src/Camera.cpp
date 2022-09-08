#include "camera.h"
#include <Windows.h>

void Void_Camera::SetMatrix() {

    // Projection matrix
    Projection = Matrix::identity(4);
    Projection[0][0] = ((float)window_height / (float)window_width) / std::tan((fov * M_PI / 180)/2);
    Projection[1][1] = 1 / std::tan((fov * M_PI / 180)/2);
    Projection[2][2] = z_far / (z_far - z_near);
    Projection[3][3] = 0;
    Projection[2][3] = -(z_far * z_near) / (z_far - z_near);
    Projection[3][2] = 1;
}

Matrix Homo(Matrix m) {
    m[0][0] /= m[3][0];
    m[1][0] /= m[3][0];
    m[2][0] /= m[3][0];
    m[3][0] /= m[3][0];
    return m;
}

Vec2i Void_Camera::to_viewport(Vec3f v) {
    return { (int)((v.x+1)*0.5f*(float)window_width), (int)((v.y + 1) * 0.5f * (float)window_height) };
}

void Void_Camera::TransformTriangle(Triangle& t) {
    t.a = to_viewport(m2v(Homo(Projection * v2m(t.v1))));
    t.b = to_viewport(m2v(Homo(Projection * v2m(t.v2))));
    t.c = to_viewport(m2v(Homo(Projection * v2m(t.v3))));
}

