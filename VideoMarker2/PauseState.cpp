#include "stdafx.h"
#include "PauseState.h"

#include "state.h"

#include "VideoMarkerPresenter.h"

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
	StartPlayThread();
	SetState(PLAY);
}

void CPauseState::Stop()
{
	if (IsPlaying())
	{
		SetPlaying(false);
	}
	GetPresenter()->Stop();
	SetState(S1);
}


