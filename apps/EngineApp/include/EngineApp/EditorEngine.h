// EngineApp/EditorEngine.h
#pragma once

#include <d3d11.h>

class URenderer;
class FEditorEngine
{
public:
	FEditorEngine();
	~FEditorEngine();

	void Init(HWND hWindow);
	bool Update();
	void End();
private:
	/* Resources */
	URenderer* mRenderer;

	ID3D11Buffer* mVertexBuffer;

	/* Timer */
	LARGE_INTEGER mPrevTime;
	LARGE_INTEGER mFrequency;
	int mTargetFPS;
	double mTargetFrameTime;

	/* Internal States */
	bool mIsInitialized;
	bool mIsReleased;
};