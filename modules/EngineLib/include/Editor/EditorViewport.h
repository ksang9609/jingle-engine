// Editor/EditorViewport.h
#pragma once

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#include <d3d11.h>

#include "Core/Core.h"
#include "Core/Math/Vector.h"

// TODO: 
// Separate the responsiblity of drawing the panel
// to a different class, e.g., FEditorViewportPanel, 
// and let FEditorViewport only handle the viewport resources and status.
class FEditorViewport
{
public:
	FEditorViewport(ID3D11Device& deviceRef);
	~FEditorViewport();

	/* Panel Feature */
	void BeginPanel();
	void DrawViewport();
	void EndPanel();

	ID3D11RenderTargetView* GetViewportRTV() const { return mViewportRTV; }

	/* Getters/Setters */
	FVector2 GetImagePosition() const { return mImagePosition; }
	FVector2 GetDisplaySize() const { return mDisplaySize; }

	uint32 GetRenderWidth() const { return mRenderWidth; }
	uint32 GetRenderHeight() const { return mRenderHeight; }


private:
	bool ensureViewportSize();
	TResult<void, FString> createViewportResources(uint32 width, uint32 height);

	void releaseViewportResources();

private:
	/* Device Reference */
	ID3D11Device* mDeviceRef;

	/* Viewport Resources */
	ID3D11Texture2D* mViewportTexture;
	ID3D11RenderTargetView* mViewportRTV;
	ID3D11ShaderResourceView* mViewportSRV;

	/* Viewport Status */
	FVector2 mImagePosition; // Position of the image in the program window
	FVector2 mDisplaySize;

	uint32 mRenderWidth;	// Width of the render target
	uint32 mRenderHeight;	// Height of the render target
};