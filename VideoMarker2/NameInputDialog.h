#pragma once


// CNameInputDialog 对话框

class CNameInputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNameInputDialog)

public:
	CNameInputDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNameInputDialog();

// 对话框数据
	enum { IDD = IDD_NAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPersonName;
};
