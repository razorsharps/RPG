#pragma once

#include "visitor.h"
#include <string>
#include <iostream>
class CollisionVisitor :
	public Visitor
{
public:
	void visit(GameObject *e) {
		e->render();
	}

	void visit(Controls *e) {
		std::cout << " DO NOT USE ME FOR CONTROLS" << std::endl;
	}


	void visit(Door *e) {
		std::cout << " DO NOT USE ME FOR DOOR" << std::endl; 
	}

};

