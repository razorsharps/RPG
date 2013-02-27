#include "../headers/GameObject.h"

GameObject::GameObject(std::string _name, glm::vec3 aPosition) {
	name = _name;
	translate(aPosition);
}

GameObject::~GameObject() {
	
}

void GameObject::translate( glm::vec3 translation )
{
	transform = glm::translate( transform, translation );
}

void GameObject::rotate( float angle, glm::vec3 axis )
{
	transform = glm::rotate( transform, angle, axis);
}

const std::string GameObject::getName()
{
	return name;
}

glm::vec3 GameObject::getLocation()
{
	return glm::vec3( transform[3][0], transform[3][1], transform[3][2] );
}

void GameObject::setTexture(Texture* tex) {
	colorMap = tex;
}

void GameObject::setMesh(Mesh* mes) {
	mesh = mes;
}

