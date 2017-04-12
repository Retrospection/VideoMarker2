#include "stdafx.h"
#include "InitState.h"

#include "VideoMarker2Dlg.h"


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


void CInitState::Undo()
{

}

void CInitState::Redo()
{

}








