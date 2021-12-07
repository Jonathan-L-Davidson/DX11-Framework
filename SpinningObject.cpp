#include "SpinningObject.h"

SpinningObject::SpinningObject() {

}

SpinningObject::~SpinningObject() {

}

void SpinningObject::Update() {
	XMVECTOR rot = XMLoadFloat3(new XMFLOAT3(0.0f, 0.01f, 0.0f));

	this->AddRotation(rot);

	UpdateWorldCoords();
	return;
}