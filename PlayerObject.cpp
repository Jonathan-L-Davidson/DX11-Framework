#include "PlayerObject.h"

PlayerObject::PlayerObject(XMFLOAT3 pos) {
	SetPos(pos);

	_cameraOffsetFPS = XMFLOAT3(0, 3, 0);
	_cameraOffsetTPS = XMFLOAT3(0, 6, 6);
}

PlayerObject::~PlayerObject() {

}

void PlayerObject::Update(double dt) {
	CheckInput();

	if (_firstPersonView || _thirdPersonView) {
		UpdateCameraOffset();
		// Create position with offset from the object's offset.
		XMVECTOR pos = XMVector3Transform(XMLoadFloat3(&_cameraOffset), XMLoadFloat4x4(&GetPos()));
		ChangeCameraPos(_currentCamera, pos);
	}

	UpdateWorldCoords();
	return;
}

void PlayerObject::UpdateCameraOffset() {
	if (_firstPersonView && _thirdPersonView) { // both can't be enabled.
		_thirdPersonView = false;
		ChangeCamera(_firstPersonCamera); // force camera to be first person if both are selected.
	}

	if (_firstPersonView) {
		_cameraOffset = _cameraOffsetFPS;
	}

	if (_thirdPersonView) {
		_cameraOffset = _cameraOffsetTPS;
	}
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
