// Editor/UI/ContorlPanel.h
#pragma once

class FCamera;

struct FControlPanelRefs
{
	FCamera& CameraRef;
};

class FControlPanel
{
public:
	FControlPanel(FControlPanelRefs&& refs);

	void DrawPanel();
private:
	FControlPanelRefs mRefs;
};