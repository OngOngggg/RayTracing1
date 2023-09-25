#pragma once
#include<iostream>
#include"vec3.h"

#include"sphere.h"
#include"hittable_list.h"
#include"rtweekend.h"
#include"camera.h"
#include"material.h"

using std::cin;
using std::cout;

const int max_depth = 50;

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));//地面
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double(); //随机材质
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); //随机中心

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse 漫反射
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal 金属
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass 玻璃
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

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

color ray_color(const ray& r,const hittable& world,int depth) {
	hit_record rec;
	if (depth >= max_depth) return color(0, 0, 0);

	//if (world.hit(r, 0.001, infinity, rec)) {
	//	//point3 target = rec.p + rec.normal + random_in_unit_sphere();//随机点S
	//	point3 target = rec.p + rec.normal + random_unit_vector();//随机点S
	//	return 0.5 * ray_color(ray(rec.p, target - rec.p), world,depth+1);
	//}

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth + 1);
		return color(0, 0, 0);
	}
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

	const double aspect_ratio = 4.0 / 3.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	//抗锯齿模拟随机次数
	const int sample_per_pixel = 100;

	//World
	auto world = random_scene();

	//Camera
	point3 lookfrom(12, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	//auto dist_to_focus = 10.0;
	//auto aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio);

	//都是书中的例子

	/*auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<dielectric>(1.7);
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2),0.7);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/

	//camera cam;
	
	cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < sample_per_pixel; ++s)
			{
				//加随机值完成对抗锯齿的模拟，
				double u = (i + random_double()) / (image_width + 1);
				double v = (j + random_double()) / (image_height + 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world,0);
			}
			pixel_color.write_color(cout, sample_per_pixel);
		}
	}
}
