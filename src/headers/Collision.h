#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../headers/GameObject.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

class Collision
{
public:
	Collision(GameObject * halo);
	~Collision();
	std::vector<GameObject*> *colliders;
	GameObject * halo;
	void addObjects(GameObject* anObject);
	void update();
};
