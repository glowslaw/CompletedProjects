#pragma once
#include "GameObject.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "ResourceManager.h"
#include <string>
#include<sstream> 

using namespace glm;

struct constants
{
	vec3 g = glm::vec3(0,-9.81,0);
	mat3 inertiaTensor = mat3(vec3(1200, 0, -800), vec3(0, 3500, 0), vec3(-800, 0, 2500));
	float pi = 3.1413f;
	float halfRo = 0.6f;
};

float liftCoef(vec3 airstream, glm::vec3 up, vec3 forawrd, float pitch);
quat RotationBetweenVectors(vec3 start, vec3 dest);
quat RotateTowards(quat q1, quat q2, float maxAngle);

class PhysObject : public GameObject
{
public:
	vec3 vel;
	quat angularVel;
	float mass;
	vec3 dragCross;
	float thrust;
	float throttle;
	float wingArea;
	float velForwardMag;

	float pitch, roll, yaw;

	vec3 forward, wing, up;
	quat rotation;


	vec3 centerOfMass;
	vec3 centerOfLift;
	vec3 centerOfThrust;
	int grounded;

	float boundingBoxSize;

	void updatePhysics(float timeDelta);
	void updateControl(float timeDelta, float velForwardMag);
	void checkCollision(GameObject* collider);
	void updateModelMatrix();
	void updateUI(ResourceManager *res);
	void reset(vec3 start);

	PhysObject(GameObject* parent = nullptr, // nullptr only for root
		int modelIndex = -1,
		int textureIndex = -1,
		vec3 pos = vec3(0, 0, 0),
		vec3 scale = vec3(1, 1, 1),
		vec3 rot = vec3(0, 1, 0),
		bool renderFlag = true
		);
};

