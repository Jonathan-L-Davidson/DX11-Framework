#pragma once
#include <d3d11_1.h>
#include <stdio.h>
#include <vector>
#include "DxDevice.h"
#include "TextureLoader.h"

using namespace DirectX;

class Texture {
private:
	LPCWSTR _id;
	ID3D11ShaderResourceView* _textureRSV;

public:
	
	Texture(LPCWSTR id);
	~Texture();


	Texture* LoadTexture(LPCWSTR FileName, ID3D11Device* device);

	Texture* GetTexture() { return this; };
	LPCWSTR GetID() { return _id; };
	ID3D11ShaderResourceView* GetRSV() { return _textureRSV; };
	ID3D11ShaderResourceView& GetRSVRef() { return *_textureRSV; };
};

class Shader {
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _vertexLayout;
	ID3D11SamplerState* _samplerLinear;

	HRESULT SetupSampler(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

public:
	Shader();
	~Shader();

	void Destroy();

	HRESULT Initialise(LPCWSTR name, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	HRESULT CompileShaderFromFile(LPCWSTR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	void SetShader(ID3D11DeviceContext* immediateContext);
};

class TextureManager {
private:
	DxDevice*				_device;

	std::vector<Shader*>*	_shaders;
	std::vector<Texture*>*	_textures;

public:
	TextureManager();
	~TextureManager();
	void Initialise();

	void CreateTexture(LPCWSTR id, LPCWSTR cat);
	void RemoveTexture(LPCWSTR id);
	Texture* GetTexture(LPCWSTR id);
	ID3D11ShaderResourceView* GetTextureRSV(LPCWSTR id) { return GetTexture(id)->GetRSV(); };

	void CreateShader(LPCWSTR id, LPCWSTR cat);
	void RemoveShader(LPCWSTR id);
	Shader* GetShader(LPCWSTR id);

	void SetDevice(DxDevice* device) { _device = device; };
};

