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
// 	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);										     // ���ô򿪰�ť
// 	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);											 // ���ò��Ű�ť
// 	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);											 // ������ͣ��ť
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);									         // ����ֹͣ��ť
// 	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);											 // ������ӱ�ע��ť
// 	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);											 // ���ô��ı���ť
// 	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);										     // ���ñ����ı���ť
// 	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // ���ý�����
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


