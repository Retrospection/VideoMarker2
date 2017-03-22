#include "stdafx.h"
#include "StopState.h"

#include "VideoMarkerPresenter.h"
CStopState::CStopState(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CStopState::~CStopState()
{
}

void CStopState::RefreshButton()
{
// 	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);										     // 启用打开按钮
// 	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);											 // 启用播放按钮
// 	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);											 // 禁用暂停按钮
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);									         // 禁用停止按钮
// 	GetDlgItem(IDC_BUTTON9)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);                // 根据文本文件打开状态决定是否启用按钮
// 	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);											 // 启用打开文本按钮
// 	GetDlgItem(IDC_BUTTON4)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);				 // 根据文本文件打开状态决定是否启用按钮
// 	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // 启用进度条

}

// void CStopState::Play()
// {
// 	GetPresenter()->Play();
// 	SetState(PLAY);
// }

void CStopState::Open()
{
	GetPresenter()->Open();
	//SetState(STOP);
}

void CStopState::OpenTextFile()
{
	GetPresenter()->OpenTextFile();
	//SetState(STOP);
}

