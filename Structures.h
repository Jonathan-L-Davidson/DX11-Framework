#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>

struct SimpleVertexLegacy {
	XMFLOAT3 Pos;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Normal;

	bool operator<(const SimpleVertex other) const {
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	}
};

struct ConstantBuffer {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	float mTime;

	XMFLOAT3 mLightDirection;
	XMFLOAT4 mDiffuseMaterial;
	XMFLOAT4 mDiffuseLight;

	XMFLOAT4 mAmbientMaterial;
	XMFLOAT4 mAmbientLight;

	XMFLOAT4 mSpecularMaterial;
	XMFLOAT4 mSpecularLight;

	float mSpecularPower;

	XMFLOAT3 mEyePosW; // Camera position in world space
};