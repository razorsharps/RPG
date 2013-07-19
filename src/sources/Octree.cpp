#include "../headers/Octree.h"
#include "../headers/GameObject.h"
#include "../headers/Rocket.h"
#include "../headers/Game.h"

Octree::Octree(Game * aGame, const glm::vec3 & aCenter, float aRadius, unsigned int aLevel )
:	game(aGame), center(aCenter), radius(aRadius), level(aLevel)
{
	for ( int i = 0; i < 8; i++ ) {
		children.push_back( NULL );
	}
	divided = false;
}

Octree::~Octree()
{
	//dtor
}


void Octree::add( GameObject * anObject )
{
	// if bottom level store otherwise divide if fits

	if ( level == 0 ) { // if on bottom
		objects.push_back( anObject );
	} else {
		// get index for min cube vertex . index is per bit (b0, b1, b2 for subdiv in xyz
		unsigned int minIndex = 1*(anObject->position.x - anObject->collisionDistance >= center.x) + 2*( anObject->position.y - anObject->collisionDistance >= center.y ) + 4*( anObject->position.z - anObject->collisionDistance >= center.z );
		// get index for max cube vertex
		unsigned int maxIndex = 1*(anObject->position.x + anObject->collisionDistance >= center.x) + 2*( anObject->position.y + anObject->collisionDistance >= center.y ) + 4*( anObject->position.z + anObject->collisionDistance >= center.z );
		if ( minIndex == maxIndex ) { // fits in one space
			unsigned int index = minIndex;

			if ( ! children[index]) { // wanted space does not exist yet so add it
				children[index] = new Octree(game, glm::vec3( center.x+radius*( (index&1)-0.5f), center.y+radius*( (index&2)/2-0.5f), center.z+radius*( (index&4)/4-0.5f)), radius / 2, level-1 );
			}
			children[index]->add( anObject ); // add object to space
		} else { // when does not fit in child, put it here
			objects.push_back( anObject ); // does not fit in any child so store here
		}
	}
}

void Octree::gatherObjects( std::vector<GameObject *> & target ) {
	for ( unsigned int i = 0; i < objects.size(); i++ ) {
		target.push_back( objects[i] );
	}
	for ( unsigned int i = 0; i < children.size(); i++ ) {
		if ( children[i] ) {
			children[i]->gatherObjects( target );
		}
	}

	objects.clear();
}

void Octree::print( std::string pre )
{
	std::cout << pre << level << " Node : " << center.x << " " << center.y << " " << center.z << "\t";
	for ( unsigned int i = 0; i < objects.size(); i++ ) {
		std::cout << objects[i]->name << ", ";
	}
	std::cout << std::endl;

	for ( int i = 0; i < 8; i++ ) {
		if ( children[i] ) {
			children[i]-> print( pre+"  " );
		}
	}
}

void Octree::CheckEdges(){
 //check x
	for ( unsigned int i = 0; i < objects.size(); i++ ) {
		Astroid * d = dynamic_cast<Astroid*>(objects[i]);
		if(d != 0) {
			if(objects[i]->position.x > center.x+radius ) {
				objects[i]->position.x = center.x + radius;
				objects[i]->orientation.x +=3.1415f;
			} else if (objects[i]->position.x < center.x-radius){
				objects[i]->position.x = center.x - radius;
				objects[i]->orientation.x +=3.1415f;
			}		
			
			if(objects[i]->position.y > center.y+radius ) {
				objects[i]->position.y = center.y + radius;
				objects[i]->orientation.y +=3.1415f;
			} else if (objects[i]->position.y < center.y-radius){
				objects[i]->position.y = center.y - radius;
				objects[i]->orientation.y +=3.1415f;
			}
			
			if(objects[i]->position.z > center.z+radius ) {
				objects[i]->position.z = center.z + radius;
				objects[i]->orientation.z +=3.1415f;
			} else 	if (objects[i]->position.z < center.z-radius){
				objects[i]->position.z = center.z - radius;
				objects[i]->orientation.z +=3.1415f;
			}
		}
	}
	for ( unsigned int i = 0; i < children.size(); i++ ) {
		if ( children[i] ) {
			children[i]->CheckEdges();
		}
	}

}

unsigned int Octree::detectCollisions() {
	unsigned int count = 0;
	// detect agains objects in this spaces and in children
	for ( unsigned int i = 0; i < objects.size(); i++ ) { // check every objects in this space
		GameObject * collider = objects[i];
		for ( unsigned int j = i+1; j < objects.size(); j++ ) { // check collider agains otyher object in this space
			GameObject * collidee = objects[j];
			count ++;

			if((glm::distance(collider->getPosition(), collidee->getPosition()) < (collider->collisionDistance/2) +( collidee->collisionDistance/2) ) ) { //aanpassen
				Astroid * d = dynamic_cast<Astroid*>(collider);
				
				if( d != 0) {
					Rocket * r = dynamic_cast<Rocket*>(collidee);

					if(r != 0) {
						removeObject(r);
						removeObject(d);

						game->renderer->removeObject(r);
						game->renderer->removeObject(d);

						game->removeObject(r);
					} else {
						d->accept(cv);
					}
				} else {
					Rocket * r = dynamic_cast<Rocket*>(collider);

					if(r != 0) {
						removeObject(r);
						removeObject(d);

						game->removeObject(r);
						game->renderer->removeObject(r);
						game->renderer->removeObject(d);
					} else {
						collider->accept(cv);
					}
				}
			}
		}

		// detect collider  against children content
		for ( int i = 0; i < 8; i++ ) {
			Octree * child = children[i];
			if ( child ) {
				count += child->detectCollisions( collider );
			}
		}
	}

	// let children detect internal collisions
	for ( int i = 0; i < 8; i++ ) {
		Octree * child = children[i];
		if ( child ) {
			count += child->detectCollisions();
		}
	}
	return count;
}

unsigned int Octree::detectCollisions( GameObject * collider ) {
	unsigned int count = 0;
	// checking against own objects
	for ( unsigned int j = 0; j < objects.size(); j++ ) {
		GameObject * collidee = objects[j];
		if ( collider != collidee ) { // do not check against itself
			if((glm::distance(collider->getPosition(), collidee->getPosition()) < (collider->collisionDistance/2) +( collidee->collisionDistance/2) ) ) { //aanpassen
				Astroid * d = dynamic_cast<Astroid*>(collider);
				if( d != 0) {
					d->accept(cv);
				} else {
					collider->accept(cv);
				}
			}
		}
	}
	// let children check as wel
	for ( int i = 0; i < 8; i++ ) {
		Octree * child = children[i];
		if ( child ) {
			count += child->detectCollisions( collider );
		}
	}
	return count;
}

void Octree::removeObject(GameObject* gameObject) {
	std::vector<GameObject*>::iterator iter = objects.begin();
	bool done = false;

	while (iter != objects.end())
	{
		if ((*iter) == gameObject) {
			iter = objects.erase(iter);
			done = true;
			break;
		} else
			++iter;
	}

	if(!done) {
		for ( int i = 0; i < 8; i++ ) {
			Octree * child = children[i];
			if ( child ) {
				child->removeObject( gameObject );
			}
		}
	}
}