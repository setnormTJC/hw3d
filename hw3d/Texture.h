#pragma once

#include"Bindable.h"
#include"Surface.h"

class Texture : public Bindable
{
public: 
	Texture(Graphics& gfx, const class Surface& s);
	void Bind(Graphics& gfx) noexcept override; 

protected: 
	/*used by CreateShaderResourceView in Texture constructor */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView; 
	
};

