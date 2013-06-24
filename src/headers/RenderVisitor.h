#pragma once
#include "visitor.h"
#include <string>
#include <iostream>
//#include "../headers/GameObject.h"
#include "../headers/Door.h"

class RenderVisitor :
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
		e->render();
	}

};

