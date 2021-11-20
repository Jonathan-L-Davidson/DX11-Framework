#include "ObjectManager.h"

Object::Object() {
	_meshData = nullptr;
	_shader = nullptr;
}

Object::~Object() {
	Destroy();
}

void Object::Destroy() {
	_meshData = nullptr;
	_shader = nullptr;
}

void Object::LoadModel(MeshData* mesh) {
	_meshData = mesh;
}

void Object::LoadShader(Shader* shader) {
	_shader = shader;
}

void Object::Update() {

	UpdateWorldCoords();
	return;
}

void Object::Draw(ID3D11DeviceContext* immediateContext, XMMATRIX view, XMMATRIX projection, float t) {

	XMMATRIX world = GetWorld();

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(world);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	cb.mTime = t;

	cb.mLightDirection = XMFLOAT3(0.25f, 1.0f, 0.25f);

	cb.mDiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cb.mDiffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);

	cb.mAmbientLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cb.mAmbientMaterial = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cb.mSpecularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb.mSpecularMaterial = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cb.mSpecularPower = 10.0f;
	cb.mEyePosW = XMFLOAT3(0.0f, 5.0f, -30.0f);

//	immediateContext->PSSetShaderResources(0, 1, &_texture->GetTexture()); TODO: Setup texture object
	immediateContext->IASetVertexBuffers(0, 1, &_meshData->VertexBuffer, &_meshData->VBStride, &_meshData->VBOffset);
	immediateContext->IASetIndexBuffer(_meshData->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	immediateContext->DrawIndexed(_meshData->IndexCount, 0, 0);
}

void Object::UpdateWorldCoords() {
	// TODO: find an alternative way to set world coordinates for the object

	XMMATRIX scale = GetScale();
	XMMATRIX rotation = GetRotation();
	XMMATRIX position = GetPos();
	XMMATRIX world = GetWorld();

	XMStoreFloat4x4(&_world, scale * position * rotation * world); // Apply local values before multiplying it into the world coordinates.

	XMStoreFloat4x4(&_scale, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotation, XMMatrixIdentity());
	XMStoreFloat4x4(&_position, XMMatrixIdentity());

}