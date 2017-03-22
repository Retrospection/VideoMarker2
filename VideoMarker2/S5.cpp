#include "stdafx.h"
#include "S5.h"

#include "State.h"

CS5::CS5(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CS5::~CS5()
{
}

void CS5::RefreshButton()
{
	//GetDlgItem(IDC_BUTTON_OPENPROJECT)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_ADDMARK)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_REDO)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_STEPBACK)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_STEPFORWARD)->EnableWindow(FALSE);
	//GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
}

void CS5::OpenProject()
{

}

void CS5::AddMark()
{

}

void CS5::OnLButtonDown(int nFlags, cv::Point point)
{
	SetState(S12);
}
