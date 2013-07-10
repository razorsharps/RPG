#pragma once

#include "visitor.h"
#include <string>
#include <iostream>
#include "../headers/Door.h"
#include "../headers/Key.h"
#include "../headers/Astroid.h"
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
	
	void visit(Astroid *e){
		e->onCollision();
	}


};

