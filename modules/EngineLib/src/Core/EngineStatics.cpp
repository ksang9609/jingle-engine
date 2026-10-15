// Core/EngineStatics.cpp
#include "Core/EngineStatics.h"

#include <cassert>

namespace
{
	uint32 gNextUUID = 0;
	uint32 gTotalAllocationBytes = 0;
	uint32 gTotalAllocationCount = 0;
}

/* engineStatics namespace */
uint32 engineStatics::GenUUID()
{
	return gNextUUID++;
}

uint32 engineStatics::GetNextUUID()
{
	return gNextUUID;
}

void engineStatics::SetNextUUID(uint32 uuid)
{
	gNextUUID = uuid;
}

uint32 engineStatics::GetTotalAllocationBytes()
{
	return gTotalAllocationBytes;
}

uint32 engineStatics::GetTotalAllocationCount()
{
	return gTotalAllocationCount;
}

void* operator new(size_t size)
{
	void* ptr = malloc(size);

	if (!ptr)
	{
		throw std::bad_alloc();
	}

	++gTotalAllocationCount;
	gTotalAllocationBytes += static_cast<uint32>(size);

	return ptr;
}

void operator delete(void* deleteObject, size_t size)
{
	assert(deleteObject);

	--gTotalAllocationCount;
	gTotalAllocationBytes -= static_cast<uint32>(size);

	free(deleteObject);
}