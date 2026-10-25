// Editor/UI/ControlPanel.cpp
#include "Editor/UI/ControlPanel.h"

#include <memory>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

#include "Core/Math/Camera.h"


FControlPanel::FControlPanel(FControlPanelRefs&& refs)
	: mRefs(std::move(refs))
{
}

void FControlPanel::DrawPanel()
{
	ImGui::Begin("Control Panel");

	ImGui::SeparatorText("Camera Controls");
	{
		FVector cameraPosition = mRefs.CameraRef.GetPosition();
		if (ImGui::DragFloat3(
			"Camera Position",
			&cameraPosition.X, 0.1f, -100.0f, 100.0f, "%.2f"))
		{
			mRefs.CameraRef.SetPosition(cameraPosition);
		}

		FRotator cameraRotation = mRefs.CameraRef.GetRotation().ToRotator();
		if (ImGui::DragFloat3(
			"Camera Rotation (Roll, Pitch, Yaw)",
			&cameraRotation.Roll, 0.1f, -180.0f, 180.0f, "%.2f"))
		{
			mRefs.CameraRef.SetRotation(cameraRotation);
		}

		float cameraFov = mRefs.CameraRef.GetFovDeg();
		if (ImGui::DragFloat(
			"Camera FOV",
			&cameraFov, 0.1f, 1.0f, 179.0f, "%.2f"))
		{
			mRefs.CameraRef.SetFovDeg(cameraFov);
		}

		bool bIsPerspective = mRefs.CameraRef.GetProjectionType() == ECameraProjectionType::Perspective;
		if (ImGui::Checkbox("Perspective Projection", &bIsPerspective))
		{
			mRefs.CameraRef.SetProjectionType(
				bIsPerspective ? ECameraProjectionType::Perspective : ECameraProjectionType::Orthographic
			);
		}
	}

	ImGui::End();
}