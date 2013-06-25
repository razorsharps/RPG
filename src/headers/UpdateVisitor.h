#pragma once
#include "visitor.h"
#include <string>
//#include "Controls.h"
class UpdateVisitor :
	public Visitor
{
public:
	void visit(Controls *e) {
		e->update();
		e->updateCamera();
	}

	void visit (GameObject * e) {
		std::cout << " DO NOT USE ME FOR GAMEOBJECTS " << std::endl;
	}

	void visit (Door *e) {
		std::cout << "DO NOT USE ME FOR DOOR" << std::endl;
	}

	void visit (Key *e) {
		std::cout << "DO NOT USE ME FOR DOOR" << std::endl;
	}
};
