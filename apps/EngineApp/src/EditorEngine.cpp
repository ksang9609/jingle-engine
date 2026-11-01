// EngineApp/EditorEngine.cpp

// D3D libraries
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

#include "Core/ConsoleLogger.h"
#include "Core/EngineStatics.h"
#include "Core/Math/MathTypes.h"
#include "Core/Math/Transform.h"
#include "EngineApp/EditorEngine.h"
#include "Rendering/Renderer.h"
#include "Rendering/Primitives/Sphere.h"
#include "Rendering/Primitives/Cube.h"
#include "Editor/UI/EditorUiManager.h"
#include "Editor/EditorViewport.h"

#include "Object/Object.h"
#include "Object/SceneComponent.h"
#include "Object/PrimitiveComponent.h"
#include "Object/CubeComp.h"
#include "Object/SphereComp.h"

FEditorEngine::FEditorEngine()
	: mRenderer(nullptr)
	, mEditorViewportCamera()
	, mEditorViewport(nullptr)
	, mUiManager(nullptr)
	, mSceneManager(nullptr)
	, mSphereVertexBuffer(nullptr)
	, mCubeVertexBuffer(nullptr)
	, mEngineTimer(120) // target FPS
	, mbIsInitialized(false)
	, mbIsReleased(false)
{

}

FEditorEngine::~FEditorEngine()
{
	End();
}

void FEditorEngine::Init(HWND hWindow)
{
	if (mbIsInitialized)
	{
		return;
	}

	createManagers(hWindow);

	// Create vertex buffer after renderer and shader
	mSphereVertexBuffer = mRenderer->CreateVertexBuffer(SPHERE_VERTICES, sizeof(SPHERE_VERTICES));
	mCubeVertexBuffer = mRenderer->CreateVertexBuffer(CUBE_VERTICES, sizeof(CUBE_VERTICES));

	// Create scene components
	{
		UCubeComp* cubeComp = new UCubeComp();
		cubeComp->SetRelativeLocation(FVector(0.0f, 2.0f, 0.0f));
		mSceneManager->AddSceneComponent(*cubeComp);

		USphereComp* sphereComp = new USphereComp();
		sphereComp->SetRelativeLocation(FVector(2.0f, 0.0f, 0.0f));
		mSceneManager->AddSceneComponent(*sphereComp);
	}

	mbIsInitialized = true;
}

bool FEditorEngine::Update()
{
	assert(mbIsInitialized);

	// Timer
	mEngineTimer.StartFrame();
	float64 deltaTimeMS = mEngineTimer.GetDeltaTimeMS();

	/* Message handling */
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	/* Rendering */
	{
		mUiManager->BeginFrame();

		static FTransform transform(
			FVector(0.0f, 2.0f, 0.0f), FQuat::Identity, FVector(1.0f, 1.0f, 1.0f)
		);

		const FVector2 viewportSize = mUiManager->GetViewportDisplaySize();
		const float viewportWidth = viewportSize.X;
		const float viewportHeight = viewportSize.Y;
		const float aspectRatio = mUiManager->GetViewportAspectRatio();
		const FMatrix worldMatrix = transform.GetWorldMatrix();
		const FMatrix viewProjectionMatrix =
			mEditorViewportCamera.GetViewMatrix()
			* mEditorViewportCamera.GetProjectionMatrix(aspectRatio);

		// Draw the scene to the viewport render target
		{
			mRenderer->GetDeviceContext()->ClearDepthStencilView(
				mEditorViewport->GetDepthStencilDSV(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
				1.0f, 0
			);

			mRenderer->BindViewportRTV(
				*mEditorViewport->GetViewportRTV(),
				*mEditorViewport->GetDepthStencilDSV(),
				viewportWidth,
				viewportHeight);

			mRenderer->Prepare();
			mRenderer->PrepareShader();

			// Gather render data from scene components
			TArray<FPrimitiveRenderData> renderDatas =
				mSceneManager->GetAllPrimitiveRenderDatas();

			renderPrimitives(renderDatas, viewProjectionMatrix);
		}

		mUiManager->Render();

		mRenderer->SwapBuffer();
	}

	// Wait for the next frame
	mEngineTimer.WaitForNextFrame();

	return true;
}

void FEditorEngine::End()
{
	assert(mbIsInitialized);
	if (mbIsReleased)
	{
		return;
	}

	// Release resources
	if (mSphereVertexBuffer)
	{
		mRenderer->ReleaseVertexBuffer(*mSphereVertexBuffer);
		mSphereVertexBuffer = nullptr;
	}
	if (mCubeVertexBuffer)
	{
		mRenderer->ReleaseVertexBuffer(*mCubeVertexBuffer);
		mCubeVertexBuffer = nullptr;
	}

	releaseManagers();

	mbIsReleased = true;
}

void FEditorEngine::createManagers(HWND hWindow)
{
	assert(!mbIsInitialized);

	// Initialize camera
	mEditorViewportCamera
		.SetPosition(FVector(-5.0f, 0.0f, 0.0f))
		.LookAt(FVector(0.0f, 2.0f, 0.0f));

	// Create Renderer
	if (mRenderer == nullptr)
	{
		mRenderer = new URenderer(hWindow);
	}

	// Create Editor Viewport
	if (mEditorViewport == nullptr)
	{
		mEditorViewport = new FEditorViewport(
			*mRenderer->GetDevice()
		);
	}

	// Create UI Manager
	if (mUiManager == nullptr)
	{
		mUiManager = new FEditorUiManager({
			.HWnd = hWindow,
			.DeviceRef = *mRenderer->GetDevice(),
			.ContextRef = *mRenderer->GetDeviceContext(),
			.CameraRef = mEditorViewportCamera,
			.EditorViewportRef = *mEditorViewport
			});
	}

	// Create Scene Manager
	if (mSceneManager == nullptr)
	{
		mSceneManager = new FSceneManager();
	}
}

void FEditorEngine::releaseManagers()
{
	if (mSceneManager)
	{
		mSceneManager->Release();
		delete mSceneManager;
		mSceneManager = nullptr;
	}
	if (mUiManager)
	{
		mUiManager->Release();
		delete mUiManager;
		mUiManager = nullptr;
	}
	if (mEditorViewport)
	{
		delete mEditorViewport;
		mEditorViewport = nullptr;
	}
	if (mRenderer)
	{
		mRenderer->Release();
		delete mRenderer;
		mRenderer = nullptr;
	}
}

void FEditorEngine::renderPrimitives(
	const TArray<FPrimitiveRenderData>& renderDatas,
	const FMatrix& viewProjectionMatrix)
{
	for (const FPrimitiveRenderData& renderData : renderDatas)
	{
		mRenderer->UpdateConstant(renderData.WorldMatrix, viewProjectionMatrix);
		renderPrimitive(renderData.PrimitiveType);
	}
}

void FEditorEngine::renderPrimitive(EPrimitiveType primitiveType)
{
	switch (primitiveType)
	{
	case EPrimitiveType::Sphere:
		mRenderer->RenderPrimitive(*mSphereVertexBuffer, sizeof(SPHERE_VERTICES) / sizeof(FVertexSimple));
		break;
	case EPrimitiveType::Cube:
		mRenderer->RenderPrimitive(*mCubeVertexBuffer, sizeof(CUBE_VERTICES) / sizeof(FVertexSimple));
		break;
	default:
		assert(false && "Unsupported primitive type");
		break;
	}
}