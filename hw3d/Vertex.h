//This boy is a beast.

#pragma once

#include<type_traits>
#include<vector> 

#include"Graphics.h"

namespace hw3dexp
{
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
			BGRAColor, //same as struct above? (yes)
			Count
		};
		/*This fancy business is used to simplify `Vertex::Attr`*/
		template<ElementType> struct Map; //More template metaprogramming type stuff
		template<> struct Map<Position2D> //template specialization here 
		{
			using SysType = DirectX::XMFLOAT2; //SystemType
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position"; //maybe only mark as const? 
			//pointer cannot be constexpr (memory location cannot be known until runtime - OS decides what block to give)
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = hw3dexp::BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
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
					return sizeof(Map<Position2D>::SysType);

				case Position3D:
					return sizeof(Map<Position3D>::SysType);

				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);

				case Normal:
					return sizeof(Map<Normal>::SysType);

				case Float3Color:
					return sizeof(Map<Float3Color>::SysType);

				case Float4Color:
					return sizeof(Map<BGRAColor>::SysType);

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

			/*Gets a single layout element's description*/
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept(!IS_DEBUG)
			{
				switch (type)
				{
				case Position2D:
					return GenerateDesc<Position2D>(GetOffset());
				case Position3D:
					return GenerateDesc<Position3D>(GetOffset());
				case Texture2D:
					return GenerateDesc<Texture2D>(GetOffset());
				case Normal:
					return GenerateDesc<Normal>(GetOffset());
				case Float3Color:
					return GenerateDesc<Float3Color>(GetOffset());
				case Float4Color:
					return GenerateDesc<Float4Color>(GetOffset());
				case BGRAColor:
					return GenerateDesc<BGRAColor>(GetOffset());
				}
				assert("Invalid element type" && false);
				return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}


		private:
			/*Helper called by GetDesc*/
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcept(!IS_DEBUG)
			{
				return { Map<type>::semantic,0,Map<type>::dxgiFormat,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
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

		VertexLayout& Append(ElementType type) noexcept (!IS_DEBUG)
		{
			elements.emplace_back(type, Size());
			return *this;
		}
		/* @returns the size of a SINGLE vertex*/
		size_t Size() const noexcept(!IS_DEBUG)
		{
			return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}
		size_t GetElementCount() const noexcept
		{
			return elements.size();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept(!IS_DEBUG)
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc; 

			desc.reserve(GetElementCount());
			
			for (const auto& e : elements)
			{
				desc.push_back(e.GetDesc());
			}

			return desc; 
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

			
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}

		/*@param val - this will be source (Src) in `SetAttribute`*/
		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG) //&& enables "perfect forwarding"
		{
			using namespace DirectX;
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val)); //XMFLOAT2 is an example of Dest in SetAttribute
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

	protected:
		Vertex(char* pData, const VertexLayout& layout) noexcept(!IS_DEBUG)
			:
			pData(pData),
			layout(layout)
		{
			assert(pData != nullptr);
		}

	private:
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...); //when i = 1, base case is hit and
			//the PUBLIC SetAttributeByIndex is called (or something like that) 
		}

		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept(!IS_DEBUG)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
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

	/*Can only READ the data of vertices - COMPOSED of a vertex (interestingly) - not inheriting*/
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) noexcept (!IS_DEBUG)
			:
			vertex(v)
		{
		}
		template<VertexLayout::ElementType Type>
		const auto& Attr() const noexcept (!IS_DEBUG)
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}

	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noexcept(!IS_DEBUG)
			:
			layout(std::move(layout))
		{
		}

		const char* GetData() const noexcept(!IS_DEBUG)
		{
			return buffer.data(); 
		}
		
		/*@returns the size, in bytes, of the entire vertex buffer*/
		size_t SizeBytes() const noexcept(!IS_DEBUG)
		{
			return buffer.size(); 
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

		Vertex operator[] (size_t i) noexcept (!IS_DEBUG)
		{
			assert(i < Size());
			return Vertex{ buffer.data() + layout.Size() * i, layout }; //also, calcs here? 
		}

		/*Const overloads of the 3 above*/
		ConstVertex Back() const noexcept (!IS_DEBUG)
		{
			return const_cast<VertexBuffer*>(this)->Back();
		}

		ConstVertex Front() const noexcept (!IS_DEBUG)
		{
			return const_cast<VertexBuffer*>(this)->Front();
		}

		ConstVertex operator[] (size_t i) const noexcept (!IS_DEBUG)
		{
			//assert(i < Size());
			return const_cast<VertexBuffer&>(*this)[i];
		}

	private:
		std::vector<char> buffer; //byte representation of all vertices
		VertexLayout layout; //describes the structure of vertex
	};
}