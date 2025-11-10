#pragma once
#include "Window.h"
#include "ChiliTimer.h"

#include<memory>
#include<vector> 

#include"Drawable.h"

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
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawables; 
	static constexpr size_t nDrawables = 100; 
	

};