#pragma once

#include"hittable.h"
#include"vec3.h"

class sphere :public hittable {
public:
	sphere() { }
	sphere(point3 cen, double r,shared_ptr<material> m ) :
		center(cen), radius(r), mat_ptr(m) {}
	//virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
		vec3 oc = r.getOrigin() - center;

		double a = dot(r.getDirection(), r.getDirection());
		double double_b = dot(r.getDirection(), oc);
		double c = dot(oc, oc) - radius * radius;
		double discriminant = double_b * double_b - a * c;

		if (discriminant)
		{
			double root = sqrt(discriminant);

			double t = (-double_b - root) / a;
			if (t > t_min && t < t_max)
			{
				rec.t = t;
				rec.p = r.at(t);
				rec.normal = (rec.p - center) / radius;
				rec.set_face_normal(r, rec.normal);
				rec.mat_ptr = mat_ptr;
				return true;
			}

			t = (-double_b + root) / a;
			if (t > t_min && t < t_max)
			{
				rec.t = t;
				rec.p = r.at(t);
				rec.normal = (rec.p - center) / radius;
				rec.set_face_normal(r, rec.normal);
				rec.mat_ptr = mat_ptr;
				return true;
			}

			return false;
		}
	}

public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

