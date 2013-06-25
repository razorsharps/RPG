#pragma once

#include "visitor.h"
#include <string>
#include <iostream>
#include "../headers/Door.h"

class CollisionVisitor :
	public Visitor
{
public:
	void visit(GameObject *e) {
		std::cout << "GO"<< std::endl;
		e->onCollision();
	}

	void visit(Controls *e) {
		std::cout << " DO NOT USE ME FOR CONTROLS" << std::endl;
	}

	void visit(Door *e) {
		std::cout << "door"<< std::endl;
		e->onCollision();
	}
	/*
	void visit(Key *e) {
		std::cout << "key"<< std::endl;
		e->onCollision();
	}
	*/


};

