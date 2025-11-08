#pragma once
#include "Graphics.h"

class Bindable
{
public: 
	/*Overridden by children - ex: Topology calls IASetPrimitiveTopology*/
	virtual void Bind(Graphics& gfx) noexcept = 0; 
	virtual ~Bindable() = default;  //Bindable will have children

protected: 
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept; 
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	/*Related to the "INFOMAN" macro defined in "GraphicsThrowMacros.h"*/
	static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept(!(IS_DEBUG));

};

