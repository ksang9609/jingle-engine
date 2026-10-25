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
#include "Editor/UI/EditorUiManager.h"
#include "Editor/EditorViewport.h"

#include "Object/Object.h"

FEditorEngine::FEditorEngine()
	: mRenderer(nullptr)
	, mEditorViewportCamera()
	, mEditorViewport(nullptr)
	, mUiManager(nullptr)
	, mVertexBuffer(nullptr)
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
	mVertexBuffer = mRenderer->CreateVertexBuffer(SPHERE_VERTICES, sizeof(SPHERE_VERTICES));

	mbIsInitialized = true;

	// Initialize camera
	mEditorViewportCamera
		.SetPosition(FVector(-5.0f, 0.0f, 0.0f))
		.LookAt(FVector(0.0f, 2.0f, 0.0f));
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
		mEditorViewport->BeginPanel();

		static FTransform transform(
			FVector(0.0f, 2.0f, 0.0f), FQuat::Identity, FVector(1.0f, 1.0f, 1.0f)
		);

		const FVector2 viewportSize = mEditorViewport->GetDisplaySize();
		const float viewportWidth = viewportSize.X;
		const float viewportHeight = viewportSize.Y;
		const float aspectRatio = viewportWidth / viewportHeight;
		const FMatrix worldMatrix = transform.GetWorldMatrix();
		const FMatrix viewProjectionMatrix = 
			mEditorViewportCamera.GetViewMatrix()
			* mEditorViewportCamera.GetProjectionMatrix(aspectRatio);


		mRenderer->BindViewportRTV(
			*mEditorViewport->GetViewportRTV(),
			viewportWidth,
			viewportHeight);

		mRenderer->Prepare();
		mRenderer->PrepareShader();
		mRenderer->UpdateConstant(worldMatrix, viewProjectionMatrix);

		mRenderer->RenderPrimitive(*mVertexBuffer, sizeof(SPHERE_VERTICES) / sizeof(FVertexSimple));
		mEditorViewport->DrawViewport();
		mEditorViewport->EndPanel();

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
	if (mVertexBuffer)
	{
		mRenderer->ReleaseVertexBuffer(*mVertexBuffer);
		mVertexBuffer = nullptr;
	}

	releaseManagers();

	mbIsReleased = true;
}

void FEditorEngine::createManagers(HWND hWindow)
{
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
		mUiManager = new FEditorUiManager(
			hWindow,
			mRenderer->GetDevice(),
			mRenderer->GetDeviceContext()
		);
	}
}

void FEditorEngine::releaseManagers()
{
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