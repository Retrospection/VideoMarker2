#pragma once


// CNameInputDialog �Ի���

class CNameInputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNameInputDialog)

public:
	CNameInputDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNameInputDialog();

// �Ի�������
	enum { IDD = IDD_NAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPersonName;
};
