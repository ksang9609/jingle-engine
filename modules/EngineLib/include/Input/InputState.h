// Input/InputState.h
#pragma once

#include <windows.h>

//현재 프레임 입력 상태
struct FInputState
{
	bool bDown[256] = {};
	bool bPressed[256] = {};
	bool bReleased[256] = {};

	//이번 프레임 마우스 이동량
	int64 MouseDX = 0;
	int64 MouseDY = 0;
	int32 CursorX = 0;
	int32 CursorY = 0;

	//이번 프레임 마우스휠(중간) 이동량
	float32 MouseWheelDelta = 0;

	void ResetStates()
	{
		memset(bPressed, 0, sizeof(bPressed));
		memset(bReleased, 0, sizeof(bReleased));
		MouseDX = 0;
		MouseDY = 0;
		MouseWheelDelta = 0;
	}

	void OnKeyDown(int vk) { bPressed[vk] = true; bDown[vk] = true; }
	void OnKeyUp(int vk) { bReleased[vk] = true; bDown[vk] = false; }
	void OnRawMouse(long dx, long dy) { MouseDX += dx; MouseDY += dy; }
	void OnMouseWheel(float Delta) { MouseWheelDelta = Delta; }
	void OnFocusLost() { memset(bDown, 0, sizeof(bDown)); MouseDX = MouseDY = 0; }

	//bool IsDown(int vk)		 const { return bDown[vk]; }
	//bool WasPressed(int vk)  const { return bPressed[vk]; }
	//bool WasReleased(int vk) const { return bReleased[vk]; }
};
