#pragma once

#include "DrawableBase.h"
#include <random>
class Box : public DrawableBase<Box> //interesting template param! 
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

	void Update(float dt) noexcept override; 
	
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private: 
	float r; //radius 
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	float theta;
	float phi;
	float chi; 
	// speed (delta/s)

	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	//model tranform
	DirectX::XMFLOAT3X3 mt;
};

