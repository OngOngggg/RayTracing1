#pragma once
#include<iostream>
#include"vec3.h"

#include"sphere.h"
#include"hittable_list.h"
#include"rtweekend.h"

using std::cin;
using std::cout;

//判断光线是否与某个球相交
//double hit_sphere(const point3& center, double radius, const ray& r) {
//	vec3 oc = r.getOrigin() - center;
//	//套公式
//	double a = dot(r.getDirection(), r.getDirection());
//	//double b = 2.0 * dot(r.getDirection(), oc);
//	double double_b = 2.0 * dot(r.getDirection(), oc);
//	double c = dot(oc, oc) - radius * radius;
//	/*double discriminant = b * b - 4 * a * c;*/
//	double discriminant = double_b * double_b - a * c;
//
//	if (discriminant < 0) return -1.0;
//	//double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
//	double t1 = (-double_b - sqrt(discriminant)) / a;
//	if (t1) return t1;
//	double t2 = (-double_b + sqrt(discriminant)) / a;
//	if (t2) return t2;
//	return -1.0;
//}

color ray_color(const ray& r,const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) { return 0.5 * (rec.normal + color(1,1,1)); }
	
	vec3 unit_direction = unit_vector(r.getDirection());
	double t = 0.5 * (unit_direction.y() + 1.0);
	//线性插值
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() 
{
	/*const int image_width = 256;
	const int image_height = 256;

	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			double r = double(i) / (image_width - 1);
			double g = double(j) / (image_height - 1);
			double b = 0.25;
			int ir = static_cast<int>(259.999 * r);
			int ig = static_cast<int>(259.999 * g);
			int ib = static_cast<int>(259.999 * b);
			cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}*/

	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	//视平面（cmos?)
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	//焦距
	double focal_length = 1.0;

	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	point3 origin = point3(0, 0, 0);
	vec3 horizontal = vec3(viewport_width, 0, 0);
	vec3 vertical = vec3(0, viewport_height, 0);
	//视口左下角的坐标
	point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	
	cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			double u = double(i) / (image_width - 1);
			double v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r,world);
			pixel_color.write_color(cout);
		}
	}
}
