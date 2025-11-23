#include "Sampler.h"

#include"GraphicsThrowMacros.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //POINT and ANISOTROPIC are alternatives
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;//a fun alternative: CLAMP instead of WRAP
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;  //not used unless 3D Texture 

		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(
			&samplerDesc, &pSampler
		));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
}