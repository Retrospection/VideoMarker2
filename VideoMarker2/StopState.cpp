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
// 	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);										     // ���ô򿪰�ť
// 	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);											 // ���ò��Ű�ť
// 	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);											 // ������ͣ��ť
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);									         // ����ֹͣ��ť
// 	GetDlgItem(IDC_BUTTON9)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);                // �����ı��ļ���״̬�����Ƿ����ð�ť
// 	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);											 // ���ô��ı���ť
// 	GetDlgItem(IDC_BUTTON4)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);				 // �����ı��ļ���״̬�����Ƿ����ð�ť
// 	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // ���ý�����

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

