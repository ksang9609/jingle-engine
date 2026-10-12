// Rendering/Renderer.cpp

#include <cassert>

#include "Rendering/Renderer.h"

URenderer::URenderer(HWND hWindow)
	: mDevice(nullptr)
	, mDeviceContext(nullptr)
	, mSwapChain(nullptr)
	, mFrameBuffer(nullptr)
	, mFrameBufferRTV(nullptr)
	, mRasterizerState(nullptr)
	, mConstantBuffer(nullptr)
	, mClearColor{ 0.025f, 0.025f, 0.025f, 1.0f }
	, mViewportInfo{}
	, mSimpleVertexShader(nullptr)
	, mSimplePixelShader(nullptr)
	, mSimpleInputLayout(nullptr)
	, mStride(0)
	, mIsReleased(false)
{
	createDeviceAndSwapChain(hWindow);
	createFrameBuffer();
	createRaterizerState();
	createShader();
	createConstantBuffer();
}

URenderer::~URenderer()
{
	Release();
}

void URenderer::Release()
{
	if (mIsReleased)
	{
		return;
	}

	releaseRasterizerState();
	releaseConstantBuffer();
	releaseShader();

	mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	releaseFrameBuffer();
	releaseDeviceAndSwapChain();

	mIsReleased = true;
}

ID3D11Buffer* URenderer::CreateVertexBuffer(const FVertexSimple* vertices, UINT byteWidth)
{
	D3D11_BUFFER_DESC bufferDesc = {
		.ByteWidth = byteWidth,
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
	};

	D3D11_SUBRESOURCE_DATA initData = {
		.pSysMem = vertices,
	};

	ID3D11Buffer* vertexBuffer = nullptr;
	HRESULT hr = mDevice->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
	if (FAILED(hr))
	{
		assert(false);
		return nullptr;
	}
	return vertexBuffer;
}

void URenderer::ReleaseVertexBuffer(ID3D11Buffer& vertexBuffer)
{
	vertexBuffer.Release();
}

void URenderer::SwapBuffer()
{
	mSwapChain->Present(1, 0);
}

void URenderer::Prepare()
{
	mDeviceContext->ClearRenderTargetView(mFrameBufferRTV, mClearColor);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->RSSetViewports(1, &mViewportInfo);
	mDeviceContext->RSSetState(mRasterizerState);

	mDeviceContext->OMSetRenderTargets(1, &mFrameBufferRTV, nullptr);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void URenderer::PrepareShader()
{
	mDeviceContext->VSSetShader(mSimpleVertexShader, nullptr, 0);
	mDeviceContext->PSSetShader(mSimplePixelShader, nullptr, 0);
	mDeviceContext->IASetInputLayout(mSimpleInputLayout);

	if (mConstantBuffer)
	{
		mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
	}
}

void URenderer::RenderPrimitive(ID3D11Buffer& vertexBuffer, UINT numVertices)
{
	UINT offset = 0;
	ID3D11Buffer* const pBuffer = &vertexBuffer;
	mDeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &mStride, &offset);
	mDeviceContext->Draw(numVertices, 0);
}

void URenderer::UpdateConstant(const FVector& location, const FVector& scale)
{
	if (mConstantBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE constantbufferMSR; // mapped sub resource

		mDeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
		FConstants* constants = reinterpret_cast<FConstants*>(constantbufferMSR.pData);

		constants->Location = location;
		constants->Scale = scale;

		mDeviceContext->Unmap(mConstantBuffer, 0);
	}
}

void URenderer::createDeviceAndSwapChain(HWND hWindow)
{
	D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapchaindesc = {
		.BufferDesc = {
			.Width = 0,
			.Height = 0,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		},
		.SampleDesc = {
			.Count = 1,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.OutputWindow = hWindow,
		.Windowed = TRUE,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
	};


	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
		&swapchaindesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);

	mSwapChain->GetDesc(&swapchaindesc);

	mViewportInfo = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<float>(swapchaindesc.BufferDesc.Width),
		.Height = static_cast<float>(swapchaindesc.BufferDesc.Height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};
}

void URenderer::releaseDeviceAndSwapChain()
{
	if (mDeviceContext)
	{
		mDeviceContext->Flush();
	}
	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = nullptr;
	}
	if (mDevice)
	{
		mDevice->Release();
		mDevice = nullptr;
	}
	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = nullptr;
	}
}

void URenderer::createFrameBuffer()
{
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mFrameBuffer));

	D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
	};

	mDevice->CreateRenderTargetView(mFrameBuffer, &framebufferRTVdesc, &mFrameBufferRTV);
}

void URenderer::releaseFrameBuffer()
{
	if (mFrameBufferRTV)
	{
		mFrameBufferRTV->Release();
		mFrameBufferRTV = nullptr;
	}
	if (mFrameBuffer)
	{
		mFrameBuffer->Release();
		mFrameBuffer = nullptr;
	}
}

void URenderer::createRaterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {
		.FillMode = D3D11_FILL_SOLID,
		.CullMode = D3D11_CULL_BACK,
	};
	mDevice->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
}

void URenderer::releaseRasterizerState()
{
	if (mRasterizerState)
	{
		mRasterizerState->Release();
		mRasterizerState = nullptr;
	}
}

void URenderer::createShader()
{
	ID3DBlob* vertexshaderCSO;
	ID3DBlob* pixelshaderCSO;

	D3DCompileFromFile(L"resources/Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

	mDevice->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &mSimpleVertexShader);

	D3DCompileFromFile(L"resources/Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

	mDevice->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &mSimplePixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mDevice->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &mSimpleInputLayout);

	mStride = sizeof(FVertexSimple);

	vertexshaderCSO->Release();
	pixelshaderCSO->Release();
}

void URenderer::releaseShader()
{
	if (mSimpleInputLayout)
	{
		mSimpleInputLayout->Release();
		mSimpleInputLayout = nullptr;
	}
	if (mSimplePixelShader)
	{
		mSimplePixelShader->Release();
		mSimplePixelShader = nullptr;
	}
	if (mSimpleVertexShader)
	{
		mSimpleVertexShader->Release();
		mSimpleVertexShader = nullptr;
	}
}

void URenderer::createConstantBuffer()
{
	D3D11_BUFFER_DESC constantbufferdesc = {
		.ByteWidth = (sizeof(FConstants) + 0xf) & 0xfffffff0, // ensure constant buffer size is multiple of 16 bytes
		.Usage = D3D11_USAGE_DYNAMIC, // will be updated from CPU every frame
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};
	mDevice->CreateBuffer(&constantbufferdesc, nullptr, &mConstantBuffer);
}

void URenderer::releaseConstantBuffer()
{
	if (mConstantBuffer)
	{
		mConstantBuffer->Release();
		mConstantBuffer = nullptr;
	}
}