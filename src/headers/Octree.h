#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <string>
#include "GLM/glm.hpp"
#include "../headers/CollisionVisitor.h"

class GameObject;
class Game;

class Octree
{
	public:
		Octree(Game * game, const glm::vec3 & aCenter, float aRadius, unsigned int aLevel = 0 );
		virtual ~Octree();

		virtual void add( GameObject * anObject );
		virtual void gatherObjects( std::vector<GameObject *> & target );

		virtual void print(std::string pre = "" );
		virtual unsigned int detectCollisions();
		virtual unsigned int detectCollisions( GameObject * collider );
		void removeObject(GameObject* gameObject);
		void CheckEdges();
	
		CollisionVisitor cv;
		Game * game;

		glm::vec3 center; // center of the cube
		float radius; // radius of the cube ( center to side )
		std::vector<GameObject *> objects; // all the objects in this space
		bool divided;
		unsigned int level;
		std::vector<Octree *> children; // all the subspaces ( x,y,z, should be 0 as smaller than plane of 1 bigger than plane, may be null
};

#endif // OCTREE_H
