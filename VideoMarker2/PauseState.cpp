#include "stdafx.h"
#include "PauseState.h"

#include "state.h"

#include "VideoMarkerPresenter.h"
#include "VideoMarker2Dlg.h"

CPauseState::CPauseState(CVideoMarker2Dlg* pDlg)
	:CStateBase(pDlg)
{
}


CPauseState::~CPauseState()
{
}

void CPauseState::RefreshButton()
{
	m_ui.RefreshButton();
}

void CPauseState::Play()
{
//	StartPlayThread();
	SetTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER, 40, NULL);
	SetState(PLAY);
}

void CPauseState::Stop()
{
	KillTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER);
	GetPresenter()->Stop();
	SetState(S1);
}


