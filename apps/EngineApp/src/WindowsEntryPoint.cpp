// EngineApp/WindowsEntryPoint.cpp

#include <Windows.h>

#include "EngineApp/EditorEngine.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPAram);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_DESTROY:
		// Sinal that the app should quit
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Window class name
	WCHAR WindowClass[] = L"JungleWindowClass";

	// Window title name
	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	RegisterClassW(&wndclass);

	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);


	FEditorEngine editorEngine;

	editorEngine.Init(hWnd);

	while (editorEngine.Update());

	editorEngine.End();


	return 0;
}