// EngineLib/Rendering/Renderer.h
#pragma once

// D3D headers
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

#include "VertexLayout.h"

class URenderer final
{
public:
	// Constant buffer
	struct alignas(16) FConstants
	{
		FMatrix WorldMatrix;
		FMatrix ViewMatrix;
	};

	URenderer(HWND hWindow);
	~URenderer();
	void Release();

	void ResizeSwapChain(uint32 width, uint32 height);
	
	ID3D11Buffer* CreateVertexBuffer(const FVertexSimple* vertices, UINT byteWidth);
	void ReleaseVertexBuffer(ID3D11Buffer& vertexBuffer);
	void SwapBuffer();
	void Prepare();
	void PrepareShader();
	void BindViewportRTV(
		ID3D11RenderTargetView& viewportRTV, 
		ID3D11DepthStencilView& viewportDSV,
		float32 width, float32 height);
	void RenderPrimitive(ID3D11Buffer& vertexBuffer, UINT numVertices);
	void UpdateConstant(const FMatrix& worldMatrix, const FMatrix& viewMatrix);

	/* Getter */
	ID3D11Device* GetDevice() const { return mDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext; }

	// Temp
	ImVec2 GetViewportSize() const { return mViewportSize; }
private:
	void createDeviceAndSwapChain(HWND hWindow);
	void releaseDeviceAndSwapChain();
	void createFrameBuffer();
	void createDepthStencil();
	void releaseFrameBuffer();
	void createRaterizerState();
	void releaseRasterizerState();
	void createShader();
	void releaseShader();
	void createConstantBuffer();
	void releaseConstantBuffer();

private:
	// Pointers to manage Direct3D 11 device, device context, and swap chain
	ID3D11Device* mDevice = nullptr; // Direct3D device to communicate with GPU
	ID3D11DeviceContext* mDeviceContext = nullptr; // Context to execute the GPU instructions
	IDXGISwapChain* mSwapChain = nullptr; // Swap chain to swap the frame buffer

	// Variables to manage the resources and the state for rendering
	ID3D11Texture2D* mFrameBuffer = nullptr;
	ID3D11RenderTargetView* mFrameBufferRTV = nullptr;
	ID3D11RasterizerState* mRasterizerState = nullptr;
	ID3D11Buffer* mConstantBuffer = nullptr;
	ID3D11RenderTargetView* mCurrentViewportBufferRTVRef = nullptr;

	FLOAT mClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
	D3D11_VIEWPORT mViewportInfo;

	// Shader
	ID3D11VertexShader* mSimpleVertexShader;
	ID3D11PixelShader* mSimplePixelShader;
	ID3D11InputLayout* mSimpleInputLayout;
	unsigned int mStride;

	/* Internal State */
	bool mIsReleased;

	// Temp
	ImVec2 mViewportSize;
};