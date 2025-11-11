#pragma once
#include "ChiliTimer.h"
#include"Drawable.h"
#include"ImguiManager.h"
#include "Window.h"


#include<memory>
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
private:
	ImguiManager imgui; 
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawables; 
	static constexpr size_t nDrawables = 100; 
	

};