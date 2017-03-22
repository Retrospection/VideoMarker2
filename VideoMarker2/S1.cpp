#include "stdafx.h"
#include "S1.h"


CS1::CS1(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg,config)
{
}


CS1::~CS1()
{
}

void CS1::RefreshButton()
{
	//GetDlgItem(IDC_BUTTON_OPENPROJECT)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_ADDMARK)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_REDO)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_STEPBACK)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_STEPFORWARD)->EnableWindow(TRUE);
	//GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);
	m_ui.RefreshButton();
}

void CS1::SaveMark()
{

}

void CS1::Undo()
{

}

void CS1::Redo()
{

}

