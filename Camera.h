#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera {
public:
	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

	Camera();
	~Camera();

	void SetEye(const XMVECTOR val) { Eye = val; };
	void SetPos(const XMVECTOR val) { At  = val; };
	void SetUp (const XMVECTOR val) { Up  = val; };
};

