#include "../headers/Collision.h"
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>


Collision::Collision(GameObject * aHalo)
{
	halo = aHalo;
	colliders = new std::vector<GameObject*>();
}


Collision::~Collision()
{
}

void Collision::addObjects(GameObject* anObject) {
	colliders->push_back(anObject);
}

void Collision::update() {
	std::vector<GameObject*>::iterator iter;
	for(iter = colliders->begin(); iter != colliders->end(); ++iter) {
		
		if((glm::distance(halo->getPosition(), (*iter)->getPosition()) < (*iter)->collisionDistance) ) {
			std::cout << "Collided: " << halo->getName() << " to " << (*iter)->getName() << std::endl;
			halo->onCollide(*iter);
		}
	}
			//	playSound = true;
	//		control->setSpeed(-1.0f);
	//		s->playSound();
	//	} else if (playSound && currentTime - lastTime > 0.9f) {
	//		playSound = false;
	//}
}