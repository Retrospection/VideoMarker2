#include "stdafx.h"
#include "S1.h"

#include "VideoMarker2Dlg.h"

CS1::CS1(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg,config)
{
}


CS1::~CS1()
{
}

void CS1::RefreshButton()
{
	m_ui.RefreshButton();
	GetPictureBox()->SetDrawable(false);
}

void CS1::AddSaveMarkBtnClicked()
{
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"完成编辑");
	GetPictureBox()->SetEditType(CPictureBox::ADD_MARK_TYPE);
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
	GetPictureBox()->SetEditType(CPictureBox::SELECT_MARK);
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"保存修改");
	SetState(S13);
}



