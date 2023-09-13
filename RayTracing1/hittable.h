#pragma once
#include"ray.h"

class material;

class hit_record
{
public:
	point3 p; //��������
	vec3 normal;//����ǣ���������
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
	//���������Ϣ������tֵ��Χ���Լ��������ϸ��Ϣ
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
};