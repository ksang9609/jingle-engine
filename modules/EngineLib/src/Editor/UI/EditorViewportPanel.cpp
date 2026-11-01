// Editor/UI/EditorViewportPanel.cpp
#include "Editor/UI/EditorViewportPanel.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>

#include "Editor/EditorViewport.h"

FEditorViewportPanel::FEditorViewportPanel(FEditorViewport& viewportRef)
	: mViewportRef(&viewportRef)
	, mImagePosition(FVector2::Zero)
	, mDisplaySize(FVector2::Zero)
{
}

FEditorViewportPanel::~FEditorViewportPanel()
{
}

void FEditorViewportPanel::BeginPanel()
{
	assert(mViewportRef);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	mImagePosition = FVector2(
		ImGui::GetCursorScreenPos().x,
		ImGui::GetCursorScreenPos().y);
	mDisplaySize = FVector2(
		ImGui::GetContentRegionAvail().x,
		ImGui::GetContentRegionAvail().y);

	if (!mViewportRef->EnsureViewportSize(mDisplaySize.X, mDisplaySize.Y))
	{
		assert(false);
		return;
	}
}

void FEditorViewportPanel::DrawViewport()
{
	assert(mViewportRef);
	auto viewportSRV = mViewportRef->GetViewportSRV();
	ImGui::Image(
		(ImTextureID)viewportSRV, 
		ImVec2(mDisplaySize.X, mDisplaySize.Y));
}

void FEditorViewportPanel::EndPanel()
{
	ImGui::End();
	ImGui::PopStyleVar();
}