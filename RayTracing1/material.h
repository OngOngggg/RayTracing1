#pragma once

#include"rtweekend.h"
#include"hittable.h"

struct hit_record;

//更加近似的算法，用来算折射率
double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 *= r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

//朗博反射
class lambertian :public material {
public:
	lambertian(const color& a) :albedo(a) { }

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
public:
	color albedo;//反射率
};

//金属材料
class metal :public material {
public:
	metal(const color& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) { }

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.getDirection()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.getDirection(), rec.normal) > 0);
	}
public:
	color albedo;
	//模糊度，因为金属表面太大，反射可以不按正常反射来发生（或许是金属球不是绝对光滑的原因？加这个模糊度用来改变正确的反射光线走向
	//模糊度最大取1
	double fuzz;
};

//玻璃材料
class dielectric :public material {
public:
	dielectric(double r):ref_idx(r){}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		
		attenuation = color(1.0, 1.0, 1.0);
		
		double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;
		
		vec3 unit_direction = unit_vector(r_in.getDirection());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		//考虑反射的情况，sin和n相乘大于1不能发生折射
		if (etai_over_etat * sin_theta > 1.0)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}
		double reflect_prob = schlick(cos_theta, etai_over_etat);//反射率
		if (random_double() < reflect_prob) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}
		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = ray(rec.p, refracted);
		return true;
	}

public:
	//折射率，由内到外的时候是1/ref_idx,玻璃一般是1.3-1.7
	double ref_idx;
};

