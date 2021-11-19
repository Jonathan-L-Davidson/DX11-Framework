#pragma once
#include <d3d11_1.h>
#include "GraphicManager.h"



class DxDevice {
private:
	GraphicManager*			_manager;

	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*			_pd3dDevice;
	ID3D11DeviceContext*	_pImmediateContext;
public:

	DxDevice();
	~DxDevice();
	void Destroy();

	HRESULT InitDevice();

	ID3D11DeviceContext*	GetDeviceContext()	{ return _pImmediateContext; };
	ID3D11Device*			GetDevice()			{ return _pd3dDevice; };

};

