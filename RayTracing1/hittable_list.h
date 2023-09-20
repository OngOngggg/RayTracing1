#pragma once
#include"hittable.h"
#include<vector>
#include<memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class hittable_list :public hittable 
{
public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object){objects.push_back(object);}

public:
	vector<shared_ptr<hittable>> objects;

	bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
	{
		hit_record tem_rec;
		bool hit_anything = false;
		double tem_t_max = t_max;
		for (const auto& object : objects)
			if (object->hit(r, t_min, tem_t_max, tem_rec))
			{
				hit_anything = true;
				tem_t_max = tem_rec.t;
				rec = tem_rec;
			}
		return hit_anything;
	}
};
