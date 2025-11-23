#pragma once

#include"Bindable.h"
#include"GraphicsThrowMacros.h"


namespace Bind
{
	//templated because constant buffers will be user-defined
	// (ex: transformation matrix or passing certain colors to pixel shader) 
	template <typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		/*called by TransformCBuf -> fancy design*/
		void Update(Graphics& gfx, const C& consts) //the C here can be SLOTS to bind to in GPU (among other things) 
		{
			INFOMAN(gfx);

			D3D11_MAPPED_SUBRESOURCE msr;

			GFX_THROW_INFO(
				GetContext(gfx)->Map(   //new thing here 
					pConstantBuffer.Get(), 0u,
					D3D11_MAP_WRITE_DISCARD, 0u,
					&msr
				));


			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u); //another new thing
		}

		ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u)
			:
			slot(slot)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(consts);
			cbd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;

			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		}


		ConstantBuffer(Graphics& gfx, UINT slot = 0u)
			:
			slot(slot)
		{

			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(C); //a difference!
			cbd.StructureByteStride = 0u;

			/*no subresource data here!*/

			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));

		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		UINT slot; //example use: bind info on point light to slot 0, add color of a Drawable to slot 1
	};

	template <typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{

		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using Bindable::GetContext;
		//if these 2 using statements are commented out 
		//then errors get thrown on building
		//inheritance with templated classes causes GetContext to be undefined
		//(unless you have these "usings" or use this-> (or Bindable::)
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};


	template <typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using Bindable::GetContext;

	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};

}