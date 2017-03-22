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
	m_ui.RefreshButton();
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








