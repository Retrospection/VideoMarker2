m_pPictureBox
m_pPictureBox
m_pPictureBox
m_pPictureBox
#include "stdafx.h"
#include "StateBase.h"
#include "State.h"

#include "VideoMarker2Dlg.h"

CStateBase::CStateBase(CVideoMarker2Dlg* pDlg) :_pDlg(pDlg)
{
}

CStateBase::CStateBase(CVideoMarker2Dlg* pDlg, const UIConfig& config) : _pDlg(pDlg), m_ui(pDlg,config)
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
	GetPresenter()->Close();
	wchar_t* alertMessage[] = { L"", L"Load Project File Failed!", L"open video failed!", L"open text failed!" };
	size_t result = GetPresenter()->OpenProject();
	if (result != 0)
	{
		_pDlg->MessageBox(alertMessage[result]);
		SetState(INIT);
		_pDlg->SetRawFrame(cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0)));
		return;
	}
	SetState(S1);
}

void CStateBase::AddMark()
{
	SetState(S5);
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK,L"Íê³É±à¼­");
}

void CStateBase::SaveMark()
{
	GetPresenter()->SaveMark();
	SetState(S1);
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

}

void CStateBase::Stop()
{
}


void CStateBase::Pause()
{

}




void CStateBase::OpenTextFile()
{

}


#include <iostream>
void CStateBase::SetState(const std::string& state)
{
	std::cout << "set state " << state << std::endl;
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

void CStateBase::Undo()
{

}

void CStateBase::Redo()
{

}

bool CStateBase::CanDraw() const
{
	return false;
}

CPictureBox* CStateBase::GetPictureBox() const
{
	return _pDlg->m_pPictureBox;
}

void CStateBase::OnPictureBoxLBtnDown()
{
	_pDlg->GetDlgItem(IDC_BUTTON_ADDMARK)->EnableWindow(TRUE);
}

void CStateBase::OnPictureBoxLBtnUp()
{
	_pDlg->GetDlgItem(IDC_BUTTON_ADDMARK)->EnableWindow(TRUE);
}















CUI& CUI::Enable(const std::string& strItemName)
{
	return Enable(strItemName, true);
}

CUI& CUI::Enable(const std::string& strItemName, bool bEnable)
{
	int id = 0;
	bool bResult = FindID(id, strItemName);
	assert(bResult);
	m_pDlg->GetDlgItem(id)->EnableWindow(bEnable ? TRUE : FALSE);
	return *this;
}

bool CUI::FindID(int& id, const std::string& strItemName)
{
	auto iter = m_IDByName.find(strItemName);
	if (iter == m_IDByName.end())
	{
		return false;
	}
	id = iter->second;
	return true;
}

CUI::CUI(CVideoMarker2Dlg* pDlg, const UIConfig& config) :m_pDlg(pDlg), m_Config(config)
{
	m_IDByName.insert(std::make_pair("OpenProject", IDC_BUTTON_OPENPROJECT));
	m_IDByName.insert(std::make_pair("StepForward", IDC_BUTTON_STEPFORWARD));
	m_IDByName.insert(std::make_pair("StepBackward", IDC_BUTTON_STEPBACK));
	m_IDByName.insert(std::make_pair("AddMark", IDC_BUTTON_ADDMARK));
	m_IDByName.insert(std::make_pair("Slider", IDC_SLIDER_1));
	m_IDByName.insert(std::make_pair("Undo", IDC_BUTTON_UNDO));
	m_IDByName.insert(std::make_pair("Redo", IDC_BUTTON_REDO));
}

CUI::CUI()
{

}

CUI::~CUI()
{

}

CUI& CUI::Disable(const std::string& strItemName)
{
	return Enable(strItemName, false);
}

void CUI::RefreshButton()
{
	for (auto& enable: m_Config.enables)
	{
		Enable(enable);
	}
	for (auto& disable : m_Config.disnables)
	{
		Disable(disable);
	}

}
