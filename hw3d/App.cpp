#include "App.h"


#include"GDIPlusManager.h"
#include"Sheet.h"
#include"Surface.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	int numberOfBoxes = 10; 

	//for (auto i = 0; i < numberOfBoxes; ++i)
	//{
	//	boxes.push_back(std::make_unique<Box>(
	//		wnd.Gfx(), rng, adist, ddist, odist, rdist
	//	));
	//}

	//int numberOfSheets = 100; 
	//for (auto i = 0; i < numberOfSheets; ++i)
	//{
	//	sheets.push_back(std::make_unique<Sheet>(
	//		wnd.Gfx(), rng, adist, ddist, odist, rdist
	//	));
	//}

	for (auto i = 0; i < numberOfBoxes; ++i)
	{
		skinnedBoxes.push_back(std::make_unique<SkinnedBox>(
			wnd.Gfx(), rng, adist, ddist, odist, rdist
		));
	}


	//const auto s = Surface::FromFile("images//kappa50.png");

	//hardcoded initial perspective matrix params: 
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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

App::~App()
{
}

void App::DoFrame()
{
	static int frameCount = 0; 

	auto dt = timer.Mark(); 

	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);

	for (auto& b : skinnedBoxes)
	//for (auto& s : sheets)
	{
		b->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt); //PAUSE if space is pressed
		b->Draw(wnd.Gfx());
		//s->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt); //PAUSE if space is pressed
		//s->Draw(wnd.Gfx());
	}

	wnd.Gfx().EndFrame();

	frameCount++; 
}