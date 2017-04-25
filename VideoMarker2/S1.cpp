#include "stdafx.h"
#include "S1.h"

#include "VideoMarker2Dlg.h"


#include <iostream>

#include <thread>



CS1::CS1(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg,config)
{
}


CS1::~CS1()
{
}

void CS1::RefreshButton()
{
	m_ui.RefreshButton();
	GetDlgItem(IDC_BUTTON_STEPBACK)->EnableWindow((GetCurrentFrameIndex() != 0) ? TRUE : FALSE);
	GetDlgItem(IDC_BUTTON_STEPFORWARD)->EnableWindow((GetCurrentFrameIndex() < (GetTotalFrameCount() - 1)) ? TRUE : FALSE);
}

void CS1::AddSaveMarkBtnClicked()
{
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"��ɱ༭");
	GetPictureBox()->SetEditType(CPictureBox::ADD_MARK_STATE);
	SetState(S5);
}


void CS1::Undo()
{

}

void CS1::Redo()
{

}

void CS1::SelectMarkBtnClicked()
{
	GetPictureBox()->SetEditType(CPictureBox::SELECT_MARK_STATE);
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"�����޸�");
	SetState(S13);
}

void CS1::OnLbnSelchangeList1()
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

void CS1::Play()
{
	SetTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER, 40,NULL);
	SetState(PLAY);
}

void CS1::Stop()
{
	KillTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER);
	GetPresenter()->Stop();

}



