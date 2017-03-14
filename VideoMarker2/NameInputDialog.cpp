// NameInputDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoMarker2.h"
#include "NameInputDialog.h"
#include "afxdialogex.h"


// CNameInputDialog 对话框

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


// CNameInputDialog 消息处理程序
