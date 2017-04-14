#include "stdafx.h"
#include "PlayState.h"
#include "State.h"
#include "VideoMarkerPresenter.h"

#include <thread>

CPlayState::CPlayState(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CPlayState::~CPlayState()
{
}




void CPlayState::RefreshButton()
{
	m_ui.RefreshButton();
	GetDlgItem(IDC_BUTTON_STEPBACK)->EnableWindow((GetCurrentFrameIndex() != 0) ? TRUE : FALSE);
	GetDlgItem(IDC_BUTTON_STEPFORWARD)->EnableWindow((GetCurrentFrameIndex() < (GetTotalFrameCount() - 1)) ? TRUE : FALSE);
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

void CPlayState::Play()
{
	SetPlaying(true);
	while ((GetCurrentFrameIndex() + 1 < GetTotalFrameCount()) && IsPlaying())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		GetPresenter()->ForwardOneFrame(GetCurrentFrameIndex());
	}
}


