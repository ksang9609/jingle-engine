// Input/InputSystem.cpp
#include "Input/InputSystem.h"

FInputSystem::FInputSystem()
	: mInputState()
	, mDefferedMessages{}
{
}

void FInputSystem::Defer(FDeferredMessage&& message)
{
	mDefferedMessages.Add(std::move(message));
}

void FInputSystem::ProcessDeferredEvents()
{
	mInputState.ResetStates();

	// Take the deferred messages in case new messages are added during processing
	TArray<FDeferredMessage> local;
	local.Swap(mDefferedMessages);

	for (const FDeferredMessage& message : local)
	{
		switch (message.Message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			// If the 30th bit of lParam is not set, 
			// it means the key was not previously down, 
			// so we consider it a new key press
			if ((message.LParam & (1 << 30)) == 0)
			{
				mInputState.OnKeyDown(static_cast<int>(message.WParam));
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			mInputState.OnKeyUp(static_cast<int>(message.WParam));
			break;

		case WM_LBUTTONDOWN:
			mInputState.OnKeyDown(VK_LBUTTON);
			break;
		case WM_LBUTTONUP:
			mInputState.OnKeyUp(VK_LBUTTON);
			break;
		case WM_RBUTTONDOWN:
			mInputState.OnKeyDown(VK_RBUTTON);
			break;
		case WM_RBUTTONUP:
			mInputState.OnKeyUp(VK_RBUTTON);
			break;

		case WM_MOUSEWHEEL:
			mInputState.OnMouseWheel(GET_WHEEL_DELTA_WPARAM(message.WParam)
				/ static_cast<float32>(WHEEL_DELTA));
			break;
		case WM_MOUSEMOVE:
			mInputState.CursorX =
				static_cast<int32>(LOWORD(message.LParam));
			mInputState.CursorY =
				static_cast<int32>(HIWORD(message.LParam));
			break;
		case WM_INPUT:
			mInputState.OnRawMouse(message.RawMouseDX, message.RawMouseDY);
			break;

		case WM_KILLFOCUS:
			mInputState.OnFocusLost();
			break;
		}
	}
}

bool FInputSystem::IsKeyDown(EKeyCode keyCode)
{
	return mInputState.bDown[static_cast<int32>(keyCode)];
}

bool FInputSystem::IsKeyPressed(EKeyCode keyCode)
{
	return mInputState.bPressed[static_cast<int32>(keyCode)];
}

bool FInputSystem::IsKeyReleased(EKeyCode keyCode)
{
	return mInputState.bReleased[static_cast<int32>(keyCode)];
}

FVector2 FInputSystem::GetMousePosition()
{
	return FVector2(
		static_cast<float32>(mInputState.CursorX),
		static_cast<float32>(mInputState.CursorY));
}

float32 FInputSystem::GetMouseX()
{
	return static_cast<float32>(mInputState.CursorX);
}

float32 FInputSystem::GetMouseY()
{
	return static_cast<float32>(mInputState.CursorY);
}

FVector2 FInputSystem::GetMouseDelta()
{
	return FVector2(
		static_cast<float32>(mInputState.MouseDX), 
		static_cast<float32>(mInputState.MouseDY));
}

float32 FInputSystem::GetMouseDeltaX()
{
	return static_cast<float32>(mInputState.MouseDX);
}

float32 FInputSystem::GetMouseDeltaY()
{
	return static_cast<float32>(mInputState.MouseDY);
}

float32 FInputSystem::GetWheelDelta()
{
	return mInputState.MouseWheelDelta;
}
