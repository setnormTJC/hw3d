#include "App.h"

#include <algorithm>
#include<random>

#include"Box.h"
#include"ChiliMath.h"
#include"GDIPlusManager.h"
#include"imgui/imgui.h"
#include"Sheet.h"
#include"SkinnedBox.h"
#include"Surface.h"



GDIPlusManager gdipm; //GLOBAL var here!


App::App()
	:
	wnd(1200, 900, "The Window title"), //previously 800 x 600
	//wnd(wnd.Gfx().getScreenWidthAndHeight().first, wnd.Gfx().getScreenWidthAndHeight().second,
	//	"The Window title"), 
	light(wnd.Gfx())
{
	class Factory
	{
	public: 
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{ }

		std::unique_ptr<Drawable> operator()()
		{
			/*The material (object)'s color*/
			const DirectX::XMFLOAT3 mat =
			{
				cdist(rng), //random red intensity between 0 and 1
				cdist(rng), //random green 
				cdist(rng) //etc.
			};

			return std::make_unique<Box>(gfx, rng,
				adist, ddist,
				odist, rdist,
				bdist, mat); 
		}

	private: 
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_real_distribution<float> cdist {0.0f, 1.0f}; //COLOR distribution (for each of r, g, and b)
	};

	Factory f(wnd.Gfx());

	drawables.reserve(nDrawables); 
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

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

	wnd.Gfx().BeginFrame(0.05f, 0.05f, 0.15f); 
	wnd.Gfx().SetCamera(cam.GetMatrix()); 

	light.Bind(wnd.Gfx(), wnd.Gfx().GetCamera()); 

	for (auto& d : drawables)
	{
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt); //PAUSE if space is pressed
		d->Draw(wnd.Gfx());
	}

	light.Draw(wnd.Gfx()); 

	static char buffer[1024]; 

	//control simulation speed with imgui!
	if (ImGui::Begin("Simulation speed")) //creates a window
	{
		ImGui::SliderFloat("Speed factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 
			1000.0f/ImGui::GetIO().Framerate, 
			ImGui::GetIO().Framerate);

		ImGui::InputText("Label", buffer, sizeof(buffer)); 
	}
	ImGui::End(); 

	cam.SpawnControlWindow();
	light.SpawnControlWindow(); 

	wnd.Gfx().EndFrame();

}

App::~App()
{
}
