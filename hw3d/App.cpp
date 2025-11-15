#include "App.h"

#include <algorithm>
#include<random>

#include"Box.h"
#include"ChiliMath.h"
#include"Cylinder.h"
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
			
			switch (sdist(rng))
			{
			case 0: 
				return std::make_unique<Box>(gfx, rng,
					adist, ddist,
					odist, rdist,
					bdist, mat); 

			case 1: 
				return std::make_unique<Cylinder>(gfx, rng,
					adist, ddist, odist, rdist, bdist, tdist);

			default: 
				assert(false && "impossible drawable option in factory");
				return {};
			}

		}

	private: 
		using realDistrib = std::uniform_real_distribution<float>;
		using wholeDistrib = std::uniform_int_distribution<int>;
		
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };

		wholeDistrib sdist{ 0, 1 }; //IMPORTANT "magic number" here for switch above!
		realDistrib adist{ 0.0f,PI * 2.0f };
		realDistrib ddist{ 0.0f,PI * 0.5f };
		realDistrib odist{ 0.0f,PI * 0.08f };
		realDistrib rdist{ 6.0f,20.0f };
		realDistrib bdist{ 0.4f,3.0f };
		wholeDistrib latdist{ 5,20 };
		wholeDistrib longdist{ 10,40 };
		realDistrib cdist {0.0f, 1.0f}; //COLOR distribution (for each of r, g, and b)
		wholeDistrib tdist{ 3, 30 };
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
