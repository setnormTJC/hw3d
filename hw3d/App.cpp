

#include "App.h"

#include <algorithm>
#include<random>

#include"AssTest.h"
#include"ChiliMath.h"
#include"GDIPlusManager.h"
#include"imgui/imgui.h"
#include"Sheet.h"
#include"Surface.h"
#include "Vertex.h"


GDIPlusManager gdipm; //GLOBAL var here!

App::App()
	:
	wnd(1200, 900, "The Window title"), //previously 800 x 600
	//wnd(wnd.Gfx().getScreenWidthAndHeight().first, wnd.Gfx().getScreenWidthAndHeight().second,
	//	"The Window title"), 
	light(wnd.Gfx())
{



	//hardcoded initial perspective matrix params: 
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	
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
	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		dx::XMMatrixTranslation(pos.x, pos.y, pos.z);

	nano.Draw(wnd.Gfx(), transform);

	light.Draw(wnd.Gfx()); 

	cam.SpawnControlWindow();
	light.SpawnControlWindow(); 

	ShowModelWindow(); 


	wnd.Gfx().EndFrame();

}

void App::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}



App::~App()
{
}
