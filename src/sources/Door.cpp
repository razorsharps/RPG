#pragma once

#include "../headers/Door.h";

Door::Door() {
}

Door::Door(std::string _name, glm::vec3 aPosition, glm::vec3 scale, bool steer, glm::vec3 rotation, float aCollisionDistance) : GameObject(_name,aPosition,scale,steer,rotation, aCollisionDistance) {
}

Door::~Door(){
}


void Door::accept(Visitor &v) {
	if( typeid(v).name() == "RenderVisitor") {
		GameObject::accept(v);
	}
	else {
		v.visit(this);
	}
}
/*
void Door::render(){
	GameObject::render();
}
*/

void Door::onCollision() {
	std::cout << "Door collided" << std::endl;
}