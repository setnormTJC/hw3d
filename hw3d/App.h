#pragma once
#include"Camera.h"
#include "ChiliTimer.h"
#include"Drawable.h"
#include"ImguiManager.h"
#include"Mesh.h"
#include"PointLight.h"
#include "Window.h"

#include<memory>
#include<set> //allows multiple imgui control windows for separate boxes that get drawn 
#include<vector> 



class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ShowModelWindow(); 
private:
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;

	/*Adjusting an ImGui slider modifies this*/
	float speed_factor = 1.0f;

	Camera cam;
	PointLight light;

	std::vector<class Box*> boxes; //allows for varying optical properties of box
	/*Note that `boxes` ARE a subset of `drawables`*/

	Model nano{ wnd.Gfx() , "Models\\nanosuit.obj"};

	struct
	{
		float roll = 0.0f; 
		float pitch = 0.0f; 
		float yaw = 0.0f; 
		float x = 0.0f; 
		float y = 0.0f; 
		float z = 0.0f; 
	} pos;
};