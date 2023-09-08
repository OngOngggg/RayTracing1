#pragma once

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//常量
const double infinity = std::numeric_limits<double>::infinity(); //无穷大
const double pi = 3.1415926535897932385;

//函数
inline double degrees_to_radians(double degrees) { //度数到弧度
    return degrees * pi / 180.0;
}

//公共头文件
#include"ray.h"
#include"vec3.h"