#include "stdafx.h"
#include "InitState.h"


CInitState::CInitState(CVideoMarker2Dlg* pDlg) :CStateBase(pDlg)
{
}


CInitState::~CInitState()
{
}

void CInitState::RefreshButton()
{
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);   // 启用打开按钮
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  // 禁用播放按钮
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  // 禁用暂停按钮
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);  // 禁用停止按钮
	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);  // 禁用添加标注按钮
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);  // 禁用打开文本按钮
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);  // 禁用保存文本按钮
	GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);  // 禁用进度条
}



void CInitState::Open()
{
	GetPresenter()->Open();
	SetState(STOP);
}


