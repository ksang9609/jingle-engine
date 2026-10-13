// EngineApp/EditorEngine.h
#pragma once

#include <d3d11.h>

class URenderer;
class FEditorUiManager;

class FEditorEngine final
{
public:
	FEditorEngine();
	~FEditorEngine();

	void Init(HWND hWindow);
	bool Update();
	void End();
private:
	/* Managers */
	// TODO: Wrap renderer by GraphicsManager
	URenderer* mRenderer;
	FEditorUiManager* mUiManager;

	// TODO: Move this to a separate class, e.g., GpuResourceManager
	ID3D11Buffer* mVertexBuffer;

	/* Timer */
	LARGE_INTEGER mPrevTime;
	LARGE_INTEGER mFrequency;
	int mTargetFPS;
	double mTargetFrameTime;

	/* Internal States */
	bool mIsInitialized;
	bool mIsReleased;

	/* Member Functions */
	void createManagers(HWND hWindow);
	void releaseManagers();
};