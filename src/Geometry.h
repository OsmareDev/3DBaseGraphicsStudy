#ifndef __GEOMETRY__
#define __GEOMETRY__

#pragma once

#include <cmath>
#include <assert.h>
#include <string>
#include <vector>
#include "Crono.h"

// se crean las definiciones aqui porque no las alacanzaba el cpp
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define M_PI 3.14159265358979323846
#define toRad(ang) (ang * M_PI / 180)

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
	t& operator[](const int i) { return i <= 0 ? x : y; }
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

	t& operator[](const int i) { return i <= 0 ? x : (i == 1 ? y : z); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;


// MATRIX ///////////////////

const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float> > m;
public:
	int rows, cols;
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	static Matrix identity(const int& dimensions);
	static Matrix RotX(const float& ang);
	static Matrix RotY(const float& ang);
	static Matrix RotZ(const float& ang);

	std::vector<float>& operator[](const int& i);
	Matrix operator*(const Matrix& a);
	Matrix transpose();
	Matrix inverse();
};


// TRIANGLE ///////////////////

struct BoundingBox {
	Vec2i p_max, p_min;
};

struct Triangle {
	union {
		struct { Vec3f v1, v2, v3; };
		Vec3f vertex[3];
	};
	union {
		struct { Vec2i a, b, c; };
		Vec2i screen_points[3];
	};

	Triangle(Vec2i p1, Vec2i p2, Vec2i p3) : screen_points{ p1,p2,p3 } {}
	Triangle(Vec3f p1, Vec3f p2, Vec3f p3) : vertex{ p1,p2,p3 } {}

	//Triangle(Vec2i p1, Vec2i p2, Vec2i p3, const uint32_t color) : screen_points{ p1,p2,p3 } { Draw(color); }

	inline BoundingBox GetBoundingBox() {
		return { {max(max(a.x, b.x), c.x), max(max(a.y, b.y), c.y)}, {min(min(a.x, b.x), c.x), min(min(a.y, b.y),c.y)} };
	}

	inline Vec2i GetV1() const { return { b.x - a.x, b.y - a.y }; }
	inline Vec2i GetV2() const { return { c.x - b.x, c.y - b.y }; }
	inline Vec2i GetV3() const { return { a.x - c.x, a.y - c.y }; }
	inline Vec2i GetAP(Vec2i P) const { return { P.x - a.x, P.y - a.y }; }
	inline Vec2i GetBP(Vec2i P) const { return { P.x - b.x, P.y - b.y }; }
	inline Vec2i GetCP(Vec2i P) const { return { P.x - c.x, P.y - c.y }; }

	inline bool isInside(const Vec2i& p) const {
		bool check = true;

		check &= ((GetV1().cross(GetAP(p))) >= 0.f);
		check &= ((GetV2().cross(GetBP(p))) >= 0.f);
		check &= ((GetV3().cross(GetCP(p))) >= 0.f);

		return check;
	}

	inline Vec3f& operator[](const int i) { return i <= 0 ? v1 : (i == 1 ? v2 : v3); }

	void RotateX(const float& ang);
	void RotateY(const float& ang);
	void RotateZ(const float& ang);
};


// LINE /////////////////////////

struct Line {
	Vec2i a, b;

	Line() : a(), b() {}
	Line(Vec2i p1, Vec2i p2) : a(p1), b(p2) {}
};


Vec3f m2v(Matrix m);
Matrix v2m(Vec3f v);

#endif //__GEOMETRY__