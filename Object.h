#pragma once
#include <DirectXMath.h>
#include "OBJLoader.h"
#include "TextureManager.h"

class Object {
private:
	UINT _id;

	XMFLOAT4X4	_position;
	XMFLOAT4X4	_rotation;
	XMFLOAT4X4	_scale;
	MeshData* _meshData;
	Shader* _shader;
	Texture* _texture;

	XMFLOAT4X4	_world;
public:
	Object();
	~Object();

	void Destroy();
	void LoadModel(MeshData* mesh);
	void LoadTexture(Texture* texture);
	void LoadShader(Shader* shader);

	virtual void Update(double dt);
	void Draw(ID3D11DeviceContext* immediateContext, ID3D11Buffer* constantBuffer, XMFLOAT4X4 view, XMFLOAT4X4 projection, float t);

	void UpdateWorldCoords();

public:
	///
	// Position helpers!
	///
	void AddPos(XMFLOAT3 pos) {
		XMFLOAT4X4 fPos = GetPos();
		XMMATRIX bufferPos = XMLoadFloat4x4(&fPos); // Make an old copy of the current position.
		XMStoreFloat4x4(&_position, XMMatrixTranslation(pos.x, pos.y, pos.z) * bufferPos); // Multiply the old matrice ontop of the old one..?
		// TO DO, check if this works!
		// It works!!
	};

	void SetPos(XMFLOAT3 pos) { XMStoreFloat4x4(&_position, XMMatrixTranslation(pos.x, pos.y, pos.z)); };
	XMFLOAT4X4 GetPos() { return _position; };
	///
	// Rotation Helpers!
	///
	void AddRotation(XMFLOAT3 rotation) {
		XMFLOAT4X4 fRot = GetRotation();
		XMMATRIX bufferRot = XMLoadFloat4x4(&fRot);
		XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * bufferRot);
	};

	void SetRotation(XMFLOAT3 rotation) { XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)); };
	XMFLOAT4X4 GetRotation() { return _rotation; };

	///
	// Scale Helpers!
	///
	void AddScale(XMFLOAT3 scale) {
		XMFLOAT4X4 fScale = GetScale();
		XMMATRIX bufferScale = XMLoadFloat4x4(&fScale);
		XMStoreFloat4x4(&_scale, XMMatrixScaling(scale.x, scale.y, scale.z) * bufferScale);
	};

	void SetScale(XMFLOAT3 scale) { XMStoreFloat4x4(&_scale, XMMatrixScaling(scale.x, scale.y, scale.z)); };
	XMFLOAT4X4 GetScale() { return _scale; };

	XMFLOAT4X4 GetWorld() { return _world; };


	void SetID(UINT id) { _id = id; };
};
