#pragma once

#include"Bindable.h"
#include"Surface.h"
namespace Bind
{

	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const class Surface& s, unsigned int slot = 0);
		void Bind(Graphics& gfx) noexcept override;
	private: 
		unsigned int slot;
	protected:
		/*used by CreateShaderResourceView in Texture constructor */
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

	};

}