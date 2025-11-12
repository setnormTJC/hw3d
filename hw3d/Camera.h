#pragma once
#include"Graphics.h"

class Camera
{
public: 
	DirectX::XMMATRIX GetMatrix() const noexcept; 
	/*Creates an imgui controller for camera*/
	void SpawnControlWindow() noexcept; 
	void Reset() noexcept; 
private: 
	float r = 20.0f; 
	float theta = 0.0f; //equatorial rotation 
	float phi = 0.0f; //polar rotation 
	float pitch = 0.0f; //orientation 
	float yaw = 0.0f; 
	float roll = 0.0f; 
};