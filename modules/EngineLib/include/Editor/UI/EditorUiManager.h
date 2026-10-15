// Editor/UI/EditorUiManager.h
#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "Core/Core.h"
#include "Editor/UI/ConsoleWindow.h"

class FEditorUiManager final
{
public:
	// TODO: Wrap device and device context in a class and pass it to the constructor
	FEditorUiManager(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
	~FEditorUiManager();

	void Render(ID3D11ShaderResourceView* frameTextureSRV);

	void Release();
private:
	/* Windows */
	FConsoleWindow mConsoleWindow;

	/* Internal States */
	bool mbIsInitialized;
	bool mbIsReleased;
};