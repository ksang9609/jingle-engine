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

FEditorEngine::FEditorEngine()
	: mRenderer(nullptr)
	, mVertexBuffer(nullptr)
	, mPrevTime{}
	, mFrequency{}
	, mTargetFPS(30)
	, mTargetFrameTime(1000.0 / mTargetFPS)
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

	// Create Renderer
	mRenderer = new URenderer(hWindow);

	// Create ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWindow);
	ImGui_ImplDX11_Init(mRenderer->GetDevice(), mRenderer->GetDeviceContext());

	// Create vertex buffer after renderer and shader
	mVertexBuffer = mRenderer->CreateVertexBuffer(SPHERE_VERTICES, sizeof(SPHERE_VERTICES));

	// Timer
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mPrevTime);
	}

	mIsInitialized = true;
}

bool FEditorEngine::Update()
{
	if (!mIsInitialized)
	{
		return false;
	}

	// Timer
	// TODO: Move this to a separate class
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);

	const float deltaTime = static_cast<float>(
		startTime.QuadPart - mPrevTime.QuadPart) /
		mFrequency.QuadPart;
	mPrevTime = startTime;


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

		/* ImGui */
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Jungle Property Window");
			ImGui::Text("Current deltaTime: %.3f ms", deltaTime * 1000.0f);
			ImGui::End();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		mRenderer->SwapBuffer();
	}

	// Wait for the next frame
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);
	float elapsedTimeMS = static_cast<float>(endTime.QuadPart - mPrevTime.QuadPart) / mFrequency.QuadPart * 1000.0f;
	while (elapsedTimeMS < mTargetFrameTime)
	{
		Sleep(0);
		QueryPerformanceCounter(&endTime);
		elapsedTimeMS = static_cast<float>(endTime.QuadPart - mPrevTime.QuadPart) / mFrequency.QuadPart * 1000.0f;
	}

	return true;
}

void FEditorEngine::End()
{
	if (!mIsInitialized || mIsReleased)
	{
		return;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Release resources
	if (mVertexBuffer)
	{
		mRenderer->ReleaseVertexBuffer(*mVertexBuffer);
		mVertexBuffer = nullptr;
	}

	if (mRenderer)
	{
		mRenderer->Release();
		delete mRenderer;
		mRenderer = nullptr;
	}

	mIsReleased = true;
}
