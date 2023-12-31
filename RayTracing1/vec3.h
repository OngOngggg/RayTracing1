#pragma once

#include<cmath>
#include<iostream>
using std::sqrt;

inline double random_double();
inline double random_double(double min, double max);

class vec3 {
public:

	vec3() :e{ 0,0,0 } { }
	vec3(double e0) :e{ e0,e0,e0 } {};
	vec3(double e0, double e1, double e2) :e{ e0,e1,e2 } {};

	double x() const { return e[0];}
	double y() const { return e[1];}
	double z() const { return e[2];}

	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3 operator-() const { 
		return vec3(-e[0], -e[1], -e[2]); 
	}

	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	double length() const {
		return sqrt(length_squared());
	}
	void write_color(std::ostream& out) {
		out << static_cast<int>(255.999 * e[0]) << ' '
			<< static_cast<int>(255.999 * e[1]) << ' '
			<< static_cast<int>(255.999 * e[2]) << '\n';
	}
	double clamp(double x, double min, double max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	void write_color(std::ostream& out, int samples_per_pixel) {
		double scale = 1.0 / samples_per_pixel;
		double r = e[0];
		double g = e[1];
		double b = e[2];
		r = sqrt(r * scale);
		g = sqrt(g*scale);
		b = sqrt(b*scale);
		out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

	}

	static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

public:
	double e[3];
};

vec3 random_in_unit_sphere()
{
	while (1)
	{
		vec3 p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {
	double a = random_double(0, 2 * 3.1415926535897932385);
	double z = random_double(-1, 1);
	double r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		        u.e[2] * v.e[0] - u.e[0] * v.e[2],
		        u.e[0] * v.e[1] - u.e[1] * v.e[0]
	            );
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

//返回折射光线，n是切线方向向量，etai_over_etat折射率
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	//cos值
	auto cos_theta = dot(-uv, n);
	//折射后的光线垂直分量
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	//折射后的光线水平分量
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}
