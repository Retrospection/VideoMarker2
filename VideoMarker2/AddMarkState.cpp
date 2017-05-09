#include "stdafx.h"
#include "AddMarkState.h"

#include "State.h"

#include "VideoMarker2Dlg.h"

CAddMarkState::CAddMarkState(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CAddMarkState::~CAddMarkState()
{
}

void CAddMarkState::RefreshButton()
{
	m_ui.RefreshButton();
}

bool CAddMarkState::CanDraw() const
{
	return true;
}

void CAddMarkState::AddSaveMarkBtnClicked()
{
	GetPresenter()->SaveMark();
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"Ìí¼Ó±ê×¢");
	GetPictureBox()->SetEditType(CPictureBox::INIT_STATE);
	SetState(ProjectOpened);
}

