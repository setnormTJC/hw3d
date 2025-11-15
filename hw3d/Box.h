#pragma once

#include "TestObject.h"
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

	//void Update(float dt) noexcept override; 
	
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private: 
	//model tranform
	DirectX::XMFLOAT3X3 mt;
};

