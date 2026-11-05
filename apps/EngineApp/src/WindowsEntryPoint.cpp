// EngineApp/WindowsEntryPoint.cpp

#include <Windows.h>

#include "EngineApp/EditorEngine.h"
#include "Input/InputSystem.h"

namespace
{
	// Global ??
	FInputSystem GInputSystem;
	bool GbIsExit = false;

	void ProcessMessage(bool* outbIsExit)
	{
		assert(outbIsExit);

		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			//WinProc 호출
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				*outbIsExit = true;
				break;
			}
		}
	}
}

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

	// Defer input messages to the input system for processing later
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_KILLFOCUS:
		GInputSystem.Defer({ hWnd, message, wParam, lParam });
		return 0;

	case WM_INPUT:
	{
		FDeferredMessage newMessage{ hWnd, message, wParam, lParam };

		BYTE buf[sizeof(RAWINPUT)]{ 0 };
		UINT size = sizeof(buf);
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buf,
			&size, sizeof(RAWINPUTHEADER)) != (UINT)-1)
		{
			const RAWINPUT* ri = (const RAWINPUT*)buf;
			if (ri->header.dwType == RIM_TYPEMOUSE &&
				(ri->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == 0)
			{
				newMessage.RawMouseDX = ri->data.mouse.lLastX;
				newMessage.RawMouseDY = ri->data.mouse.lLastY;
			}
		}
		GInputSystem.Defer(std::move(newMessage));
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	// SYS_ : Input messages that are generated when the ALT key is pressed.
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		GInputSystem.Defer({ hWnd, message, wParam, lParam });
		return DefWindowProc(hWnd, message, wParam, lParam);

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

	FEditorEngine editorEngine(GInputSystem);

	editorEngine.Init(hWnd);
	
	while (GbIsExit == false)
	{
		ProcessMessage(&GbIsExit);

		if (GbIsExit)
		{
			break;
		}

		if (!editorEngine.Update())
		{
			break;
		}
	}

	editorEngine.End();


	return 0;
}