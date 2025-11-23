#include "InputLayout.h"
#include "GraphicsThrowMacros.h"


namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
	{

		//const D3D11_INPUT_ELEMENT_DESC ied[] =
		//{
		//	//{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//	{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//	//{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
		//	//the macro there is automatic offsetting - more convenient 
		//};

		////LPCSTR SemanticName;
		////UINT SemanticIndex;
		////DXGI_FORMAT Format;
		////UINT InputSlot;
		////UINT AlignedByteOffset;
		////D3D11_INPUT_CLASSIFICATION InputSlotClass;
		////UINT InstanceDataStepRate;
		INFOMAN(gfx);

		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout
		(
			layout.data(), (UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}

	void InputLayout::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}