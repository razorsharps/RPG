#pragma once

#include "../headers/Key.h";

Key::Key() {
}

Key::Key(std::string _name, glm::vec3 aPosition, glm::vec3 scale, bool steer, glm::vec3 rotation, float aCollisionDistance) : GameObject(_name,aPosition,scale,steer,rotation, aCollisionDistance) {
}

Key::~Key(){
}


void Key::accept(Visitor &v) {
	if( typeid(v).name() == "RenderVisitor") {
		GameObject::accept(v);
	}
	else {
		v.visit(this);
	}
}


void Key::onCollision() {
	std::cout << "Key collided" << std::endl;
}