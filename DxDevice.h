#pragma once
#include <d3d11_1.h>

class DxDevice {

	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*			_pd3dDevice;
	ID3D11DeviceContext*	_pImmediateContext;

};

