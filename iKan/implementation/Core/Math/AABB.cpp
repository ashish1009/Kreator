//
//  AABB.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "AABB.hpp"

using namespace iKan;

AABB::AABB() : Min(0.0f), Max(0.0f) {}
AABB::AABB(const glm::vec3& min, const glm::vec3& max) : Min(min), Max(max) {}
