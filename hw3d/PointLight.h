#pragma once

#include"Graphics.h"
#include"SolidSphere.h"
#include"ConstantBuffers.h"

class PointLight //note that PointLight is NOT inheriting from bindable
{
public: 
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept; 
	void Reset() noexcept; 
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx) const noexcept; 

private: 
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos; 
		float padding; //no initialization??
	};
private: 
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f }; 
	/*The mesh here is going to act as a "lightbulb" - this makes the light source "obvious"*/
	mutable SolidSphere mesh; //mutable!!!
	mutable PixelConstantBuffer<PointLightCBuf> cbuf; //again, mutable!
};