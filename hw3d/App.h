#pragma once
#include "Window.h"
#include "ChiliTimer.h"

#include<memory>
#include<vector> 

#include"Box.h"
#include"Cube.h"
#include"SkinnedBox.h"
#include"Sheet.h"

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
	std::vector<std::unique_ptr<class Box>> boxes; //app is composed of stuff to draw
	std::vector < std::unique_ptr<class SkinnedBox>> skinnedBoxes; 

	std::vector < std::unique_ptr<class Sheet>> sheets; 

};