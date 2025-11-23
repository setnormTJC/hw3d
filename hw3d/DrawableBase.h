#pragma once

#include"Bindable.h"
#include "ConditionalNoexcept.h"
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

	static void AddStaticBind(std::unique_ptr<Bind::Bindable> bind) noxnd
	{
		assert("MUST use AddStaticIndexBuffer to bind  index buffer"
			&&
			typeid(*bind) != typeid(Bind::IndexBuffer));

		staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noxnd
	{
		assert(pIndexBuffer == nullptr);

		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
	
	void SetIndexFromStatic() noxnd
	{
		assert("Attempting to add index buffer a second time" 
			&& 
			pIndexBuffer == nullptr);

		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p; 
				return;
			}
		}

		assert("Failed to find index buffer in static binds" 
			&& 
			pIndexBuffer != nullptr);
	}

	const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds; 
	}

private: 
	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

/*Define the static var outside of the class (as required for static member vars)*/
template<class T> 
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;

