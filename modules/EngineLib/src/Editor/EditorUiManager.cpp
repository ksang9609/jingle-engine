// Editor/EditorUiManager.cpp

#include "Editor/EditorUiManager.h"

#include <cassert>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

FEditorUiManager::FEditorUiManager(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
}

FEditorUiManager::~FEditorUiManager()
{
	Release();
}

void FEditorUiManager::Render() const
{
	assert(mbIsInitialized);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello, world!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
}