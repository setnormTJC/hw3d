#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "Cube.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist, 
	std::uniform_real_distribution<float>& bdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))

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
		SetIndexFromStatic(); 
		//prevents pIndexBuffer from being nullptr if multiple Drawables of same type are being drawn
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this)); //NON-static

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}





DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	//*
	//	dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
