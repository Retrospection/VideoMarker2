#include "stdafx.h"
#include "PauseState.h"


CPauseState::CPauseState(CVideoMarker2Dlg* pDlg)
	:CStateBase(pDlg)
{
}


CPauseState::~CPauseState()
{
}

void CPauseState::RefreshButton()
{

	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);										     // 禁用打开按钮
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);											 // 启用播放按钮
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);											 // 禁用暂停按钮
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);									         // 启用停止按钮
	GetDlgItem(IDC_BUTTON9)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);                // 根据文本文件打开状态决定是否启用按钮
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);											 // 启用打开文本按钮
	GetDlgItem(IDC_BUTTON4)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);				 // 根据文本文件打开状态决定是否启用按钮
	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // 启用进度条
}

void CPauseState::Play()
{
	GetPresenter()->Play();
	SetState(PLAY);
}

void CPauseState::Stop()
{
	GetPresenter()->Stop();
	SetState(STOP);
}


