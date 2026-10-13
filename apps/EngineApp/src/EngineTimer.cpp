// EngineApp/EngineTimer.cpp
#include "EngineApp/EngineTimer.h"

#include <windows.h>

FEngineTimer::FEngineTimer(int targetFPS)
	: mStartTimeTick{}
	, mFrequency{}
	, mTargetFPS(targetFPS)
	, mTargetFrameTimeMS(0.0)
{
	QueryPerformanceFrequency(&mFrequency);
	mTargetFrameTimeMS = (mTargetFPS > 0) ? 1.0 / (mTargetFPS * 1000.0f) : 0.0;
}

void FEngineTimer::StartFrame()
{
	QueryPerformanceCounter(&mStartTimeTick);
}

void FEngineTimer::WaitForNextFrame() const
{
	// Don't wait if target FPS is 0 (unlimited)
	if (mTargetFPS == 0)
	{
		return;
	}

	LARGE_INTEGER currentTimeTick;
	QueryPerformanceCounter(&currentTimeTick);
	float64 deltaTimeMS = static_cast<float64>(
		currentTimeTick.QuadPart - mStartTimeTick.QuadPart
		/ (mFrequency.QuadPart * 1000.0f));

	while (deltaTimeMS < mTargetFrameTimeMS)
	{
		Sleep(0);
		QueryPerformanceCounter(&currentTimeTick);

		deltaTimeMS = static_cast<float64>(
			currentTimeTick.QuadPart - mStartTimeTick.QuadPart
			/ (mFrequency.QuadPart * 1000.0f));
	}
}

void FEngineTimer::SetTargetFPS(uint32 targetFPS)
{
	mTargetFPS = targetFPS;
	mTargetFrameTimeMS = (mTargetFPS > 0) ? 1.0 / (mTargetFPS * 1000.0f) : 0.0;
}