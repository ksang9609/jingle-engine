// EngineApp/EditorEngine.h
#pragma once

#include <d3d11.h>

#include "EngineTimer.h"
#include "Core/Math/Camera.h"

class URenderer;
class FEditorUiManager;
class FEditorViewport;

class FEditorEngine final
{
public:
	FEditorEngine();
	~FEditorEngine();

	void Init(HWND hWindow);
	bool Update();
	void End();
private:
	void createManagers(HWND hWindow);
	void releaseManagers();
private:
	/* Managers */
	// TODO: Wrap renderer by GraphicsManager
	URenderer* mRenderer;
	FCamera mEditorViewportCamera;
	FEditorViewport* mEditorViewport;

	FEditorUiManager* mUiManager;

	// TODO: Move this to a separate class, e.g., GpuResourceManager
	ID3D11Buffer* mVertexBuffer;

	/* Timer */
	FEngineTimer mEngineTimer;

	/* Internal States */
	bool mbIsInitialized;
	bool mbIsReleased;
};