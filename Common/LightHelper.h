//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include <xnamath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLightOptimized
{
	PointLightOptimized() : 
		lightColour(0.0f,0.0f,0.0f,0.0f), 
		pos(0.0f,0.0f,0.0f), 
		range(0.0f), 
		att(0.0f, 0.0f, 0.0f), 
		pad(0.0f)
	{}

	XMFLOAT4 lightColour;
	XMFLOAT3 pos;
	float range;

	XMFLOAT3 att;
	float pad;
};

struct SpotLightOptimized
{
	SpotLightOptimized() : 
		lightColour(0.0f,0.0f,0.0f,0.0f), 
		pos(0.0f,0.0f,0.0f), 
		range(0.0f), 
		direction(0.0f, 0.0f, 0.0f),
		spot(0.0f),
		att(0.0f, 0.0f, 0.0f), 
		pad(0.0f)
	{}



	XMFLOAT4 lightColour;
	XMFLOAT3 pos;
	float range;

	XMFLOAT3 direction;
	float spot;

	XMFLOAT3 att;
	float pad;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
};

#endif // LIGHTHELPER_H