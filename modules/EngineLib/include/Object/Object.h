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
	UObject(const UObject& other);
	UObject& operator= (const UObject& other);
	UObject(UObject&& other) noexcept;
	UObject& operator= (UObject&& other) noexcept;

	/* Getter / Setters */
	inline uint32 GetUUID() const { return mObjectID.UUID; }
	inline uint32 GetInternalIndex() const { return mObjectID.InternalIndex; }

private:
	FObjectID mObjectID;
};


inline TSparseArray<UObject*> GUObjectArray;
