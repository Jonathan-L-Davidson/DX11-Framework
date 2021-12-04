#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include <vector>
#include "Object.h"

class ObjectManager {
private:
	std::vector<Object*>* _objects;

public:
	ObjectManager();
	~ObjectManager();

	void Initialise();

	void Update();

	std::vector<Object*>* GetObjects() { return _objects; };

	void AddObject(Object* obj) { _objects->push_back(obj); obj->SetID(_objects->size()); };
};

