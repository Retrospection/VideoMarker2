// NameInputDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoMarker2.h"
#include "NameInputDialog.h"
#include "afxdialogex.h"


// CNameInputDialog �Ի���

IMPLEMENT_DYNAMIC(CNameInputDialog, CDialogEx)

CNameInputDialog::CNameInputDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNameInputDialog::IDD, pParent)
	, m_strPersonName(_T(""))
{

}

CNameInputDialog::~CNameInputDialog()
{
}

void CNameInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strPersonName);
}


BEGIN_MESSAGE_MAP(CNameInputDialog, CDialogEx)
END_MESSAGE_MAP()


// CNameInputDialog ��Ϣ�������
