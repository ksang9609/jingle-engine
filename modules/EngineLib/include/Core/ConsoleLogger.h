// Core/ConsoleLogger.h
#pragma once

#include "Core/Core.h"
#include "Core/Containers/TArray.h"

/* Temp */
#include <format>

enum class ELogLevel : uint8
{
	Info,
	Warning,
	Error
};

struct FLogEntry
{
	ELogLevel Level;
	FString Message;
};

namespace consoleLog
{
	template<typename... Args>
	void Log(ELogLevel Level, std::format_string<Args...> Format, Args&&... args);
	void Log(ELogLevel Level, FStringView Message);
	void Log(ELogLevel Level, FString&& Message);

	TArray<FLogEntry> FlushLogs();
};

template<typename... Args>
void consoleLog::Log(ELogLevel Level, std::format_string<Args...> Format, Args&&... args)
{
	FString Message = std::vformat(Format.get(), std::make_format_args(args...));
	consoleLog::Log(Level, std::move(Message));
}