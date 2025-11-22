#include "App.h"

#include <algorithm>
#include<random>

#include"AssTest.h"
#include"Box.h"
#include"ChiliMath.h"
#include"Cylinder.h"
#include"GDIPlusManager.h"
#include"imgui/imgui.h"
#include"Sheet.h"
#include"SkinnedBox.h"
#include"Surface.h"
#include "Pyramid.h"
#include "Vertex.h"

//#include<assimp/Importer.hpp>
//#include<assimp/scene.h>
//#include<assimp/postprocess.h>



GDIPlusManager gdipm; //GLOBAL var here!

//void f()
//{
//
//	VertexBuffer vb(std::move(
//		VertexLayout{}
//		.Append<VertexLayout::Position3D>()
//		.Append<VertexLayout::Normal>()
//		.Append<VertexLayout::Texture2D>()
//	));
//	vb.EmplaceBack(
//		DirectX::XMFLOAT3{ 1.0f,1.0f,5.0f },
//		DirectX::XMFLOAT3{ 2.0f,1.0f,4.0f },
//		DirectX::XMFLOAT2{ 6.0f,9.0f }
//	);
//	vb.EmplaceBack(
//		DirectX::XMFLOAT3{ 6.0f,9.0f,6.0f },
//		DirectX::XMFLOAT3{ 9.0f,6.0f,9.0f },
//		DirectX::XMFLOAT2{ 4.2f,0.0f }
//	);
//	auto pos = vb[0].Attr<VertexLayout::Position3D>();
//	auto nor = vb[0].Attr<VertexLayout::Normal>();
//	auto tex = vb[1].Attr<VertexLayout::Texture2D>();
//	vb.Back().Attr<VertexLayout::Position3D>().z = 420.0f;
//	pos = vb.Back().Attr<VertexLayout::Position3D>();
//
//	int a = 123; 
//}


App::App()
	:
	wnd(1200, 900, "The Window title"), //previously 800 x 600
	//wnd(wnd.Gfx().getScreenWidthAndHeight().first, wnd.Gfx().getScreenWidthAndHeight().second,
	//	"The Window title"), 
	light(wnd.Gfx())
{

	//f(); 

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

			case 2:
				return std::make_unique<Pyramid>(gfx, rng,
					adist, ddist, odist, rdist, tdist);

			case 3:
				return std::make_unique<SkinnedBox>(gfx, rng,
					adist, ddist, odist, rdist);

			case 4:
				return std::make_unique<AssTest>(gfx, rng,
					adist, ddist, odist, rdist, mat, 1.5f);

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

		wholeDistrib sdist{ 0, 4 }; //IMPORTANT "magic number" here for switch above!
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

	for (auto& pd : drawables)
	{
		if (auto pb = dynamic_cast<Box*> (pd.get())) //again, ASSIGNMENT used here!
		{
			boxes.push_back(pb); 
		}
	}


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

	SpawnSimulationWindow(); 
	cam.SpawnControlWindow();
	light.SpawnControlWindow(); 

	SpawnBoxWindowManagerWindow(); 

	SpawnBoxWindows(); 

	wnd.Gfx().EndFrame();

}

void App::SpawnSimulationWindow() noexcept
{
	//control simulation speed with imgui!
	if (ImGui::Begin("Simulation speed")) //creates a window
	{
		ImGui::SliderFloat("Speed factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("App average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);

		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;

		const std::string preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		//std optional has dereferencing operator 
		if (ImGui::BeginCombo("Box number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); ++i)
			{
				const bool selected = comboBoxIndex.has_value() && (*comboBoxIndex == i);
				//const bool selected = *comboBoxIndex == i; //bool expression here
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Spawn control window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset(); //not sure what this fellow does
		}
	}

	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	for (auto i = boxControlIds.begin(); i != boxControlIds.end();)
	{
		//remove the control window for a box if it is closed
		if (!boxes[*i]->SpawnControlWindow(*i, wnd.Gfx()))
		{
			i = boxControlIds.erase(i);  
		}
		else
		{
			i++; 
		}
	}
}

App::~App()
{
}
