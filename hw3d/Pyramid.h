#pragma once

#include"TestObject.h"

class Pyramid : public TestObject<Pyramid>
{
public:
	Pyramid
	(Graphics& gfx, std::mt19937& rng,
		realDistrib& adist,
		realDistrib& ddist,
		realDistrib& odist,
		realDistrib& rdist,
		std::uniform_int_distribution<int>& tdist //not sure what this fellow is
	);
};