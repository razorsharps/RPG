#pragma once

#include "../headers/Door.h";

Door::Door(std::string _name, glm::vec3 aPosition, glm::vec3 scale, bool steer, glm::vec3 rotation) : GameObject(_name,aPosition,scale,steer,rotation) {
}

Door::~Door(){
}


void Door::accept(Visitor &v) {
	//for the moment let gameobject handle it maybe in the future door will get his own accept
	GameObject::accept(v);
}
/*
void Door::render(){
	GameObject::render();
}
*/