#include "stdafx.h"
#include "S1.h"

#include "VideoMarker2Dlg.h"


#include <iostream>

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
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"完成编辑");
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
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"保存修改");
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
	SetPlaying(true);
	while ((GetCurrentFrameIndex() + 1 < GetTotalFrameCount()) && IsPlaying())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		GetPresenter()->ForwardOneFrame(GetCurrentFrameIndex());
	}
	SetPlaying(false);
	GetPresenter()->Stop();
	SetState(PLAY);
}



