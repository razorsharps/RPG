#pragma once

#include "../headers/Door.h"
#include "../headers/Game.h"
#include "../headers/Inventory.h"

#include "../headers/DoorObserver.h"

Door::Door() {
	playSound = true;
}

Door::Door(std::string _name, glm::vec3 aPosition, glm::vec3 scale, bool steer, glm::vec3 rotation, float aCollisionDistance) : GameObject(_name,aPosition,scale,steer,rotation, aCollisionDistance) {
}

Door::~Door(){
}


void Door::accept(Visitor &v) {
	v.visit(this);
}

void Door::onCollision() {
	if(playSound) {
		playSound = false;
		
		Subject sub;
		DoorObserver dob(&sub, sound);
		sub.setVal();
	}

	if(Inventory::getKey(key))
		this->position.y += 0.01f;
}