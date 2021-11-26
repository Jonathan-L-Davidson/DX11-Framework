#include <d3dcompiler.h>
#include <stdio.h>
#include <string>
#include "TextureManager.h"



Shader::Shader() {
	_vertexShader = nullptr;
	_pixelShader = nullptr;
	_vertexLayout = nullptr;
	_samplerLinear = nullptr;
};

Shader::~Shader() {
	Destroy();
}

void Shader::Destroy() {
	if (_vertexShader) _vertexShader->Release();
	if (_pixelShader) _pixelShader->Release();
	if (_vertexLayout) _vertexLayout->Release();
	if (_samplerLinear) _samplerLinear->Release();
}

HRESULT Shader::Initialise(DWORD* name, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	HRESULT hr = S_OK;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;

	hr = CompileShaderFromFile((wchar_t*)name, "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr)) {
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_vertexShader);

	if (FAILED(hr))	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile((wchar_t*)name, "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pixelShader);
	pPSBlob->Release();

	if (FAILED(hr)) {
		pPSBlob->Release();
		return hr;
	}

	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_vertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Set the input layout
	immediateContext->IASetInputLayout(_vertexLayout);

	return hr;
}


HRESULT Shader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		if (pErrorBlob) pErrorBlob->Release();

		return hr;
	}

	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


HRESULT Shader::SetupSampler(ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	HRESULT hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &_samplerLinear);

	if (FAILED(hr))
		return hr;

	immediateContext->PSSetSamplers(0, 1, &_samplerLinear);

	return S_OK;
};

Texture::Texture(DWORD id) {
	_id = id;
	_textureRSV = nullptr;
}

Texture::~Texture() {
	delete _textureRSV;
}

Texture* Texture::LoadTexture(DWORD FileName, ID3D11Device* device) {
	CreateDDSTextureFromFile(device, (wchar_t*)FileName, nullptr, &_textureRSV);

	return this;
}

TextureManager::TextureManager() {
	_textures = nullptr;
	_shaders = nullptr;

	_device = nullptr;
}

void TextureManager::Initialise() {
	_shaders = new std::vector<Shader*>();
	_textures = new std::vector<Texture*>();
}

TextureManager::~TextureManager() {
	_shaders->clear();
	delete _shaders;

	_textures->clear();
	delete _textures;
}

void TextureManager::CreateTexture(DWORD id, DWORD cat) {
	Texture* tex = new Texture(id);
	
	DWORD fileName = cat + *L"/" + id + *L".dds";
	tex->LoadTexture(fileName, _device->GetDevice());

	if (tex->GetTexture() == NULL) {
		delete tex;
		return;
	}
	_textures->push_back(tex);
}

void TextureManager::RemoveTexture(DWORD id) {
	for (UINT i = 0; i < _textures->size(); i++) {
		if (id == _textures->at(i)->GetID()) { // If ID is equal to the texture's ID.
			_textures->erase(_textures->begin() + i);
		}
	}
}

Texture* TextureManager::GetTexture(DWORD id) {
	
	for (UINT i = 0; i < _textures->size(); i++) {
		if (id == _textures->at(i)->GetID()) { // If ID is equal to the texture's ID.
			//std::cout << "GetTexture() passed\n";
			return _textures->at(i);
		}
	}

	CreateTexture(id, *L"Textures");
	return GetTexture(id); // Return the 1st texture loaded.
}

//void TextureManager::CreateShader(WCHAR id, WCHAR cat) {
//	Shader* shad = new Shader(id);
//
//	WCHAR fileName = cat + *L"/" + id + *L".dds";
//	shad->LoadShader(fileName, _device);
//
//	if (shad->GetShader() == NULL) {
//		delete shad;
//		return;
//	}
//	_textures->push_back(tex);
//}
//
//void TextureManager::RemoveShader(WCHAR id) {
//	for (int i = 0; i < _shaders->size(); i++) {
//		if (id == _shaders->at(i)->GetID()) { // If ID is equal to the texture's ID.
//			_shaders->erase(_shaders->begin() + i);
//		}
//	}
//}
//
//Shader* TextureManager::GetShader(WCHAR id) {
//
//	std::cout << "GetTexture(" << id << ", " << cat << ") started." << std::endl;
//
//	Shader* result;
//
//	for (int i = 0; i < _shaders->size(); i++) {
//		if (id == _shaders->at(i)->GetID()) { // If ID is equal to the texture's ID.
//			std::cout << "GetTexture() passed\n";
//			result = _shaders->at(i);
//		}
//	}
//	if (!result || !wcslen((wchar_t*)result->GetID())) { // For some reason, WCHAR is a typedefinition for wchar_t, YET IT'S NOT A WCHAR_T???
//		result = CreateShader(id);
//		return _shaders->at(0); // Return the 1st texture loaded.
//	}
//
//	return result;
//}
