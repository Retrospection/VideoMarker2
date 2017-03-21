#include "stdafx.h"
#include "StateBase.h"
#include "State.h"


CStateBase::CStateBase(CVideoMarker2Dlg* pDlg) :_pDlg(pDlg)
{
}


CStateBase::~CStateBase()
{
}

void CStateBase::Play()
{
	GetPresenter()->Play();
	SetState(PLAY);
}

void CStateBase::Stop()
{
}



void CStateBase::SetState(const std::string& state)
{
	_pDlg->SetState(state);
	_pDlg->ClearHighLight();
}

void CStateBase::Init()
{

}

void CStateBase::RefreshButton()
{

}

CWnd* CStateBase::GetDlgItem(int nID)
{
	return _pDlg->GetDlgItem(nID);
}


void CStateBase::Open()
{

}

void CStateBase::Pause()
{

}


void CStateBase::SeekTo(int nPos)
{
	GetPresenter()->SeekTo(nPos);
	_pDlg->ClearHighLight();
}


void CStateBase::ForwardOneFrame(int nCurrentFrameIndex)
{
	GetPresenter()->ForwardOneFrame(nCurrentFrameIndex);
	_pDlg->ClearHighLight();
}

void CStateBase::BackOneFrame(int nCurrentFrameIndex)
{
	GetPresenter()->BackOneFrame(nCurrentFrameIndex);
	_pDlg->ClearHighLight();
}

CVideoMarkerPresenter* CStateBase::GetPresenter()
{
	return _pDlg->m_pPresenter;
}

void CStateBase::OpenTextFile()
{
	GetPresenter()->OpenTextFile();
}

void CStateBase::AddMark()
{

}

void CStateBase::SaveTextFile()
{

}

bool CStateBase::IsTextFileOpened() const
{
	return _pDlg->m_bTextStatus;
}

void CStateBase::OnLButtonDown(int nFlags, cv::Point point)
{

}

void CStateBase::OnMouseMove(int nFlags, cv::Point point)
{

}

void CStateBase::OnLButtonUp(int nFlags, cv::Point point)
{

}

void CStateBase::SaveMark()
{

}

void CStateBase::OpenProject()
{
	wchar_t* alertMessage[] = { L"", L"Load Project File Failed!", L"open video failed!", L"open text failed!" };
	size_t result = GetPresenter()->OpenProject();
	if (result != 0)
	{
		_pDlg->MessageBox(alertMessage[result]);
	}
	SetState(STOP);
}


