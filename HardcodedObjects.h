#pragma once
#include "Object.h"

class Cube : Object {
	Cube(DxDevice* device);
	~Cube();

	void Update(double dt);

};


class Pyramid : Object {
	Pyramid(DxDevice* device);
	~Pyramid();

	void Update(double dt);

};

