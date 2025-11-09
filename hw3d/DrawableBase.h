#pragma once

#include"Bindable.h"
#include"Drawable.h"
#include"IndexBuffer.h"

#include<cassert>
#include<memory> 
#include<typeinfo>
#include<vector> 


template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized() noexcept //NOTE: static methods CANNOT be marked const
	{
		return !staticBinds.empty(); 
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
	{
		assert("MUST use AddStaticIndexBuffer to bind  index buffer"
			&&
			typeid(*bind) != typeid(IndexBuffer));

		staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept (!IS_DEBUG)
	{
		assert(pIndexBuffer == nullptr);

		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
	
	void SetIndexFromStatic() noexcept (!IS_DEBUG)
	{
		assert("Attempting to add index buffer a second time" 
			&& 
			pIndexBuffer == nullptr);

		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p; 
				return;
			}
		}

		assert("Failed to find index buffer in static binds" 
			&& 
			pIndexBuffer != nullptr);
	}

	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds; 
	}

private: 
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

/*Define the static var outside of the class (as required for static member vars)*/
template<class T> 
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

