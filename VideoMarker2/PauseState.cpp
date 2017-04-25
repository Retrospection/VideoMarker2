#include "stdafx.h"
#include "PauseState.h"

#include "state.h"

#include "VideoMarkerPresenter.h"
#include "VideoMarker2Dlg.h"

CPauseState::CPauseState(CVideoMarker2Dlg* pDlg, const UIConfig& config)
	:CStateBase(pDlg, config)
{
}


CPauseState::~CPauseState()
{
}

void CPauseState::RefreshButton()
{
	m_ui.RefreshButton();
}

void CPauseState::Play()
{
//	StartPlayThread();
	SetTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER, 40, NULL);
	SetState(PLAY);
}

void CPauseState::Stop()
{
	KillTimer(_pDlg->m_hWnd, CVideoMarker2Dlg::PLAY_TIMER);
	GetPresenter()->Stop();
	SetState(S1);
}

void CPauseState::AddSaveMarkBtnClicked()
{
	_pDlg->SetDlgItemText(IDC_BUTTON_ADDMARK, L"完成编辑");
	GetPictureBox()->SetEditType(CPictureBox::ADD_MARK_STATE);
	SetState(S5);
}

void CPauseState::Undo()
{

}

void CPauseState::Redo()
{

}

void CPauseState::SelectMarkBtnClicked()
{
	GetPictureBox()->SetEditType(CPictureBox::SELECT_MARK_STATE);
	_pDlg->SetDlgItemText(IDC_BUTTON_SELECTMARK, L"保存修改");
	SetState(S13);
}

void CPauseState::OnLbnSelchangeList1()
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


