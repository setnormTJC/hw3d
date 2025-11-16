#include "Cylinder.h"
#include "Prism.h"
#include"BindableBase.h"

Cylinder::Cylinder(Graphics& gfx, std::mt19937& rng, realDistrib& adist, realDistrib& ddist,
	realDistrib& odist, realDistrib& rdist, realDistrib& bdist,
	std::uniform_int_distribution<int>& tdist)
	:
	TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;
	if (!IsStaticInitialized())
	{

		auto pvs = std::make_unique<VertexShader>(gfx, L"../x64/Debug/PhongVS.cso");
		//working directory is the folder that contains this cpp file (step up one, then into x64, etc.)
		auto pvsbc = pvs->GetBytecode(); //used by InputLayout below

		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"../x64/Debug/IndexedPhongPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,12, D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique <Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			dx::XMFLOAT3A color[6] =  //'A' means aligned 
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f},
				{1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f}
			};

			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
		} matConst;

		//colorConst.color = material;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, matConst, 1u)); //note the 1u here!!!

	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
	};

	auto index = tdist(rng); 

	const auto model = Prism::MakeTesselatedIndependentCapNormals<Vertex>(index);

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));


	AddBind(std::make_unique<TransformCbuf>(gfx, *this)); //NON-static

}
