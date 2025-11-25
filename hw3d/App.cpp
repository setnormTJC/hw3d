

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
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1000.f / 1333.f, 0.5f, 40.0f));
	//Note that the second param here MUST match the params in wnd() above!

	//wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1440.0f/2560.0f, 1.0f, 0.5f, 40.0f));

	//NOTE: the hardcoded values above (1440, 2560) ONLY work for a monitor in PORTAIT mode with 1440p resolution
	// (this is my setup at home) 

}


void App::DoFrame()
{
	auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.07f, 0.05f, 0.15f);  //bgrd color 

	wnd.Gfx().SetCamera(cam.GetMatrix()); //altered from my prev commit
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	nano.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break; 
			//other cases ... 
		}

		if (!wnd.CursorEnabled())
		{
			if (wnd.kbd.KeyIsPressed('W'))
			{
				cam.Translate({ 0.0f,0.0f,dt });
			}
			if (wnd.kbd.KeyIsPressed('A'))
			{
				cam.Translate({ -dt,0.0f,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('S'))
			{
				cam.Translate({ 0.0f,0.0f,-dt });
			}
			if (wnd.kbd.KeyIsPressed('D'))
			{
				cam.Translate({ dt,0.0f,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('R'))
			{
				cam.Translate({ 0.0f,dt,0.0f });
			}
			if (wnd.kbd.KeyIsPressed('F'))
			{
				cam.Translate({ 0.0f,-dt,0.0f });
			}
		}

	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}

	/*ImGui stuff*/
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow("nanosuit");
	ShowRawInputWindow();


	wnd.Gfx().EndFrame();
}

void App::ShowRawInputWindow()
{
	while (const auto d = wnd.mouse.ReadRawDelta())
	{
		x += d->x;
		y += d->y;
	}

	if (ImGui::Begin("raw input"))
	{
		ImGui::Text("Tally: (%d,%d)", x, y);
		ImGui::Text("Cursor (press Esc to toggle): %s", wnd.CursorEnabled() ? "enabled" : "disabled");
		ImGui::Text("Raw input (press Esc to toggle): %s", wnd.mouse.RawEnabled() ? "enabled" : "disabled");
	}
	ImGui::End();
}

App::~App()
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



