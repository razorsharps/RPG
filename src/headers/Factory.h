#include <iostream>
#include <string>

using namespace std;

// Abstract Base Class
class Factory {
    public:
       // Static class to create objects
       // Change is required only in this function to create a new object type
       static Factory* Create(const char * fileName);

	   virtual Mesh* getMesh(const char * fileName) = 0;
	   virtual Texture* getTexture(const char * fileName) = 0;
};

class MyGameObject : public Factory {
    public:	   
		Mesh* getMesh(const char * fileName) {
			return new Mesh(fileName);
		}
      
		Texture* getTexture(const char * fileName) {
			return new Texture(fileName);
		}
	  
		friend class Factory;
};

Factory* Factory::Create(const char * name) {
	if(name == "gameobject")
		return new MyGameObject();

	return NULL;
}
