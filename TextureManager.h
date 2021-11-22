#pragma once
#include <d3d11_1.h>
#include <stdio.h>
#include <vector>

class Texture {
public:
	UINT id;
	
	Texture();
	~Texture();

	ID3D11ShaderResourceView* _textureRSV;
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

	HRESULT Initialise(WCHAR* name, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	void SetShader(ID3D11DeviceContext* immediateContext);
};

class TextureManager {
private:
	std::vector<Shader*>	_shaders;
	std::vector<Texture*>	_textures;

};

