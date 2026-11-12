// EngineApp/EditorEngine.h
#pragma once

#include <d3d11.h>

#include "Core/Math/Camera.h"
#include "Core/Containers/TArray.h"

#include "RenderInterface/PrimitiveRenderData.h"
#include "Object/SceneComponent.h"
#include "Input/InputSystem.h"

#include "EngineTimer.h"
#include "SceneManager.h"

class URenderer;
class FEditorUiManager;
class FEditorViewport;

class FEditorEngine final
{
public:
	FEditorEngine(FInputSystem& inputSystem);
	~FEditorEngine();

	void Init(HWND hWindow);
	bool Update();
	void End();

	void RequestResize(uint32 width, uint32 height);
private:
	void createManagers(HWND hWindow);
	void releaseManagers();

	// TODO: Move this to a separate class, e.g., SceneManager or GraphicsManager
	void renderPrimitives(
		const TArray<FPrimitiveRenderData>& renderDatas,
		const FMatrix& viewProjectionMatirx // TODO: Set view projection matrix only once per frame
	);
	void renderPrimitive(EPrimitiveType primitiveType);
	void resizeWindow(uint32 width, uint32 height);
private:
	/* References */
	FInputSystem& mInputSystemRef;

	/* Managers */
	// TODO: Wrap renderer by GraphicsManager
	URenderer* mRenderer;
	FCamera mEditorViewportCamera;
	FEditorViewport* mEditorViewport;

	FEditorUiManager* mUiManager;
	FSceneManager* mSceneManager;

	// TODO: Move this to a separate class, e.g., GpuResourceManager
	ID3D11Buffer* mSphereVertexBuffer;
	ID3D11Buffer* mCubeVertexBuffer;

	/* Timer */
	FEngineTimer mEngineTimer;

	/* Internal States */
	bool mbIsInitialized;
	bool mbIsReleased;

	/* Resize */
	uint32 mPendingResize[2]; // width, height
	bool bResizeRequested;
};