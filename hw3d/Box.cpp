#include "BindableBase.h"
#include "Box.h"
#include "Cube.h"
#include "GraphicsThrowMacros.h"
#include"imgui/imgui.h"
#include "Sphere.h"



Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material
)
	:
	TestObject(gfx, rng, adist, ddist, odist, rdist)
{

	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n; //normal vector for a particular pos
		};

		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();
		//modifies normals (which Vertex is composed of - along w. verts)

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"../x64/Debug/PhongVS.cso");
		//working directory is the folder that contains this cpp file (step up one, then into x64, etc.)
		auto pvsbc = pvs->GetBytecode(); //used by InputLayout below

		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"../x64/Debug/PhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,12, D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique <Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	}

	else
	{
		SetIndexFromStatic(); //prevents pIndexBuffer from being nullptr if multiple Drawables of same type are being drawn
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	materialConstants.color = material; //material is constructor param (called in App.cpp)

	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, materialConstants, 1u)); //note the 1u here!!!

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
	//the above is similar to:
	//mt = dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)); //won't work - 3 x 3 v 4 x 4
}

bool Box::SpawnControlWindow(int id, Graphics& gfx) noexcept
{
	bool dirty = false; //as in "dirty bit" 
	bool open = true; 
	using namespace std::string_literals;

	if (ImGui::Begin(("Box "s + std::to_string(id)).c_str(), &open)) //fancy thing here...
	{
		ImGui::Text("Material properties");
		const bool cd = dirty || ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
		const bool sid = dirty || ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", 2);
		const bool spd = dirty || ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2);

		dirty = cd || sid || spd; //specular power dirty

		//position updates do not affect "dirty" because `SyncMaterial` only modifies pixel shader (not vertex shader) 
		ImGui::Text("Position"); 
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%0.1f");
		/*add theta and phi controls, if desired*/
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		/*add pitch and yaw controls, if desired*/
	}
	ImGui::End(); 

	if (dirty)
	{
		SyncMaterial(gfx); 
	}

	return open; 
}

void Box::SyncMaterial(Graphics& gfx) noexcept(!IS_DEBUG)
{
	auto pConstPS = QueryBindable<MaterialCBuf>();

	assert(pConstPS != nullptr);

	pConstPS->Update(gfx, materialConstants);
}

