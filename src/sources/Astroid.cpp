#include "../Headers/Astroid.h"


Astroid::Astroid(void)
{
}

Astroid::Astroid(std::string _name, glm::vec3 aPosition, glm::vec3 scale, float speed, bool steer, glm::vec3 rotation, float aCollisionDistance) : GameObject(_name,aPosition,scale,steer,rotation, aCollisionDistance) {
	this->speed = speed;
}

Astroid::~Astroid(void)
{
}

void Astroid::accept(Visitor &v) {
	v.visit(this);
}

void Astroid::onCollision() {
	//std::cout << "GOT HIT" << std::endl;
	orientation.x += 3.1415f;
	orientation.y += 3.1415f;
	orientation.z += 3.1415f;
}
