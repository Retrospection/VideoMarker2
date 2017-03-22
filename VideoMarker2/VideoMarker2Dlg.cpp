
// VideoMarker2Dlg.cpp : 实现文件


#include "stdafx.h"
#include "VideoMarker2.h"
#include "VideoMarker2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <conio.h>
#include <string>
#include <codecvt>
#include <cassert>

#include <iostream>


#include "StateFactory.h"
#include "DataExchange.h"
#include "StringHelper.h"

// CVideoMarker2Dlg 对话框

CVideoMarker2Dlg::CVideoMarker2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoMarker2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pPictureBox = new CPictureBox(m_pState);
	m_pNameDlg = new CNameInputDialog(this);
	m_pPresenter = new CVideoMarkerPresenter(this);
}

void CVideoMarker2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_FRAME, *m_pPictureBox);
}

BEGIN_MESSAGE_MAP(CVideoMarker2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CVideoMarker2Dlg::OnBnClickedOpenFileButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CVideoMarker2Dlg::OnBnClickedPlayVideoButton)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON7, &CVideoMarker2Dlg::OnBnClickedBackOneFrame)
	ON_BN_CLICKED(IDC_BUTTON8, &CVideoMarker2Dlg::OnBnClickedForwardOneFrame)
	ON_BN_CLICKED(IDC_BUTTON5, &CVideoMarker2Dlg::OnBnClickedOpenTextFile)
	ON_BN_CLICKED(IDC_BUTTON6, &CVideoMarker2Dlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoMarker2Dlg::OnBnClickedPauseButton)
	ON_BN_CLICKED(IDC_BUTTON9, &CVideoMarker2Dlg::OnBnClickedAddMark)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CVideoMarker2Dlg::OnBnClickedButton4)
	ON_LBN_DBLCLK(IDC_LIST1, &CVideoMarker2Dlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_REVOKE, &CVideoMarker2Dlg::OnBnClickedButtonRevoke)
	ON_BN_CLICKED(IDC_BUTTON_REDO, &CVideoMarker2Dlg::OnBnClickedButtonRedo)
	ON_BN_CLICKED(IDC_BUTTON_PROJECT, &CVideoMarker2Dlg::OnBnClickedButtonProject)
END_MESSAGE_MAP()

// CVideoMarker2Dlg 消息处理程序

BOOL CVideoMarker2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	// 打开控制台
	BOOL result = AllocConsole();
	assert(result);
	freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
	freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT



	// 滑条初始化
	m_Slider.SetRange(1, 100);
	m_Slider.SetTicFreq(20);

	SetState(INIT);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoMarker2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete m_pPresenter;
	delete m_pPictureBox;
	delete m_pNameDlg;
//	delete m_pTrans;
	for (auto& p : m_States)
	{
		delete p.second;
	}
	m_States.clear();

	FreeConsole();

}

void CVideoMarker2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
//	Refresh();
}

HCURSOR CVideoMarker2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVideoMarker2Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//TODO:  在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	m_pState->SeekTo(pSlidCtrl->GetPos());
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVideoMarker2Dlg::Refresh()
{
	RefreshSlider();
	m_pState->RefreshButton();
// 	GetDlgItem(IDC_BUTTON7)->EnableWindow((m_nCurrentFrameIndex > 0) ? TRUE : FALSE);
// 	GetDlgItem(IDC_BUTTON8)->EnableWindow((m_nCurrentFrameIndex < m_nTotalFrameCount) ? TRUE : FALSE);
	ShowFrameInfoInListBox();
	Invalidate(FALSE);   // FIX IT
	
}

std::string CVideoMarker2Dlg::GetFileName() const 
{
	return m_strVideoFileName;
}

std::string CVideoMarker2Dlg::GetTextFileName() const
{
	std::cout << m_strTextFileName << std::endl;
	return m_strTextFileName;
}

void CVideoMarker2Dlg::SetFileOpenedStatus(bool status)
{
	m_bStatus = status;
}

void CVideoMarker2Dlg::SetRawFrame(const cv::Mat& frame)
{
	assert(m_bStatus);
	m_pPictureBox->SetImage(frame);
}

void CVideoMarker2Dlg::SetTotalFrameCount(int nTotalFrameCount)
{
	assert(nTotalFrameCount >= 0);
	m_nTotalFrameCount = nTotalFrameCount;
}

void CVideoMarker2Dlg::SetCurrentFrameIndex(int nCurrentFrameIndex)
{
	assert(nCurrentFrameIndex >= 0 && nCurrentFrameIndex < m_nTotalFrameCount);
	m_nCurrentFrameIndex = nCurrentFrameIndex;
}

void CVideoMarker2Dlg::SetTextFileOpenedStatus(bool status)
{
	m_bTextStatus = status;
}

void CVideoMarker2Dlg::RefreshSlider()
{
	CSliderCtrl* pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlidCtrl->SetPos(m_nCurrentFrameIndex);
	pSlidCtrl->SetRange(0, m_nTotalFrameCount == 0 ? 0 : m_nTotalFrameCount - 1);
}



void CVideoMarker2Dlg::SetFrameInfo(const FrameInfo& frameInfo)
{
	m_pPictureBox->SetFrameInfo(frameInfo);
	m_FrameInfo = frameInfo;
}

void CVideoMarker2Dlg::SetState(const std::string& state)
{
	auto iter = m_States.find(state);
	if (iter != m_States.end())
	{
		m_pState = iter->second;
		m_pPictureBox->SetState(m_pState);
		Refresh();
		return;
	}
	m_pState = CStateFactory::GetInstance().Create(state, this);
	m_States.insert(std::make_pair(state, m_pState));
	m_pPictureBox->SetState(m_pState);
	Refresh();
}

void CVideoMarker2Dlg::OnBnClickedOpenFileButton()
{
	CFileDialog fileDlg(TRUE, L"*.mp4", NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST, L"*.mp4|*.mp4|*.avi|*.avi||", NULL);
	fileDlg.m_ofn.lpstrTitle = L"选择要编辑的视频文件";
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	m_strVideoFileName = CStringHelper::ConvertCStringToString(fileDlg.GetPathName());

	m_pState->Open();
}


void CVideoMarker2Dlg::OnBnClickedPlayVideoButton()
{
	SetTimer(PLAY_TIMER, 40, NULL);
}

void CVideoMarker2Dlg::OnBnClickedBackOneFrame()
{
	m_pPictureBox->ClearUnsavedBoxes();
	m_pPictureBox->ClearUnsavedNames();
	m_pState->Pause();
	m_pState->BackOneFrame(m_nCurrentFrameIndex);

}

void CVideoMarker2Dlg::OnBnClickedForwardOneFrame()
{
	m_pPictureBox->ClearUnsavedBoxes();
	m_pPictureBox->ClearUnsavedNames();
	m_pState->Pause();
	m_pState->ForwardOneFrame(m_nCurrentFrameIndex);
}

void CVideoMarker2Dlg::OnBnClickedOpenTextFile()
{
	CFileDialog fileDlg(TRUE, L"*.txt", NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
		L"文本文件|*.txt||", NULL);
	fileDlg.m_ofn.lpstrTitle = L"选择要编辑的视频文件";
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	m_strTextFileName = CStringHelper::ConvertCStringToString(fileDlg.GetPathName());
	m_pState->OpenTextFile();
}

void CVideoMarker2Dlg::OnBnClickedStopButton()
{
	KillTimer(PLAY_TIMER);
	m_pState->Stop();
}

void CVideoMarker2Dlg::OnBnClickedPauseButton()
{
	KillTimer(PLAY_TIMER);
	m_pState->Pause();
}

void CVideoMarker2Dlg::OnBnClickedAddMark()
{
	CString str;
	GetDlgItemText(IDC_BUTTON9, str);
	if (str == L"完成编辑")
	{
		m_pState->SaveMark();
		SetState(PAUSE);
		SetDlgItemText(IDC_BUTTON9, L"添加标注");
	}
	else
	{
		SetState(EDIT_MARK);
	}
	Refresh();

}

void CVideoMarker2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_nCurrentFrameIndex+1 >= m_nTotalFrameCount)
	{
		CDialogEx::OnTimer(nIDEvent);
		return;
	}
	if (nIDEvent == PLAY_TIMER)
	{
		m_pState->Play();   // 
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CVideoMarker2Dlg::ShowFrameInfoInListBox()
{
	DataExchange de(&m_FrameInfo, &m_ListBox);
	de.Update(true);

}

void CVideoMarker2Dlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
}

void CVideoMarker2Dlg::OnLbnDblclkList1()
{
	// TODO:  在此添加控件通知处理程序代码
	int i = m_ListBox.GetCurSel();

	CString item;
	m_ListBox.GetText(i, item);

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string strItem = conv.to_bytes(item.GetBuffer());

	std::vector<std::string> info = CStringHelper::Split(strItem, " ");

	m_HighLight = { { atoi(info[1].c_str()), atoi(info[2].c_str()), atoi(info[3].c_str()), atoi(info[4].c_str()) } };

	m_pPictureBox->SetHighLight(m_HighLight);

}

void CVideoMarker2Dlg::OnBnClickedButtonRevoke()
{
	m_pPictureBox->Undo();
}

void CVideoMarker2Dlg::OnBnClickedButtonRedo()
{
	m_pPictureBox->Redo();
}

std::vector<cv::Rect> CVideoMarker2Dlg::GetUnsavedBox()
{
	return m_pPictureBox->GetUnsavedBoxesInRaw();
}

void CVideoMarker2Dlg::ClearHighLight()
{
	m_pPictureBox->SetHighLight({});
	m_HighLight = {};
}

std::vector<std::string> CVideoMarker2Dlg::GetUnsavedName()
{
	return m_pPictureBox->GetUnsavedNames();
}

void CVideoMarker2Dlg::ClearUnsavedFrameInfo()
{
	m_pPictureBox->ClearUnsavedNames();
	m_pPictureBox->ClearUnsavedBoxes();
}

unsigned int CVideoMarker2Dlg::ValidateFaceInfo(const FaceInfo& info)
{
	std::vector<FaceInfo> allUnsavedFaceInfo;
	std::vector<std::string> unsavedNames = m_pPictureBox->GetUnsavedNames();
	std::vector<cv::Rect> unsavedBoxes = m_pPictureBox->GetUnsavedBoxesInRaw();
	allUnsavedFaceInfo.resize(GetUnsavedName().size());
	std::transform(unsavedBoxes.begin(), unsavedBoxes.end(), unsavedNames.begin(), allUnsavedFaceInfo.begin(), [](const cv::Rect& box, const std::string& strPersonName )->FaceInfo{ return{ strPersonName, box }; });
	allUnsavedFaceInfo.push_back(info);
	return m_pPresenter->ValidateFacesInfo(allUnsavedFaceInfo);
}

int CVideoMarker2Dlg::GetCurrentFrameIndex() const
{
	return m_nCurrentFrameIndex;
}



void CVideoMarker2Dlg::OnBnClickedButtonProject()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strProjectFileName = "D:\\data\\data\\Actor\\project.txt";
	m_pState->OpenProject();

}

std::string CVideoMarker2Dlg::GetProjectFileName() const
{
	return m_strProjectFileName;
}
