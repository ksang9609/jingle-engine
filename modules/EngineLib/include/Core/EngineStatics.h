// Core/EngineStatics.h
#pragma once

#include "Core/Core.h"

class UEngineStatics
{
public:
	inline static uint32 GenUUID()
	{
		return msNextUUID++;
	}

	inline static uint32 GetNextUUID()
	{
		return msNextUUID;
	}

	inline static void SetUUID(uint32 uuid)
	{
		msNextUUID = uuid;
	}

private:
	inline static uint32 msNextUUID;
};