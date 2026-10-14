// Core/ConsoleLogger.cpp
#include "Core/ConsoleLogger.h"

namespace
{
	TArray<FLogEntry> GPendingLogs{};
}

void consoleLog::Log(ELogLevel Level, FStringView Message)
{
	GPendingLogs.Add(FLogEntry{ Level, FString(Message) });
}

void consoleLog::Log(ELogLevel Level, FString&& Message)
{
	GPendingLogs.Add(FLogEntry{ Level, std::move(Message) });
}

TArray<FLogEntry> consoleLog::FlushLogs()
{
	TArray<FLogEntry> LogsToFlush = std::move(GPendingLogs);
	GPendingLogs.Reset(0);
	return LogsToFlush;
}