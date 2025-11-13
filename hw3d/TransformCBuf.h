#pragma once

#include"ConstantBuffers.h"
#include"Drawable.h"
#include<DirectXMath.h>
#include<memory> 

class TransformCbuf : public Bindable
{
private: 
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProjection; 
		DirectX::XMMATRIX model; 
	};
public: 
	TransformCbuf(Graphics& gfx, const Drawable& parent); 
	void Bind(Graphics& gfx) noexcept override; 
private: 
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf; 
	const Drawable& parent; //fancy design here
};

