// Input/InputSystem.h
#pragma once

#include <Windows.h>

#include "Core/Core.h"
#include "Core/Containers/TArray.h"
#include "Core/Math/Vector.h"
#include "KeyCode.h"
#include "InputState.h"

enum class EGameInputAxis : uint8 { MouseX, MouseY, MouseWheel };

struct FDeferredMessage
{
	HWND    HWnd = nullptr;
	UINT    Message = 0;
	WPARAM  WParam = 0;
	LPARAM  LParam = 0;

	long    RawMouseDX = 0;
	long    RawMouseDY = 0;
};

class FInputSystem
{
public:
	FInputSystem();

	void Defer(FDeferredMessage&& message);

	void ProcessDeferredEvents();

	bool IsKeyPressed(EKeyCode keyCode);
	bool IsKeyReleased(EKeyCode keyCode);
	bool IsKeyDown(EKeyCode keyCode);


	FVector2 GetMousePosition();
	float32 GetMouseX();
	float32 GetMouseY();

	FVector2 GetMouseDelta();
	float32 GetMouseDeltaX();
	float32 GetMouseDeltaY();

	float32 GetWheelDelta();

private:
	FInputState mInputState;
	TArray<FDeferredMessage> mDefferedMessages;
};
