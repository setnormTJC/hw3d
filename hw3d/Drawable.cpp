
#include "Drawable.h"
#include"GraphicsThrowMacros.h"
#include"IndexBuffer.h"
#include<cassert>


using namespace Bind; 

void Drawable::Draw(Graphics& gfx) const noxnd
{
	for (auto& b : binds) //instance binds
	{
		b->Bind(gfx); //polymorphic call
	}
	
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx); 
	}

	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bind::Bindable> bind) noxnd
{
	assert("MUST use AddIndexBuffer to bind  index buffer"
		&&
		typeid(*bind) != typeid(Bind::IndexBuffer));

	binds.push_back(std::move(bind)); 
}

void Drawable::AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" 
		&& 
		pIndexBuffer == nullptr);

	pIndexBuffer = ibuf.get(); 
	binds.push_back(std::move(ibuf)); 
}
