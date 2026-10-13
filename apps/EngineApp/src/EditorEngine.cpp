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

#include "EngineApp/EditorEngine.h"
#include "Rendering/Renderer.h"
#include "Rendering/Primitives/Sphere.h"
#include "Editor/EditorUiManager.h"

FEditorEngine::FEditorEngine()
	: mRenderer(nullptr)
	, mUiManager(nullptr)
	, mVertexBuffer(nullptr)
	, mEngineTimer(0) // no target FPS, unlimited
	, mIsInitialized(false)
	, mIsReleased(false)
{

}

FEditorEngine::~FEditorEngine()
{
	End();
}

void FEditorEngine::Init(HWND hWindow)
{
	if (mIsInitialized)
	{
		return;
	}

	createManagers(hWindow);

	// Create vertex buffer after renderer and shader
	mVertexBuffer = mRenderer->CreateVertexBuffer(SPHERE_VERTICES, sizeof(SPHERE_VERTICES));

	mIsInitialized = true;
}

bool FEditorEngine::Update()
{
	assert(mIsInitialized);

	// Timer
	mEngineTimer.StartFrame();


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
		mRenderer->Prepare();
		mRenderer->PrepareShader();
		mRenderer->UpdateConstant(FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
		mRenderer->RenderPrimitive(*mVertexBuffer, sizeof(SPHERE_VERTICES) / sizeof(FVertexSimple));

		mUiManager->Render();

		mRenderer->SwapBuffer();
	}

	// Wait for the next frame
	mEngineTimer.WaitForNextFrame();

	return true;
}

void FEditorEngine::End()
{
	assert(mIsInitialized);
	if (mIsReleased)
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

	mIsReleased = true;
}

void FEditorEngine::createManagers(HWND hWindow)
{
	// Create Renderer
	if (mRenderer == nullptr)
	{
		mRenderer = new URenderer(hWindow);
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
	if (mRenderer)
	{
		mRenderer->Release();
		delete mRenderer;
		mRenderer = nullptr;
	}
}