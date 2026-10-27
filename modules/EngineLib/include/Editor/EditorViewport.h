// Editor/EditorViewport.h
#pragma once

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#include <d3d11.h>

#include "Core/Core.h"
#include "Core/Math/Vector.h"

class FEditorViewport
{
public:
	FEditorViewport(ID3D11Device& deviceRef);
	~FEditorViewport();

	ID3D11RenderTargetView* GetViewportRTV() const { return mViewportRTV; }
	ID3D11ShaderResourceView* GetViewportSRV() const { return mViewportSRV; }

	/* Getters/Setters */

	uint32 GetRenderWidth() const { return mRenderWidth; }
	uint32 GetRenderHeight() const { return mRenderHeight; }

	bool EnsureViewportSize(float width, float height);
private:
	TResult<void, FString> createViewportResources(uint32 width, uint32 height);

	void releaseViewportResources();

private:
	/* Device Reference */
	ID3D11Device* mDeviceRef;

	/* Viewport Resources */
	ID3D11Texture2D* mViewportTexture;
	ID3D11RenderTargetView* mViewportRTV;
	ID3D11ShaderResourceView* mViewportSRV;

	uint32 mRenderWidth;	// Width of the render target
	uint32 mRenderHeight;	// Height of the render target
};