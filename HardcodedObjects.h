#pragma once
#include "SpinningObject.h"

class Cube : public SpinningObject {
public:
	Cube(XMFLOAT3 pos, DxDevice* device) : SpinningObject(pos) {};
	~Cube();

};


class Pyramid : public SpinningObject {
public:
	Pyramid(XMFLOAT3 pos, DxDevice* device) : SpinningObject(pos) {};
	~Pyramid();

};

