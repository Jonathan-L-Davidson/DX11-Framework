#pragma once
#include "Object.h"
#include "Window.h"
#include "Time.h"

class PlayerObject : public Object {
private:
	Window* _window;
	Time*	_time;

	Camera* _currentCamera;
	Camera* _staticCamera;
	Camera* _topDownCamera;
	Camera* _firstPersonCamera;
	Camera* _thirdPersonCamera;

	void CheckInput();
	void ChangeCamera(Camera* cam);
	void ChangeCameraPos(Camera* cam, XMVECTOR pos);
	void ChangeCameraRot(Camera* cam, XMVECTOR rot);

public:
	PlayerObject(XMFLOAT3 pos);
	~PlayerObject();

	void SetWindow(Window* window)	{ _window = window; };
	void SetTime(Time* time)		{ _time = time; };

	void Update(double dt);
};

