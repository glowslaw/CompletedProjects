#include "PhysObject.h"

constants cons;


PhysObject::PhysObject( GameObject* parent,
	int modelIndex,	// -1 -> no model
	int textureIndex,
	glm::vec3 pos,
	glm::vec3 scale,
	glm::vec3 rot,
	bool renderFlag
	)
{

	this->pos = pos;
	this->rot = rot;
	this->scale = scale;

	this->vel = vec3(0);
	this->angularVel = quat(vec3(0, 0, 0));
	this->rotation = quat(rot);
	velForwardMag = 0.0f;

	pitch = 0.0f;
	roll = 0.0f;
	yaw = 0.0f;
	throttle = 0.0f;

	this->mass = 10000.0f;
	this->dragCross = vec3(0.4f * 15 , 0.6f*20, 0.3f * 10);
	this->thrust = 25000.f; //6000.0f;
	this->wingArea = 120.f; // 30.0f;

	this->forward = vec3(0, 0, 1.0f);
	this->wing = vec3(1.0f, 0, 0);

	this->modelIndex = modelIndex;
	this->textureIndex = textureIndex;

	this->parent = parent;
	if (parent != nullptr) parent->addChild(this);

	if (modelIndex != -1)
	{
		this->renderFlag = renderFlag;
		updateModelMatrix();
	}
	else this->renderFlag = false;

	boundingBoxSize = scale.y;
}

void PhysObject::updateControl(float timeDelta, float velForwardMag)
{
	//Throttle

	vec3 inertia = vec3(80.0f, 200.0f, 40.0f);

	//drag 
	angularVel = mix(angularVel, quat(vec3(0,0,0)), min(timeDelta + timeDelta * velForwardMag,1.0f));

	//Control surface rotation
	angularVel *= quat(vec3(pitch * velForwardMag * timeDelta / inertia.x,
							yaw * velForwardMag * timeDelta / inertia.y, 
						-roll * velForwardMag * timeDelta / inertia.z));

	if (grounded && eulerAngles(rotation).x < -cons.pi / 20) angularVel = quat(vec3(0, 0, 0));
	rotation = mix(rotation,rotation * angularVel, max(timeDelta,1.0f));
	//rotation = mix(rotation, RotationBetweenVectors(forward,vel), min(timeDelta* velForwardMag*0.01f,1.0f));

}


void PhysObject::updateModelMatrix()
{
	modelMatrix = parent->modelMatrix;

	mat4 I = mat4(1.0f);
	mat4 transMatrix = translate(I, pos);
	mat4 RotationMatrix = toMat4(rotation); // quat(vec3(cons.pi / 2, 0, cons.pi)));
	mat4 scaleMatrix = glm::scale(I, scale);
	
	modelMatrix = modelMatrix * transMatrix * RotationMatrix * scaleMatrix;
	modelMatrix = translate(modelMatrix,vec3(0,-1,0));

	for (GameObject* child : children)
	{
		child->updateModelMatrix();
	}
}

void PhysObject::updatePhysics(float timeDelta)
{
	vec3 forward = normalize(rotation * vec3(0.f, 0.f, 1.f));
	vec3 wing = normalize(rotation * vec3(1.f, 0.f, 0.f));
	vec3 up = normalize(rotation * vec3(0.f, 1.f, 0.f));

	glm::vec3 velUp = glm::normalize(glm::cross(vel, wing));

	glm::vec3 thrustVec = forward * (thrust * throttle);
	glm::vec3 drag = -(abs(vel) * vel) * cons.halfRo * 5.f;//2.0f;// ((-rotation) * dragCross);
	

	velForwardMag = glm::length(vel * vel * forward / glm::length(vel));
	if (std::isnan(velForwardMag)) velForwardMag = 0;
	glm::vec3 lift = (velUp * abs(velForwardMag) * velForwardMag) * cons.halfRo * wingArea * liftCoef(vel, up, forward,pitch);
	if (std::isnan(lift.x)) lift = glm::vec3(0.0f);
	
	//std::cout << "Vel:" << vel.x << ' ' << vel.y << ' ' << vel.z << std::endl;
	//std::cout << "Drag:" << drag.x << ' ' << drag.y << ' ' << drag.z << std::endl;
	//std::cout << "Lift: "<<lift.x << ' ' << lift.y << " " << lift.z << std::endl << std::endl;

	glm::vec3 totalForce = thrustVec+drag+lift;
	glm::vec3 totalAccel = totalForce / mass;
	totalAccel += cons.g;
	vel += totalAccel * timeDelta * 2.0f;
	pos += vel * timeDelta * 4.0f;

	updateControl(timeDelta, velForwardMag);
}

float liftCoef(glm::vec3 airstream, glm::vec3 up, glm::vec3 forward, float pitch)
{
	float angle = -asin(glm::dot(airstream, up)/(sqrt(up.x*up.x + up.y * up.y + up.z * up.z)*
		sqrt(airstream.x * airstream.x + airstream.y * airstream.y + airstream.z * airstream.z)));

	angle += 0.04f + (pitch * -0.05f);

	//std::cout << "Angle: ";

	if ((angle > - cons.pi / 8 && angle < cons.pi / 8) || (angle > 7 * cons.pi / 8 && angle < cons.pi))
	{ 
		//std::cout << glm::sin(6 * angle)<< std::endl;
		return 1.5*glm::sin(6 * angle);
	}
	//std::cout << glm::sin(2 * angle) << std::endl;
	return 1.5 * glm::sin(2 * angle);
}

void PhysObject::checkCollision(GameObject* collider)
{
	if (pos.y - boundingBoxSize < collider->pos.y)
	{
		pos.y = collider->pos.y + boundingBoxSize;
		vel.y = 0;
		grounded = 1;
		return;
	}
	grounded = 0;
}

quat RotationBetweenVectors(vec3 start, vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1.f + 0.001f) {
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) //they were parallel
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

quat RotateTowards(quat q1, quat q2, float maxAngle) {

	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1 * -1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	float fT = maxAngle / angle;
	angle = maxAngle;

	quat res = (sin((1.0f - fT) * angle) * q1 + sin(fT * angle) * q2) / sin(angle);
	res = normalize(res);
	return res;

}


void PhysObject::updateUI(ResourceManager *res)
{
	std::stringstream out;
	out.precision(2);
	out << std::fixed;
	out << "Speed: " <<velForwardMag *3.6f << " km/h";
	res->textQueue[0].value = out.str();
	out.precision(1);
	out.str(std::string());
	if(pos.y < 3000) out << "Attitude: " << (pos.y) /3.0 << " m";
	else out << "Attitude: " << (pos.y - 1) / 3000.f << " km";
	res->textQueue[1].value =  out.str();
	out.precision(0);
	out.str(std::string());
	out << "Throttle: " << throttle * 100 << "%";
	res->textQueue[2].value = out.str();

	////
}


void PhysObject::reset(vec3 start)
{
	pos = start;
	vel = vec3(0, 0, 0);

	velForwardMag = 0.0f;

	pitch = 0.0f;
	roll = 0.0f;
	yaw = 0.0f;
	throttle = 0.0f;

	angularVel = quat(vec3(0, 0, 0));
	rotation = quat(rot);
}