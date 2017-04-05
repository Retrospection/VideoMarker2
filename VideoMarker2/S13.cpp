#include "stdafx.h"
#include "S13.h"

#include "State.h"
#include "VideoMarker2Dlg.h"

CS13::CS13(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CS13::~CS13()
{
}

void CS13::RefreshButton()
{
	m_ui.RefreshButton();
	GetPictureBox()->SetDrawable(true);
}

void CS13::SelectMarkBtnClicked()
{
	GetPresenter()->SaveMark();
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"Ñ¡Ôñ±ê×¢");
	SetState(S1);
}

void CS13::DeleteMarkBtnClicked()
{
	GetPictureBox()->DeleteSelectedFacesInfo();
	_pDlg->Refresh();
}
