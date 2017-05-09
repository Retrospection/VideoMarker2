#include "stdafx.h"
#include "SelectMarkState.h"

#include "State.h"
#include "VideoMarker2Dlg.h"

CSelectMarkState::CSelectMarkState(CVideoMarker2Dlg* pDlg, const UIConfig& config) :CStateBase(pDlg, config)
{
}


CSelectMarkState::~CSelectMarkState()
{
}

void CSelectMarkState::RefreshButton()
{
	m_ui.RefreshButton();
}

void CSelectMarkState::SelectMarkBtnClicked()
{
	GetPresenter()->SaveMark();
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"Ñ¡Ôñ±ê×¢");
	GetPictureBox()->SetEditType(CPictureBox::INIT_STATE);
//	_pDlg->UpdateListBoxFrameInfo(_pDlg->GetFrameInfo());    TODO
	SetState(ProjectOpened);
}

void CSelectMarkState::DeleteMarkBtnClicked()
{
	GetPictureBox()->DeleteSelectedFacesInfo();
	_pDlg->UpdateListBoxFrameInfo(GetPictureBox()->GetFaceInfoEx());    
	_pDlg->Refresh();
}
