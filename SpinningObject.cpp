#include "SpinningObject.h"

SpinningObject::SpinningObject() {

}

SpinningObject::~SpinningObject() {

}

void SpinningObject::Update(double dt) {
	XMVECTOR rot = XMLoadFloat3(new XMFLOAT3(0.0f, 0.15f * dt, 0.0f));

	this->AddRotation(rot);

	UpdateWorldCoords();
	return;
}