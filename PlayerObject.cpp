#include "PlayerObject.h"

PlayerObject::PlayerObject(XMFLOAT3 pos) {
	SetPos(pos);

	_input = nullptr;
	_window = nullptr;

	_currentCamera = nullptr;
	_staticCamera = nullptr;
	_topDownCamera = nullptr;
	_firstPersonCamera = nullptr;
	_thirdPersonCamera = nullptr;
}

PlayerObject::~PlayerObject() {
	delete _staticCamera;
	delete _topDownCamera;
	delete _firstPersonCamera;
	delete _thirdPersonCamera;
}

void PlayerObject::Initialise() {
	_cameraOffsetFPS = XMFLOAT3(0, 0.5f, 0);
	_cameraOffsetTPS = XMFLOAT3(0, 0.5f, 6);
	
	_staticCamera = _window->GetCamera();
	
		XMVECTOR cameraTopDownEye = XMLoadFloat3(new XMFLOAT3(0, 50, -10));
	XMVECTOR cameraTopDownPos = XMLoadFloat3(new XMFLOAT3(0, -30, 0));

	_topDownCamera = new Camera();
	_topDownCamera->SetEye(cameraTopDownEye);
	_topDownCamera->SetPos(cameraTopDownPos);


	_firstPersonCamera = new Camera();
	_thirdPersonCamera = new Camera();
}

void PlayerObject::Update(double dt) {
	CheckInput(dt);

	if (_firstPersonView || _thirdPersonView) {
		UpdateCameraOffset();
		XMVECTOR offset = XMLoadFloat3(&_cameraOffset);
		XMFLOAT4X4 f4x4PlayerPos = GetPos();
		XMMATRIX matrixPlayerPos = XMLoadFloat4x4(&f4x4PlayerPos);
		XMVECTOR playerPos = XMVector3Transform(offset, matrixPlayerPos);

		// Create position with offset from the player's position.
		_cameraPosition = XMVectorAdd(playerPos, offset);
		
		_currentCamera->SetEye(playerPos);
	}

	UpdateWorldCoords();
	return;
}

void PlayerObject::UpdateCameraOffset() {
	if (_firstPersonView && _thirdPersonView) { // both can't be enabled.
		_thirdPersonView = false;
		ChangeCamera(_firstPersonCamera); // force camera to be first person if both are selected.
	}

	_cameraOffset = XMFLOAT3(0, 0, 0);

	if (_firstPersonView) {
		_cameraOffset = _cameraOffsetFPS;
	} else if (_thirdPersonView) {
		_cameraOffset = _cameraOffsetTPS;
	}

}

void PlayerObject::CheckInput(double dt) {
	XMFLOAT3 movement = HandleMovementInput(dt);
	XMFLOAT3 rotation = HandleViewInput(dt);


	if (movement.x || movement.y || movement.z) {
		AddPos(movement);

	}
	if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0) {
		AddRotation(rotation);
	}
}

XMFLOAT3 PlayerObject::HandleMovementInput(double dt) {
	XMFLOAT3 movement = XMFLOAT3(0, 0, 0);
	// Handle Left movement.
	if (_input->GetKeyUp('A')) {
		movement.x -= _moveSpeed * dt;
	}
	// Handle Right Movement
	if (_input->GetKeyUp('D')) {
		movement.x += _moveSpeed * dt;
	}
	// Handle Forward Movement
	if (_input->GetKeyUp('W')) {
		movement.z += _moveSpeed * dt;
	}
	// Handle Backwards Movement
	if (_input->GetKeyUp('S')) {
		movement.z -= _moveSpeed * dt;
	}

	//Handle Speed increase
	if (_input->GetKeyUp('E')) {
		_moveSpeed += 50;
	}
	//Handle Speed decrease
	if (_input->GetKeyUp('Q')) {
		_moveSpeed -= 50;
	}

	return movement;
}

XMFLOAT3 PlayerObject::HandleViewInput(double dt) {
	XMFLOAT3 rotation = XMFLOAT3(0, 0, 0);

	//Handle Viewports.
	if (_input->GetKeyUp('1')) { // Static Camera
		ChangeCamera(_staticCamera);
		_firstPersonView = false;
		_thirdPersonView = false;
	}
	if (_input->GetKeyUp('2')) { // Top Down Camera
		ChangeCamera(_topDownCamera);
		_firstPersonView = false;
		_thirdPersonView = false;
	}
	if (_input->GetKeyUp('3')) { // FPS Camera
		ChangeCamera(_firstPersonCamera);
		_firstPersonView = true;
		_thirdPersonView = false;
	}
	if (_input->GetKeyUp('4')) { // TPS Camera
		ChangeCamera(_thirdPersonCamera);
		_thirdPersonView = true;
		_firstPersonView = false;
	}

	// Handle Rotation input
	if (_input->GetKeyUp(VK_LEFT)) { // Left
		rotation.y -= _turnSpeed * dt;
	}
	if (_input->GetKeyUp(VK_RIGHT)) { // Right
		rotation.y += _turnSpeed * dt;
	}
	if (_input->GetKeyUp(VK_UP)) { // Up?
		rotation.x -= _turnSpeed * dt;
	}
	if (_input->GetKeyUp(VK_DOWN)) { // Down?
		rotation.x += _turnSpeed * dt;
	}

	// Handle rotation speed increase
	if (_input->GetKeyUp('C')) {
		_turnSpeed += 5;
	}
	// Handle rotation speed decrease
	if (_input->GetKeyUp('Z')) {
		_turnSpeed -= 5;
	}

	return rotation;
}

void PlayerObject::ChangeCamera(Camera* cam) {
	_window->SetCamera(cam);
	_currentCamera = cam;
}

