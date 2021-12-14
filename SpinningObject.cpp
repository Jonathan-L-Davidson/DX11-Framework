#include "SpinningObject.h"

SpinningObject::SpinningObject(XMFLOAT3 pos) {
	SetPos(pos);
}

SpinningObject::~SpinningObject() {

}

void SpinningObject::Update(double dt) {
	XMFLOAT3 rot = XMFLOAT3(0.0f, 0.15f * dt, 0.0f);

	this->AddRotation(rot);

	UpdateWorldCoords();
	return;
}