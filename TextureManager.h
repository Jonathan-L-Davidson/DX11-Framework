#pragma once

class Texture {
private:
	ID3D11ShaderResourceView* _texture;
public:
	UINT id;
	
	Texture();
	~Texture();

	ID3D11ShaderResourceView* GetTexture() { return _texture; };
};

//class TextureManager {
//	
//};

