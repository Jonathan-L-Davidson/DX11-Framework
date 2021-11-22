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

	void SetEye(XMVECTOR);
	void SetPos(XMVECTOR);
	void SetUp(XMVECTOR);
};

