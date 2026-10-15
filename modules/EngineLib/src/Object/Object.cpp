// Object/Object.cpp
#include "Object/Object.h"

#include "Core/EngineStatics.h"

UObject::UObject()
{
	mObjectID = {
		.UUID = engineStatics::GenUUID(),
		.InternalIndex =
			GUObjectArray.Add(this)
	};
}

UObject::~UObject()
{
	GUObjectArray.RemoveAt(mObjectID.InternalIndex);
}

// Copy constructor
UObject::UObject(const UObject&)
	: UObject()
{
}

// Copy assignment operator
UObject& UObject::operator= (const UObject&)
{
	// Copy construcotr is called,
	return *this;
}

// Move constructor
UObject::UObject(UObject&&) noexcept
	: UObject()
{
}

// Move assignment operator
UObject& UObject::operator= (UObject&&) noexcept
{
	return *this;
}

