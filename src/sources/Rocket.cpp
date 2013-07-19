#include "../headers/Rocket.h"

Rocket::Rocket(std::string _name, glm::vec3 aPosition, glm::vec3 scale, float time, bool steer, glm::vec3 rotation, float aCollisionDistance) : GameObject(_name,aPosition,scale,steer,rotation, aCollisionDistance) {
	duration = time;
}

Rocket::~Rocket() {

}