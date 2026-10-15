// Editor/UI/ConsoleWindow.cpp
#include "Editor/UI/ConsoleWindow.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

constexpr uint32 MAX_LOG_BUFFER_SIZE = 1024;

FConsoleWindow::FConsoleWindow()
	: mOutputLogBuffer{}
{
	mOutputLogBuffer.Reserve(MAX_LOG_BUFFER_SIZE);
}

void FConsoleWindow::drainLogBuffer()
{
	TArray<FLogEntry> newLogs = consoleLog::FlushLogs();
	
	// Add new logs to the output buffer

	// TODO: Optimize this by std::vector::insert() with iterators
	for (FLogEntry& log : newLogs)
	{
		if (mOutputLogBuffer.Num() < MAX_LOG_BUFFER_SIZE)
		{
			mOutputLogBuffer.Add(std::move(log));
		}
		else
		{
			// Ring buffer behavior: overwrite the oldest log
			mOutputLogBuffer[mOutputBufferHead] = std::move(log);
			mOutputBufferHead = (mOutputBufferHead + 1) % MAX_LOG_BUFFER_SIZE;
		}
	}
}

void FConsoleWindow::DrawWindow()
{
	drainLogBuffer();

	ImGui::Begin("Console Window");

	for (const FLogEntry& log : mOutputLogBuffer)
	{
		switch (log.Level)
		{
		case ELogLevel::Info:
			// White
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", "[Info] ");
			break;
		case ELogLevel::Warning:
			// Yellow
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "[Warning] ");
			break;
		case ELogLevel::Error:
			// Red
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", "[Error] ");
			break;
		default:
			break;
		}
		ImGui::SameLine();
		ImGui::TextUnformatted(log.Message.c_str());
	}

	ImGui::End();
}

