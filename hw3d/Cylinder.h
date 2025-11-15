#pragma once
#include "TestObject.h"
 
class Cylinder : public TestObject<Cylinder>
{
public: 
	Cylinder(Graphics& gfx, std::mt19937& rng,
		realDistrib& adist,
		realDistrib& ddist,
		realDistrib& odist,
		realDistrib& rdist,
		realDistrib& bdist,
		std::uniform_int_distribution<int>& tdist //not sure what this fellow is
	);

	//DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	//model tranform
	//DirectX::XMFLOAT3X3 mt;
	
};

