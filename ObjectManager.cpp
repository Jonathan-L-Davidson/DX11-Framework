#include "ObjectManager.h"
#include "Structures.h"

ObjectManager::ObjectManager() {
	_objects = nullptr;
	_time = nullptr;
}

void ObjectManager::Initialise() {
	_objects = new std::vector<Object*>();

}


void ObjectManager::Update() {
	for (int i = 0; i < _objects->size(); i++) {
		_objects->at(i)->Update(_time->GetDelta());
	}
}


ObjectManager::~ObjectManager() {
	_objects->clear();
	_objects = nullptr;
}