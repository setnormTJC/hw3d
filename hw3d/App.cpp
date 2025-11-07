#include "App.h"

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{
}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	static int frameCount = 0; 

	const float c = sin(timer.Peek()) / 2.0f + 0.5f; //varies clear (bgrd) color with time (for funsies) 
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	//wnd.Gfx().ClearBuffer(1.0f, c, 0.0f); //determines bgrd (clear) color 


	float mouseX = wnd.mouse.GetPosX() / 400.0f - 1.0f; 
	float mouseY = -1.0f*wnd.mouse.GetPosY() / 300.0f + 1.0f;

	wnd.Gfx().DrawTestTriangle(timer.Peek(), 0.0f, 0.0f);
	/*Draw a SECOND cube whose location is mouse-dependent:*/
	wnd.Gfx().DrawTestTriangle(timer.Peek(), mouseX, mouseY);
	wnd.Gfx().EndFrame();

	frameCount++; 
}