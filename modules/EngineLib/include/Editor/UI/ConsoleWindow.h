// Editor/UI/ConsoleWindow.h
#pragma once

#include "Core/Core.h"
#include "Core/Containers/TArray.h"
#include "Core/ConsoleLogger.h"

class FConsoleWindow final
{
public:
	FConsoleWindow();
	~FConsoleWindow() = default;

	void DrawWindow();

private:
	TArray<FLogEntry> mOutputLogBuffer;

	/* Ring Buffer States */
	// TODO: Add ring buffer class and use it here
	uint32 mOutputBufferHead = 0;

	/* Member Functions */
	void drainLogBuffer();
};