#include "../headers/Collision.h"
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>
#include "../headers/Door.h"
#include "../headers/Astroid.h"


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
	std::vector<GameObject*>::iterator iter2;
	for(iter = colliders->begin(); iter != colliders->end(); ++iter) {
		for(iter2 = colliders->begin(); iter2 != colliders->end(); ++iter2) {
			if( *iter != *iter2) {
				if((glm::distance((*iter)->getPosition(), (*iter2)->getPosition()) < ((*iter)->collisionDistance/2) +( (*iter2)->collisionDistance/2) ) ) { //aanpassen
					Astroid * d = dynamic_cast<Astroid*>(*iter);
					if( d != 0) {
						d->accept(cv);
					} else {
						(*iter)->accept(cv);
					}
				}
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