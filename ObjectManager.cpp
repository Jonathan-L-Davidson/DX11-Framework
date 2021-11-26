#include "ObjectManager.h"
#include "Structures.h"

ObjectManager::ObjectManager() {
	_objects = nullptr;
}

void ObjectManager::Initialise() {
	_objects = new std::vector<Object*>();
}

ObjectManager::~ObjectManager() {
	_objects->clear();
	_objects = nullptr;
}