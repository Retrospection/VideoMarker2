#include "stdafx.h"
#include "ProjectOpenedState.h"

#include "VideoMarker2Dlg.h"


#include <iostream>

#include <thread>



CProjectOpenedState::CProjectOpenedState(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg,config)
{
}


CProjectOpenedState::~CProjectOpenedState()
{
}

void CProjectOpenedState::RefreshButton()
{
	m_ui.RefreshButton();
	GetDlgItem(IDC_BUTTON_STEPBACK)->EnableWindow((GetCurrentFrameIndex() != 0) ? TRUE : FALSE);
	GetDlgItem(IDC_BUTTON_STEPFORWARD)->EnableWindow((GetCurrentFrameIndex() < (GetTotalFrameCount() - 1)) ? TRUE : FALSE);
}

void CProjectOpenedState::AddSaveMarkBtnClicked()
{
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"完成编辑");
	GetPictureBox()->SetEditType(CPictureBox::ADD_MARK_STATE);
	SetState(AddMark);
}


void CProjectOpenedState::Undo()
{

}

void CProjectOpenedState::Redo()
{

}

void CProjectOpenedState::SelectMarkBtnClicked()
{
	GetPictureBox()->SetEditType(CPictureBox::SELECT_MARK_STATE);
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"保存修改");
	SetState(SelectMark);
}

void CProjectOpenedState::OnLbnSelchangeList1()
{
	int result = GetListBox()->GetCurSel();
	if (result < 0)
	{
		return;
	}
	size_t i = static_cast<size_t>(result);
	std::cout << "ListBox: " << i << std::endl;
	GetPictureBox()->SetHighLight(i);
}

void CProjectOpenedState::Play()
{
	SetTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER, 40,NULL);
	SetState(PLAY);
}

void CProjectOpenedState::Stop()
{
	KillTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER);
	GetPresenter()->Stop();

}



