#pragma once
#include "Object.h"
class SpinningObject : public Object {
public:
	SpinningObject(XMFLOAT3 pos);
	~SpinningObject();

	void Update(double dt);
};

