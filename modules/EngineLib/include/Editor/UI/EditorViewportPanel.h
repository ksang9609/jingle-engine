// Editor/UI/EditorViewportPanel.h
#pragma once

#include "Core/Math/Vector.h"

//#include "Editor/EditorViewport.h"
class FEditorViewport;

class FEditorViewportPanel
{
public:
	FEditorViewportPanel(FEditorViewport& viewportRef);
	~FEditorViewportPanel();

	void BeginPanel();
	void DrawViewport();
	void EndPanel();

	inline FVector2 GetImagePosition() const { return mImagePosition; }
	inline FVector2 GetDisplaySize() const { return mDisplaySize; }

private:
	/* References */
	FEditorViewport* mViewportRef;

	/* Viewport Status */
	FVector2 mImagePosition; // Position of the image in the program window
	FVector2 mDisplaySize;
};