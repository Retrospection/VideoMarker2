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
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);   // ���ô򿪰�ť
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  // ���ò��Ű�ť
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  // ������ͣ��ť
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);  // ����ֹͣ��ť
	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);  // ������ӱ�ע��ť
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);  // ���ô��ı���ť
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);  // ���ñ����ı���ť
	GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);  // ���ý�����
}



void CInitState::Open()
{
	GetPresenter()->Open();
	SetState(STOP);
}


