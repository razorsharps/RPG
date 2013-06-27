#pragma once

#include "GameObject.h"
#include <iostream>
#include <string>
#include "../headers/Sound.h"

class Door :
	public GameObject
{
public:
	Door();
	Door(std::string aName, glm::vec3 aPosition, glm::vec3 scale, bool steer = false, glm::vec3 orientation = glm::vec3(0,0,0), float collisionDistance = 0.0f);
	~Door();
	void accept(Visitor &v);
	void onCollision();
	std::string key;

	Sound* sound;
	bool playSound;
};