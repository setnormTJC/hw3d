#pragma once
#include "Box.h"
#include"Camera.h"
#include "ChiliTimer.h"
#include"Drawable.h"
#include"ImguiManager.h"
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
	void SpawnSimulationWindow() noexcept; /*sim SPEED, that is*/
	void SpawnBoxWindowManagerWindow() noexcept; 
	void SpawnBoxWindows() noexcept; 
private:
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawables;
	
	static constexpr size_t nDrawables = 100;

	/*Adjusting an ImGui slider modifies this*/
	float speed_factor = 1.0f;
	
	Camera cam; 
	PointLight light; 

	std::vector<class Box*> boxes; //allows for varying optical properties of box
	/*Note that `boxes` ARE a subset of `drawables`*/

	std::optional<int> comboBoxIndex; //commonly known as a "dropdown list" 
	std::set<int> boxControlIds; 


};