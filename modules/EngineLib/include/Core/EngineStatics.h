// Core/EngineStatics.h
#pragma once
#pragma warning(disable: 28251)

#include "Core/Core.h"

namespace engineStatics
{
	/* UUID */
	uint32 GenUUID();
	uint32 GetNextUUID();
	void SetNextUUID(uint32 uuid);

	/* Memory */
	uint32 GetTotalAllocationBytes();
	uint32 GetTotalAllocationCount();
}

void* operator new(size_t size);
void operator delete(void* deleteObject, size_t size);