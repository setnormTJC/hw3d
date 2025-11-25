#pragma once
#include"Graphics.h"

class Camera
{
public: 
	Camera() noexcept; 
	DirectX::XMMATRIX GetMatrix() const noexcept; 
	/*Creates an imgui controller for camera*/
	void SpawnControlWindow() noexcept; 
	void Reset() noexcept;

	void Rotate(float dx, float dy) noexcept; 
	void Translate(DirectX::XMFLOAT3 translation) noexcept; 

private: 
	DirectX::XMFLOAT3 pos; //xyz
	float pitch; 
	float yaw; 
	static constexpr float travelSpeed = 120.0f; //bumped up from 12 in Chili commit 
	static constexpr float rotationSpeed = 0.004f; //bumped up from 0.004
};