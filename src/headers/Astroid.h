#pragma once
#include "GameObject.h"
#include <iostream>
#include <string>
class Astroid :
	public GameObject
{
public:
	Astroid();
	Astroid(std::string aName, glm::vec3 aPosition, glm::vec3 scale, float speed = 1.0f, bool steer = false, glm::vec3 orientation = glm::vec3(0,0,0), float collisionDistance = 0.0f);
	void accept(Visitor &v);
	void onCollision();
	~Astroid();
	float speed;
};

