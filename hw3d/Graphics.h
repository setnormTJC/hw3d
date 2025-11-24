#pragma once

//#include"Bindable.h"
#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include<DirectXMath.h>
#include<utility> //for std::pair (window width and height)
#include <vector>
#include <wrl.h>

#include "DxgiInfoManager.h"

namespace Bind
{
	class Bindable; //forward declaration for friendship without #including (circular dependency)
}

class Graphics
{
	friend class Bind::Bindable; 

public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	
	/*A function that I added*/
	static std::pair<int, int> getScreenWidthAndHeight() noexcept;

	void BeginFrame(float red, float green, float blue) noexcept; 
	void EndFrame();
	void EnableImgui() noexcept; 
	void DisableImgui() noexcept; 
	bool IsImguiEnabled() const noexcept; 

	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;

	void SetCamera(DirectX::FXMMATRIX cam) noexcept;

	/*Called by the Bindable TransformCBuf!*/
	DirectX::XMMATRIX GetCamera() const noexcept; 


	DirectX::XMMATRIX GetProjection() const noexcept;

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	/*A connection to the GPU. A "factory" that creates "resources"- or a workshop that builds tools - such as: 
	1) buffers
	2) textures
	3) shaders
	pDevice functions OFTEN begin with "Create..." and they return HRESULTs
	*/
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice; 

	/*Shows painting to user*/
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;

	/*A "conductor". The worker who uses tools built by pDevice (the GPU)
	Note that D3D12 and Vulkan allow "deferred" context - multithreaded draw commands
	Sets vertex and pixel shaders. 
	Sets primitive topology(points, lines, triangles)
	Sets layout of vertices (x, y, z, rgb, uv).
	Issues draw calls - uses resources created by pDevice
	Function names OFTEN include "Set" AND begin with an acronym of pipeline stage 
	Ex: VS -> vertex shader, RS -> rasterizer stage, OM -> output merger 
	*/
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	
	/*A canvas on which the GPU paints*/
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

	/*depth-stencil view - only used in Graphics constructor (as of Nov 7, 2025 commit)*/
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV; 

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;


	bool imguiEnabled = true; 

};