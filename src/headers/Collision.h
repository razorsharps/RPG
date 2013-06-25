#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../headers/GameObject.h"
#include "../headers/CollisionVisitor.h"
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
	CollisionVisitor cv;
	void addObjects(GameObject* anObject);
	void update();
};
