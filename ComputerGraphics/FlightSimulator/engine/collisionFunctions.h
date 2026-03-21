#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "PhysObject.h"

bool intersectPlane(vec3& n, vec3& p0, vec3& l0, vec3& l, float& t);
bool Collision(PhysObject* plane);
void checkCollisions(PhysObject* plane);