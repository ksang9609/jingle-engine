// Object/Object.cpp
#include "Object/Object.h"

#include "Core/EngineStatics.h"

UObject::UObject()
{
	mObjectID = {
		.UUID = UEngineStatics::GenUUID(),
		.InternalIndex =
			GUObjectArray.Add(this)
	};
}

UObject::~UObject()
{
	GUObjectArray.RemoveAt(mObjectID.InternalIndex);
}

// Copy constructor
UObject::UObject(const UObject& other)
	: UObject()
{
}

// Copy assignment operator
UObject& UObject::operator= (const UObject& other)
{
	// Copy construcotr is called,
	return *this;
}

// Move constructor
UObject::UObject(UObject&& other) noexcept
	: UObject()
{
}

// Move assignment operator
UObject& UObject::operator= (UObject&& other) noexcept
{
	return *this;
}

