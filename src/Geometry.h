#ifndef __GEOMETRY__
#define __GEOMETRY__

#pragma once

#include <cmath>
#include <vector>
#include <assert.h>
#include "Window.h"

template <class t> struct Vec2 {
	union {
		struct { t u, v; };
		struct { t x, y; };
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u), v(_v) {}
	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u * f, v * f); }

	inline float cross(const Vec2<t>& V) const { return ( x * V.y - y * V.x ); }
};

template <class t> struct Vec3 {
	union {
		struct { t x, y, z; };
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
	inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
	float norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;


// MATRIX ///////////////////

const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float> > m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	static Matrix identity(int dimensions);
	std::vector<float>& operator[](const int i);
	Matrix operator*(const Matrix& a);
	Matrix transpose();
	Matrix inverse();
};


// TRIANGLE ///////////////////

struct BoundingBox {
	Vec2i p_max, p_min;
};

template <class t> struct Triangle {
	t a, b, c;

	Triangle() : a(0), b(0), c(0) {}
	Triangle(t p1, t p2, t p3) : a(p1), b(p2), c(p3) {}
	Triangle(t p1, t p2, t p3, const uint32_t color) : a(p1), b(p2), c(p3) { Draw(color); }

	inline BoundingBox GetBoundingBox() {
		return { {min(max(max(a.x, b.x), c.x),(Window::GetInstance().GetWidth() - 1)), min(max(max(a.y, b.y), c.y), (Window::GetInstance().GetHeight() - 1))}, {max(min(min(a.x, b.x), c.x), 0), max(min(min(a.y, b.y),c.y),0)} };
	}

	inline t GetV1() const { return { b.x - a.x, b.y - a.y }; }
	inline t GetV2() const { return { c.x - b.x, c.y - b.y }; }
	inline t GetV3() const { return { a.x - c.x, a.y - c.y }; }
	inline t GetAP(t P) const { return { P.x - a.x, P.y - a.y }; }
	inline t GetBP(t P) const { return { P.x - b.x, P.y - b.y }; }
	inline t GetCP(t P) const { return { P.x - c.x, P.y - c.y }; }

	inline bool isInside(const Vec2i& p) {
		bool check = true;

		check &= ((GetV1().cross(GetAP(p))) >= 0.f);
		check &= ((GetV2().cross(GetBP(p))) >= 0.f);
		check &= ((GetV3().cross(GetCP(p))) >= 0.f);

		return check;
	}

	inline void Draw(const uint32_t& color) {
		BoundingBox box = GetBoundingBox();

		bool check = false;

		for (int y = box.p_min.y; y < box.p_max.y; y++)
		{
			for (int x = box.p_min.x; x < box.p_max.x; x++)
			{
				if (isInside({ x,y }))
				{
					check = true;
					Window::GetInstance().PaintPixel(x, y, color);
				}
				else
					if (check)
						break;

			}
			check = false;
		}
	}
};


typedef Triangle<Vec2i>   Triangle2Di;
typedef Triangle<Vec2f>   Triangle2Df;
typedef Triangle<Vec3i>   Triangle3Di;
typedef Triangle<Vec3f>   Triangle3Df;


// hacer una clase linea

struct Line {
	Vec2i a, b;

	Line() : a(), b() {}
	Line(Vec2i p1, Vec2i p2) : a(p1), b(p2) {}
	Line(Vec2i p1, Vec2i p2, const uint32_t color) : a(p1), b(p2) { Draw(color); }

	void bresenham(const uint32_t& color);
	void dda(const uint32_t& color);
	bool sunderCoen(int& x0, int& y0, int& x1, int& y1);
	int compute(const int& x, const int& y);

	void Draw(const uint32_t& color) { bresenham(color); };
};

#endif //__GEOMETRY__