// EngineApp/SceneManager.h
#pragma once

#include "Core/Containers/TArray.h"
#include "Object/SceneComponent.h"
#include "RenderInterface/PrimitiveRenderData.h"

class FSceneManager final
{
public:
	FSceneManager();
	~FSceneManager();
	FSceneManager(const FSceneManager&) = delete;
	FSceneManager& operator=(const FSceneManager&) = delete;
	FSceneManager(FSceneManager&&) = delete;
	FSceneManager& operator=(FSceneManager&&) = delete;

	/* Lifecycle */
	void Release();
	bool IsValid() const;

	/* Methods */
	void AddSceneComponent(USceneComponent& sceneComponent);

	TArray<FPrimitiveRenderData> GetAllPrimitiveRenderDatas() const;

	/* Getters / Setters */
	const TArray<USceneComponent*>& GetSceneComponents() const { return mSceneComponents; }

private:
	TArray<USceneComponent*> mSceneComponents; // Owns

	/* Internal States */
	bool mbIsInitialized;
	bool mbIsReleased;
};