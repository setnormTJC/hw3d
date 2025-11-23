#pragma once

#include"ConditionalNoexcept.h"
#include "Graphics.h"

#include<DirectXMath.h>
#include <memory>
#include<vector>

namespace Bind
{
	class Bindable; 
	class IndexBuffer; 
}

class Drawable
{
	template <class T> 
	friend class DrawableBase; 

public: 
	Drawable() = default; 
	Drawable(const Drawable&) = delete; 

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0; 
	void Draw(Graphics& gfx) const noxnd; 
	virtual void Update(float dt) noexcept
	{ }
	virtual ~Drawable() = default; 
	
protected: 
	template<typename T> 
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			auto rawBindablePtr = pb.get(); 
			if (auto pt = dynamic_cast<T*>(rawBindablePtr)) //NOTE that the ASSIGNMENT operator is used here!
			{
				return pt; 
			}
		}
	};

	void AddBind(std::unique_ptr<Bind::Bindable> bind) noxnd;
	void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noexcept;
	//neat forward declaration above of IndexBuffer
	
private:
	virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr; //this is a "workaround"
	std::vector<std::unique_ptr<Bind::Bindable>> binds;
};

