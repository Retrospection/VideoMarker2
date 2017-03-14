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

	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);										     // ���ô򿪰�ť
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);											 // ���ò��Ű�ť
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);											 // ������ͣ��ť
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);									         // ����ֹͣ��ť
	GetDlgItem(IDC_BUTTON9)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);                // �����ı��ļ���״̬�����Ƿ����ð�ť
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);											 // ���ô��ı���ť
	GetDlgItem(IDC_BUTTON4)->EnableWindow(IsTextFileOpened() ? TRUE : FALSE);				 // �����ı��ļ���״̬�����Ƿ����ð�ť
	GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);										     // ���ý�����
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


