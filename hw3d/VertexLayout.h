#pragma once

#include<vector> 
#include<DirectXMath.h>
#include<type_traits>

struct BGRAColor
{
	unsigned char a; 
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class VertexLayout
{
public: 
	enum ElementType //no enum class? 
	{
		Position2D, 
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		BGRAColor, //same as struct above?
	};

	class Element
	{
	public: 
		Element(ElementType type, size_t offset)
			:
			type(type),
			offset(offset)
		{
		}

		size_t GetOffsetAfter() const noexcept(!IS_DEBUG)
		{
			return offset + Size(); 
		}
		size_t GetOffset() const
		{
			return offset; 
		}
		size_t Size() const noexcept (!IS_DEBUG)
		{
			return SizeOf(type); 
		}
		static constexpr size_t SizeOf(ElementType type) noexcept (!IS_DEBUG)
		{
			using namespace DirectX; 

			switch (type)
			{
			case Position2D: 
				return sizeof(XMFLOAT2);

			case Position3D: 
				return sizeof(XMFLOAT3);

			case Texture2D: 
				return sizeof(XMFLOAT2);

			case Normal: 
				return sizeof(XMFLOAT3);

			case Float3Color: 
				return sizeof(XMFLOAT3);

			case Float4Color: 
				return sizeof(XMFLOAT3); //really?

			case BGRAColor: 
				return sizeof(unsigned int); 
			}

			assert("Invalid element type" && false); 
			return 0u; 
		}
		ElementType GetType() const noexcept
		{
			return type; 
		}

	private: 
		ElementType type;
		//number of bytes from beginning of vertex (ex: if Position3D then Texture2D, Texture2D offset is 4 x 3 Bytes 
		size_t offset; 
	};

public: 
	template<ElementType Type> 
	const Element& Resolve() const noexcept (!IS_DEBUG)
	{
		for (auto& e : elements)
		{
			if (e.GetType() == Type) //comparing to a template param here 
			{
				return e; 
			}
		}
		assert("Could not resolve element type" && false);
		return elements.front(); 
	}
	const Element& ResolveByIndex(size_t i) const noexcept(!IS_DEBUG)
	{
		return elements[i];
	}

	template<ElementType Type> 
	VertexLayout& Append() noexcept (!IS_DEBUG)
	{
		elements.emplace_back(Type, Size());
		return *this;
	}
	size_t Size() const noexcept(!IS_DEBUG)
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter(); 
	}
	size_t GetElementCount() const noexcept
	{
		return elements.size(); 
	}
private: 
	std::vector<Element> elements; 

};


/*This Vertex is a "view type" (a "proxy")*/
class Vertex
{
	friend class VertexBuffer;
public: 
	template<VertexLayout::ElementType Type>
	auto& Attr() noexcept(!IS_DEBUG)
	{
		using namespace DirectX; 
		const auto& element = layout.Resolve<Type>(); 
		auto pAttribute = pData + element.GetOffset(); 

		//if constexpr is a speedup method - only gets evaluated at compile time 
		if constexpr (Type == VertexLayout::Position2D)
		{
			return *reinterpret_cast<XMFLOAT2*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::Position3D)
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::Texture2D)
		{
			return *reinterpret_cast<XMFLOAT2*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::Normal)
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::Float3Color)
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::Float4Color)
		{
			return *reinterpret_cast<XMFLOAT4*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::BGRAColor)
		{
			return *reinterpret_cast<BGRAColor*>(pAttribute);
		}
		else
		{
			assert("Bad element type" && false);
			return *reinterpret_cast<char*>(pAttribute);
		}
	}

	/*
	* @param val - this will be source (Src) in `SetAttribute`
	*/
	template<typename T> 
	void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG) //&& enables "perfect forwarding"
	{
		using namespace DirectX; 
		const auto& element = layout.ResolveByIndex(i); 
		auto pAttribute = pData + element.GetOffset(); 
		switch (element.GetType())
		{
		case VertexLayout::Position2D:
			SetAttribute<XMFLOAT2>(pAttribute, std::forward<T>(val)); //XMFLOAT2 is an example of Dest in SetAttribute
			break;
		case VertexLayout::Position3D:
			SetAttribute<XMFLOAT3>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Texture2D:
			SetAttribute<XMFLOAT2>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Normal:
			SetAttribute<XMFLOAT3>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float3Color:
			SetAttribute<XMFLOAT3>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float4Color:
			SetAttribute<XMFLOAT4>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::BGRAColor:
			SetAttribute<BGRAColor>(pAttribute, std::forward<T>(val));
			break;
		default:
			assert("Bad element type" && false);
		}
	}

private: 
	//private constructor - ONLY VertexBuffer (declared as a friend) can access 
	Vertex(char* pData, const VertexLayout& layout) noexcept(!IS_DEBUG) 
		:
		pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}

	template<typename First, typename ...Rest>
	void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
	{
		SetAttributeByIndex(i, std::forward<First>(first)); 
		SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...); //when i = 1, base case is hit and
		//the PUBLIC SetAttributeByIndex is called (or something like that) 
	}

	template<typename Dest, typename Src>
	void SetAttribute(char* pAttribute, Src&& val) noexcept(!IS_DEBUG)
	{
		if constexpr (std::is_assignable<Dest, Src>::value)
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			assert("Parameter attribute type mismatch" && false);
		}
	}

private: 
	char* pData = nullptr; //pointer to beginning of vertex
	const VertexLayout& layout; 
};

class VertexBuffer
{
public: 
	VertexBuffer(VertexLayout layout) noexcept(!IS_DEBUG)
		:
		layout(std::move(layout))
	{
	}
	const VertexLayout& GetLayout() const noexcept
	{
		return layout;
	}
	//Size is in VERTICES (not bytes) 
	size_t Size() const noexcept (!IS_DEBUG)
	{
		return buffer.size() / layout.Size(); //?
	}
	//accepts ANY number of args of ANY type!
	//This is template recursion
	//It will allow Vertex structs of various sizes
	//ex: ONLY a position component versus a position component, surface normal, and texture 
	template<typename ...Params>
	void EmplaceBack(Params&&... params) noexcept (!IS_DEBUG) 
	{
		assert(sizeof...(params) == layout.GetElementCount() &&
			"param count doesn't match number of elements in layout");

		buffer.resize(buffer.size() + layout.Size());
		Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
	}
	Vertex Back() noexcept (!IS_DEBUG)
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout }; //calcs here? 
	}
	Vertex Front() noexcept (!IS_DEBUG)
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(), layout };
	}
	//read-only overload of subscript operator (no write)
	Vertex operator[] (size_t i) noexcept (!IS_DEBUG)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i, layout }; //also, calcs here? 
	}

private: 
	std::vector<char> buffer; //byte of all vertices
	VertexLayout layout; //describes the structure of vertex
};