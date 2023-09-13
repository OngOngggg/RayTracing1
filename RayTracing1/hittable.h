#pragma once
#include"ray.h"

class material;

class hit_record
{
public:
	point3 p; //（交）点
	vec3 normal;//这个是（法）向量
	double t;
	bool front_face;
	shared_ptr<material> mat_ptr;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.getDirection(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	//传入光线信息，两个t值范围，以及交点的详细信息
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
};