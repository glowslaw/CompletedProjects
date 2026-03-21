#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "ResourceManager.h"

#define PI 3.1413


void Generate_Airport(ResourceManager* res, glm::vec3 position, float size, glm::vec3 rotation);
void Generate_Sky_Ground(ResourceManager* res, glm::vec3 position, float size, glm::vec3 rotation, float partition);

void Generate_City(ResourceManager* res);
void Generate_Plane_Hitboxes(ResourceManager* res);
