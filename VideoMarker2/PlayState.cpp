#include "stdafx.h"
#include "PlayState.h"

#include "State.h"

#include "VideoMarkerPresenter.h"

CPlayState::CPlayState(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CPlayState::~CPlayState()
{
}




void CPlayState::RefreshButton()
{
}

void CPlayState::Pause()
{
	GetPresenter()->Pause();
//	SetState(PAUSE);
}

void CPlayState::Stop()
{
	GetPresenter()->Stop();
//	SetState(STOP);
}


