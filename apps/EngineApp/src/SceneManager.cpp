// EngineApp/SceneManager.cpp
#include "EngineApp/SceneManager.h"
#include "Object/PrimitiveComponent.h"

FSceneManager::FSceneManager()
	: mSceneComponents()
	, mbIsInitialized(true)
	, mbIsReleased(false)
{
}

FSceneManager::~FSceneManager()
{
	Release();
}

void FSceneManager::Release()
{
	assert(mbIsInitialized);
	if (mbIsReleased)
	{
		return;
	}
	for (USceneComponent* sceneComponent : mSceneComponents)
	{
		delete sceneComponent;
	}
	mSceneComponents.Reset(0);

	mbIsReleased = true;
}

bool FSceneManager::IsValid() const
{
	return mbIsInitialized && !mbIsReleased;
}

/* ===== Methods ====== */
void FSceneManager::AddSceneComponent(USceneComponent& sceneComponent)
{
	mSceneComponents.Add(&sceneComponent);
}

TArray<FPrimitiveRenderData> FSceneManager::GetAllPrimitiveRenderDatas() const
{
	TArray<FPrimitiveRenderData> allPrimitiveRenderDatas;
	for (const USceneComponent* sceneComponent : mSceneComponents)
	{
		if (const UPrimitiveComponent* primitiveComponent = dynamic_cast<const UPrimitiveComponent*>(sceneComponent))
		{
			FPrimitiveRenderData renderData = primitiveComponent->GetRenderData();
			allPrimitiveRenderDatas.Add(std::move(renderData));
		}
	}
	return allPrimitiveRenderDatas;
}
