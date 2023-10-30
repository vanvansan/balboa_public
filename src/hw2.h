#pragma once

#include "image.h"
#include "hw1.h"
#include <vector>
#include <string>

Image3 hw_2_1(const std::vector<std::string> &params);
Image3 hw_2_2(const std::vector<std::string> &params);
Image3 hw_2_3(const std::vector<std::string> &params);
Image3 hw_2_4(const std::vector<std::string> &params);

Vector2 c_space_to_i_space(Vector2 v, Real s, Real a, Real width, Real height);
Vector2 projectedPoint(Vector3 v);
