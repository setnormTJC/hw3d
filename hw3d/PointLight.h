#pragma once

#include"Graphics.h"
#include"SolidSphere.h"
#include"ConstantBuffers.h"

class PointLight //note that PointLight is NOT inheriting from bindable (but it DOES have a Bind function ...)
{
public: 
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept; 
	void Reset() noexcept; 
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept; 

private: 
	struct PointLightCBuf
	{
		alignas(16)DirectX::XMFLOAT3 pos; //alignas used!
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;

		float attConst; //attenuation constant 
		float attLin; //linear
		float attQuad; //quadratic 
	};
private: 
	//DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f }; 
	PointLightCBuf cbData; 
	
	/*The mesh here is going to act as a "lightbulb" - this makes the light source "obvious"*/
	mutable SolidSphere mesh; //mutable!!!
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf; //again, mutable!
};