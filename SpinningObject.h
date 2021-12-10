#pragma once
#include "Object.h"
class SpinningObject : public Object {
public:
	SpinningObject();
	~SpinningObject();

	void Update(double dt);
};

