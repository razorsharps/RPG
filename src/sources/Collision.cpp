#include "../headers/Collision.h"
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>
#include "../headers/Door.h"


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
			std::cout << (*iter)->name << std::endl;
			Door * d = dynamic_cast<Door*>(*iter);
			if( d != 0) {
				d->accept(cv);
			} else {
				Key * k = dynamic_cast<Key*>(*iter);
				if( k != 0) {
					k->accept(cv);
				} else {
					(*iter)->accept(cv);
				}
			}
		}
	}
			//	playSound = true;
	//		control->setSpeed(-1.0f);
	//		s->playSound();
	//	} else if (playSound && currentTime - lastTime > 0.9f) {
	//		playSound = false;
	//}
}