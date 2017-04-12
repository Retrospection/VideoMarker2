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
}

bool CS5::CanDraw() const
{
	return true;
}

void CS5::AddSaveMarkBtnClicked()
{
	GetPresenter()->SaveMark();
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"Ìí¼Ó±ê×¢");
	GetPictureBox()->SetEditType(CPictureBox::INIT_STATE);
	SetState(S1);
}

