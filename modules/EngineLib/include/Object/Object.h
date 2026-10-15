// Object/Object.h
#pragma once

#include "Core/Core.h"
#include "Core/Containers/TSparseArray.h"

struct FObjectID
{
	uint32 UUID = 0;
	uint32 InternalIndex = 0;
};

class UObject
{
public:
	UObject();
	virtual ~UObject();
	UObject(const UObject&);
	UObject& operator= (const UObject&);
	UObject(UObject&&) noexcept;
	UObject& operator= (UObject&&) noexcept;

	/* Getter / Setters */
	inline uint32 GetUUID() const { return mObjectID.UUID; }
	inline uint32 GetInternalIndex() const { return mObjectID.InternalIndex; }

private:
	FObjectID mObjectID;
};


inline TSparseArray<UObject*> GUObjectArray;
