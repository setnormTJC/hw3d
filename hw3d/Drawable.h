#pragma once

#include"Bindable.h"
#include "Graphics.h"
#include "IndexBuffer.h"

#include<DirectXMath.h>
#include <memory>
#include<vector>

class Drawable
{
	template <class T> 
	friend class DrawableBase; 

public: 
	Drawable() = default; 
	Drawable(const Drawable&) = delete; 

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0; 
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG); 
	virtual void Update(float dt) noexcept = 0;
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

	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept; 
	//neat forward declaration above of IndexBuffer
	
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const IndexBuffer* pIndexBuffer = nullptr; //this is a "workaround"
	std::vector<std::unique_ptr<Bindable>> binds; 
};

