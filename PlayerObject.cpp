#include "PlayerObject.h"

PlayerObject::PlayerObject(XMFLOAT3 pos) {
	SetPos(pos);
}

PlayerObject::~PlayerObject() {

}

void PlayerObject::Update(double dt) {
	CheckInput();

	if (firstPersonView || thirdPersonView) {
		XMFLOAT4X4 pos = GetPos();
		ChangeCameraPos(_currentCamera, XMLoadFloat4x4(pos))
	}
	UpdateWorldCoords();
	return;
}

void PlayerObject::CheckInput() {
	XMFLOAT3 movement = XMFLOAT3(0, 0, 0);
	XMFLOAT3 rotation = XMFLOAT3(0, 0, 0);

	if (movement.x || movement.y || movement.z) {
		AddPos(movement);
	}
	if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0) {
		AddRotation(rotation);
	}

}

void PlayerObject::ChangeCamera(Camera* cam) {

}

void PlayerObject::ChangeCameraPos(Camera* cam, XMVECTOR pos) {

}

void PlayerObject::ChangeCameraRot(Camera* cam, XMVECTOR rot) {

}
