#pragma once

#include"hittable.h"
#include"sphere.h"

class sphere :public hittable {
public:
	sphere() { }
	sphere(point3 cen, double r) :center(cen), radius(r) { }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
public:
	point3 center;
	double radius;
};
//ÖØĞ´
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.getOrigin() - center;
	//Ì×¹«Ê½
	double a = dot(r.getDirection(), r.getDirection());
	double b = dot(r.getDirection(), oc);
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - a * c;

	if (discriminant > 0)
	{
		double root = sqrt(discriminant);

		double t = (-b - root) / a;
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}

		double t = (-b + root) / a;
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}
	}
	return false;
}
