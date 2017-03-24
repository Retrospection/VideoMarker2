#include "stdafx.h"
#include "S5.h"

#include "State.h"

#include "VideoMarker2Dlg.h"

CS5::CS5(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CS5::~CS5()
{
}

void CS5::RefreshButton()
{
	m_ui.RefreshButton();
	GetPictureBox()->SetDrawable(true);
}

void CS5::OpenProject()
{

}

void CS5::AddMark()
{

}

bool CS5::CanDraw() const
{
	return true;
}

void CS5::OnPictureBoxLBtnDown()
{
	_pDlg->GetDlgItem(IDC_BUTTON_ADDMARK)->EnableWindow(FALSE);
}




