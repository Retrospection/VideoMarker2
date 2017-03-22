#include "stdafx.h"
#include "StateBase.h"
#include "State.h"


CStateBase::CStateBase(CVideoMarker2Dlg* pDlg) :_pDlg(pDlg)
{
}


CStateBase::~CStateBase()
{
}


/////////////////////////
////     public     /////
/////////////////////////

void CStateBase::RefreshButton()
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

void CStateBase::AddMark()
{

}

void CStateBase::SaveMark()
{

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


void CStateBase::Open()
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


void CStateBase::Pause()
{

}




void CStateBase::OpenTextFile()
{
	GetPresenter()->OpenTextFile();
}



void CStateBase::SaveTextFile()
{

}




void CStateBase::SetState(const std::string& state)
{
	_pDlg->SetState(state);
	_pDlg->ClearHighLight();
}

CWnd* CStateBase::GetDlgItem(int nID)
{
	return _pDlg->GetDlgItem(nID);
}

bool CStateBase::IsTextFileOpened() const
{
	return _pDlg->m_bTextStatus;
}

CVideoMarkerPresenter* CStateBase::GetPresenter()
{
	return _pDlg->m_pPresenter;
}














