#pragma once

#include "visitor.h"
#include <string>
#include <iostream>
#include "../headers/Door.h"
#include "../headers/Key.h"

class CollisionVisitor :
	public Visitor
{
public:
	void visit(GameObject *e) {
		e->onCollision();
	}

	void visit(Controls *e) {
	}

	void visit(Door *e) {
		e->onCollision();
	}
	
	void visit(Key *e) {
		e->onCollision();
	}
	


};

