#pragma once

#include "TestObject.h"
#include"ConstantBuffers.h"

#include <random>
class Box : public TestObject<Box> //interesting template param! 
//("Curiously recurring template pattern")
{
public: 

	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist, //angle
		std::uniform_real_distribution<float>& ddist, //delta? (how fast change in mvmt occurs?)
		std::uniform_real_distribution<float>& odist, //origin(al position)
		std::uniform_real_distribution<float>& rdist, //radius (box size scaling, perhaps)
		std::uniform_real_distribution<float>& bdist, //brightness? possibly
		DirectX::XMFLOAT3 material
	); 
	/*Returns false if window is closed*/
	bool SpawnControlWindow(int id, Graphics& gfx) noexcept; 
private: 
	void SyncMaterial(Graphics& gfx) noexcept (!IS_DEBUG);


	//DirectX::XMMATRIX GetTransformXM() const noexcept override;

private: 
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} materialConstants;

	using MaterialCBuf = PixelConstantBuffer<PSMaterialConstant>; 

	//model tranform
	DirectX::XMFLOAT3X3 mt;
};

