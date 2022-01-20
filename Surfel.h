// Represents a surface element

#pragma once
#include <Cartesian3.h>

class Surfel
{
public:
	Cartesian3 position;
	float alpha, beta, gamma, u, v;
	Cartesian3 normal;

	// Safe constructor
	Surfel();
};

