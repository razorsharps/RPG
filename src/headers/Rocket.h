#pragma once

#include "GameObject.h"

class Rocket :
	public GameObject
{
public:
	Rocket(std::string aName, glm::vec3 aPosition, glm::vec3 scale, float time, bool steer = false, glm::vec3 orientation = glm::vec3(0,0,0), float collisionDistance = 0.0f);
	~Rocket();

	float duration;
};