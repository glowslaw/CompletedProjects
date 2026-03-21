#include "GameObject.h"

GameObject::GameObject( GameObject* parent,
						int modelIndex,	// -1 -> no model
						int textureIndex,
						glm::vec3 pos,
						glm::vec3 scale,
						glm::vec3 rot,
						bool shading,
	                    bool renderFlag
						)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->shading = shading;

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
}

void GameObject::updateModelMatrix()
{
	modelMatrix = parent->modelMatrix;

	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, rot.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rot.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rot.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	for (GameObject* child : children)
	{
		child->updateModelMatrix();
	}
	
}


void GameObject::addChild(GameObject* child)
{
	children.push_back(child);
}