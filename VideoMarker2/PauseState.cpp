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

}

void CPauseState::Play()
{
	GetPresenter()->Play();
//	SetState(PLAY);
}

void CPauseState::Stop()
{
	GetPresenter()->Stop();
//	SetState(STOP);
}


