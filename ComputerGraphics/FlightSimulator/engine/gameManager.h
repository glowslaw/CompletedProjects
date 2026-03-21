#pragma once
//Root Object Menager

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>  

#include "openglFunctions.h"
#include "PhysObject.h"
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


class GameManager
{
public:
	GLFWwindow* window;
	int status;

	bool keyStatus[10];

	float aspectRatio;
	float screenSizeX, screenSizeY;
	float timeDelta;

	glm::mat4 camera;
	glm::mat4 perspective;
	glm::mat4 uiProjection;

	glm::vec3 lightPos;

	glm::vec3 cameraOffset;
	glm::quat cameraOrientation;

	void updateTimeDelta();
	void updateCamera(glm::vec3 pos);
	void updatePerspective();
	void cameraFollow(glm::vec3 pos, glm::quat orientation);
	void updateControls(PhysObject* plane);

	GameManager(GLFWwindow* window);
};

