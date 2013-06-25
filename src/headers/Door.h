#pragma once

#include "GameObject.h"

class Door :
	public GameObject
{
public:
	Door(std::string aName, glm::vec3 aPosition, glm::vec3 scale, bool steer = false, glm::vec3 orientation = glm::vec3(0,0,0), float collisionDistance = 0.0f);
	~Door();
	void accept(Visitor &v);
//	void render();
	void onCollide();
};