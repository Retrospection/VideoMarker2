#include "stdafx.h"
#include "PlayState.h"


CPlayState::CPlayState(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CPlayState::~CPlayState()
{
}




void CPlayState::RefreshButton()
{
// 	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);										     // 启用打开按钮
// 	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);											 // 禁用播放按钮
// 	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);											 // 禁用暂停按钮
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);									         // 禁用停止按钮
// 	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);											 // 禁用添加标注按钮
// 	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);											 // 禁用打开文本按钮
// 	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);										     // 禁用保存文本按钮
// 	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // 启用进度条
}

void CPlayState::Pause()
{
	GetPresenter()->Pause();
	SetState(PAUSE);
}

void CPlayState::Stop()
{
	GetPresenter()->Stop();
	SetState(STOP);
}


