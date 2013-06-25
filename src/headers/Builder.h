#include <iostream>
#include <string>

#include "../headers/Mesh.h"
#include "../headers/GameObject.h"
#include "../headers/Door.h"
#include "../headers/Key.h"
#include "../headers/Factory.h"

#include <sstream>
#include <fstream>
#include <string>
#include <vector>

/* Builder is responsible for constructing the smaller parts */
class Builder
{
	public:
		virtual Mesh* getMesh(const char * fileName) = 0;
		virtual Texture* getTexture(const char * fileName) = 0;
};
 
/* Director is responsible for the whole process */
class Director
{
	Builder* builder;
 
	public:
		void setBuilder(Builder* newBuilder) {
			builder = newBuilder;
		}
 
		vector<GameObject*> getGameObject(GLuint shader)
		{
			string line;
			ifstream myfile ("src/headers/level.txt");
			vector<GameObject*> gameObjectList;

			if (myfile.is_open())
			{
				while ( myfile.good() )
				{
					getline (myfile,line);
					
					std::vector<std::string> tokens;
					std::istringstream iss(line);
					std::string token;

					while(std::getline(iss, token, '\t'))   // but we can specify a different one
						tokens.push_back(token);


					GameObject* gameObject;
					std::cout << tokens.at(0) << std::endl;

					if(tokens.at(0) == "Door") {
						gameObject = new Door();
						gameObject->init(shader);
			
						gameObject->name = tokens.at(0);
						gameObject->setMesh(builder->getMesh(tokens.at(1).c_str()));
						gameObject->setTexture(builder->getTexture(tokens.at(2).c_str()));
						gameObject->position = glm::vec3(atof(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()));
						gameObject->scaling = glm::vec3(atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), atof(tokens.at(8).c_str()));
						
						gameObject->collisionDistance = 2.5f;

					} else if (tokens.at(0) == "Key1" ) {
						std::cout << "building key" <<std::endl;
						gameObject = new Key();
						gameObject->init(shader);
			
						gameObject->name = tokens.at(0);
						gameObject->setMesh(builder->getMesh(tokens.at(1).c_str()));
						gameObject->setTexture(builder->getTexture(tokens.at(2).c_str()));
						gameObject->position = glm::vec3(atof(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()));
						gameObject->scaling = glm::vec3(atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), atof(tokens.at(8).c_str()));
						gameObject->collisionDistance = 0.5f;
						
						std::cout << "build a key" <<std::endl;
					}  else {
						std::cout << " building something else" << std::endl;
						gameObject = new GameObject();
						gameObject->init(shader);
			
						gameObject->name = tokens.at(0);
						gameObject->setMesh(builder->getMesh(tokens.at(1).c_str()));
						gameObject->setTexture(builder->getTexture(tokens.at(2).c_str()));
						gameObject->position = glm::vec3(atof(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()));
						gameObject->scaling = glm::vec3(atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), atof(tokens.at(8).c_str()));
						std::cout << "Done building something else" << std::endl;
					}

					gameObjectList.push_back(gameObject);
				}
				myfile.close();
			}

			else cout << "Unable to open file" << endl; 
 
			return gameObjectList;
		}
};
 
class ObjectBuilder : public Builder
{
	public:
		Factory* fac;

		Mesh* getMesh(const char * fileName)
		{
			fac = Factory::Create("gameobject");
			return fac->getMesh(fileName);
		}

		Texture* getTexture(const char * fileName)
		{
			return fac->getTexture(fileName);
		}
};
