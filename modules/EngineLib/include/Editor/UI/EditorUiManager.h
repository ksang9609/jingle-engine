// Editor/UI/EditorUiManager.h
#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "Core/Core.h"
#include "Editor/UI/ConsoleWindow.h"
#include "Editor/UI/ControlPanel.h"
#include "Editor/UI/EditorViewportPanel.h"

struct FEditorUiManagerParams
{
	HWND HWnd;
	ID3D11Device& DeviceRef;
	ID3D11DeviceContext& ContextRef;
	FCamera& CameraRef;
	FEditorViewport& EditorViewportRef;
};

class FEditorUiManager final
{
public:
	// TODO: Wrap device and device context in a class and pass it to the constructor
	FEditorUiManager(const FEditorUiManagerParams& params);
	~FEditorUiManager();

	/* General Methods */
	void BeginFrame();
	void Render();

	void Release();

	/* Pacade */
	inline FVector2 GetViewportDisplaySize() const { return mViewportPanel.GetDisplaySize(); }
	float GetViewportAspectRatio() const;

private:
	/* Windows */
	FConsoleWindow mConsoleWindow;
	FControlPanel mControlPanel;
	FEditorViewportPanel mViewportPanel;

	/* Internal States */
	bool mbIsInitialized;
	bool mbIsReleased;
};