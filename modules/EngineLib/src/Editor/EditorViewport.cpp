// Editor/EditorViewport.cpp
#include "Editor/EditorViewport.h"

#include <cassert>

FEditorViewport::FEditorViewport(ID3D11Device& deviceRef)
	: mDeviceRef(&deviceRef)
	, mViewportTexture(nullptr)
	, mViewportRTV(nullptr)
	, mViewportSRV(nullptr)
	, mRenderWidth(0)
	, mRenderHeight(0)
{
}

FEditorViewport::~FEditorViewport()
{
	releaseViewportResources();
}

bool FEditorViewport::EnsureViewportSize(float width, float height)
{
	assert(mDeviceRef);
	if (width <= 0 || height <= 0)
	{
		return false;
	}

	if (width == mRenderWidth &&
		height == mRenderHeight)
	{
		return true;
	}

	// Release existing resources
	releaseViewportResources();

	// Create new resources
	mRenderWidth = static_cast<uint32>(width);
	mRenderHeight = static_cast<uint32>(height);

	auto result = createViewportResources(mRenderWidth, mRenderHeight);
	if (!result)
	{
		assert(false);
		return false;
	}

	return true;
}

TResult<void, FString> FEditorViewport::createViewportResources(uint32 width, UINT32 height)
{
	D3D11_TEXTURE2D_DESC viewportBufferDesc = {
		.Width = static_cast<UINT>(width),
		.Height = static_cast<UINT>(height),
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		.SampleDesc = {
			.Count = 1,
			.Quality = 0,
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
	};

	HRESULT hr = mDeviceRef->CreateTexture2D(&viewportBufferDesc, nullptr, &mViewportTexture);
	if (FAILED(hr))
	{
		return TErr<FString>(FString("Failed to create viewport texture."
			"error code: " + std::to_string(hr)
		));
	}

	D3D11_RENDER_TARGET_VIEW_DESC viewportBufferRTVdesc = {
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
	};

	hr = mDeviceRef->CreateRenderTargetView(mViewportTexture, &viewportBufferRTVdesc, &mViewportRTV);
	if (FAILED(hr))
	{
		return TErr<FString>(FString("Failed to create viewport render target view."
			"error code: " + std::to_string(hr)
		));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewportBufferSRVdesc = {
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {
			.MostDetailedMip = 0,
			.MipLevels = 1,
		},
	};

	hr = mDeviceRef->CreateShaderResourceView(mViewportTexture, &viewportBufferSRVdesc, &mViewportSRV);
	if (FAILED(hr))
	{
		return TErr<FString>(FString("Failed to create viewport shader resource view."
			"error code: " + std::to_string(hr)
		));
	}

	return {};
}

void FEditorViewport::releaseViewportResources()
{
	if (mViewportSRV)
	{
		mViewportSRV->Release();
		mViewportSRV = nullptr;
	}
	if (mViewportRTV)
	{
		mViewportRTV->Release();
		mViewportRTV = nullptr;
	}
	if (mViewportTexture)
	{
		mViewportTexture->Release();
		mViewportTexture = nullptr;
	}
}