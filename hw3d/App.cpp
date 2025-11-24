

#include "App.h"

#include <algorithm>
#include<random>

#include"ChiliMath.h"
#include"GDIPlusManager.h"
#include"imgui/imgui.h"
#include"Surface.h"
#include "Vertex.h"


GDIPlusManager gdipm; //GLOBAL var here!

App::App()
	:
	wnd(1333, 1000, "The Window title"), //previously 800 x 600
	//wnd(wnd.Gfx().getScreenWidthAndHeight().first, wnd.Gfx().getScreenWidthAndHeight().second,
	//	"The Window title"), 
	light(wnd.Gfx())
{



	//hardcoded initial perspective matrix params: 
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1000.f/1333.f, 0.5f, 40.0f));
	//Note that the second param here MUST match the params in wnd() above!
	
	//wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1440.0f/2560.0f, 1.0f, 0.5f, 40.0f));
	
	//NOTE: the hardcoded values above (1440, 2560) ONLY work for a monitor in PORTAIT mode with 1440p resolution
	// (this is my setup at home) 
	
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
	auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.07f, 0.05f, 0.15f);  //bgrd color 
	wnd.Gfx().SetCamera(cam.GetMatrix()); 

	light.Bind(wnd.Gfx(), wnd.Gfx().GetCamera()); 

	namespace dx = DirectX; 

	nano.Draw(wnd.Gfx());

	light.Draw(wnd.Gfx()); 
	cam.SpawnControlWindow();
	light.SpawnControlWindow(); 

	nano.ShowWindow("nanosuit");

	wnd.Gfx().EndFrame();

}




App::~App()
{
}
