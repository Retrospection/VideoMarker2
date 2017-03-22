#include "stdafx.h"
#include "InitState.h"


CInitState::CInitState(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CInitState::~CInitState()
{
}

void CInitState::RefreshButton()
{
// 	ENABLE_ITEM(IDC_BUTTON_OPENPROJECT, TRUE);
// 	ENABLE_ITEM(IDC_BUTTON_ADDMARK, FALSE);
// 	ENABLE_ITEM(IDC_BUTTON_UNDO, FALSE);
// 	ENABLE_ITEM(IDC_BUTTON_REDO, FALSE);
// 	ENABLE_ITEM(IDC_BUTTON_STEPBACK, FALSE);
// 	ENABLE_ITEM(IDC_BUTTON_STEPFORWARD, FALSE);
// 	ENABLE_ITEM(IDC_SLIDER1, FALSE);

//	m_ui.Enable("OpenProject").Disable("StepForward");

	m_ui.RefreshButton();

// 	m_ui.Enable("OpenProject");
// 
// 	m_ui.Disable("AddMark")
// 		.Disable("Undo")
// 		.Disable("Redo")
// 		.Disable("StepBackward")
// 		.Disable("StepForward")
// 		.Disable("Slider");
}

void CInitState::AddMark()
{

}

void CInitState::SaveMark()
{

}

void CInitState::Undo()
{

}

void CInitState::Redo()
{

}








