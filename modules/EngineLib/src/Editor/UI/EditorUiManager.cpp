// Editor/UI/EditorUiManager.cpp

#include "Editor/UI/EditorUiManager.h"

#include <cassert>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

FEditorUiManager::FEditorUiManager(const FEditorUiManagerParams& params)
	: mConsoleWindow()
	, mControlPanel({ params.CameraRef })
	, mViewportPanel(params.EditorViewportRef)
	, mbIsInitialized(false)
	, mbIsReleased(false)
{
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

	bool bSuccess = true;
	bSuccess &= ImGui_ImplWin32_Init(params.HWnd);
	bSuccess &= ImGui_ImplDX11_Init(&params.DeviceRef, &params.ContextRef);

	assert(bSuccess);
	mbIsInitialized = bSuccess;
}

FEditorUiManager::~FEditorUiManager()
{
	Release();
}

/* ===== General Methods ===== */
void FEditorUiManager::BeginFrame()
{
	assert(mbIsInitialized);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(
		0, ImGui::GetMainViewport()
	);

	// Viewport panel must be opened before rendering the viewport image,
	// to ensure the display size is updated correctly.
	mViewportPanel.BeginPanel();
}

void FEditorUiManager::Render()
{
	assert(mbIsInitialized);

	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello, world!");
	ImGui::End();

	mConsoleWindow.DrawWindow();
	mControlPanel.DrawPanel();
	mViewportPanel.DrawViewport();
	mViewportPanel.EndPanel();
}

void FEditorUiManager::Release()
{
	assert(mbIsInitialized);
	if (mbIsReleased)
	{
		return;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	mbIsReleased = true;
}

/* ===== Pacade ===== */
// Aspect ratio = width / height
float FEditorUiManager::GetViewportAspectRatio() const
{
	assert(mbIsInitialized);
	FVector2 displaySize = mViewportPanel.GetDisplaySize();
	if (displaySize.Y == 0.0f)
	{
		return 1.0f;
	}
	return displaySize.X / displaySize.Y;
}