
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
#include <fstream>


#include "StateFactory.h"
#include "DataExchange.h"
#include "StringHelper.h"
#include "message.h"

// CVideoMarker2Dlg 对话框

CVideoMarker2Dlg::CVideoMarker2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoMarker2Dlg::IDD, pParent), m_bUIConfigLoaded(false)/*, m_Timer(40, std::bind(&CVideoMarker2Dlg::OnTimer2, this))*/
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pPictureBox = new CPictureBox(/*m_pState*/);
	m_pNameDlg = new CNameInputDialog(this);
	m_pPresenter = new CVideoMarkerPresenter(this);
}

void CVideoMarker2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_1, m_Slider);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_FRAME, *m_pPictureBox);
}

BEGIN_MESSAGE_MAP(CVideoMarker2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CVideoMarker2Dlg::OnBnClickedOpenFileButton)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CVideoMarker2Dlg::OnBnClickedPlayVideoButton)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_STEPBACK, &CVideoMarker2Dlg::OnBnClickedBackOneFrame)
	ON_BN_CLICKED(IDC_BUTTON_STEPFORWARD, &CVideoMarker2Dlg::OnBnClickedForwardOneFrame)
	ON_BN_CLICKED(IDC_BUTTON6, &CVideoMarker2Dlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CVideoMarker2Dlg::OnBnClickedPauseButton)
	ON_BN_CLICKED(IDC_BUTTON_ADDMARK, &CVideoMarker2Dlg::OnBnClickedAddMark)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CVideoMarker2Dlg::OnBnClickedButtonRevoke)
	ON_BN_CLICKED(IDC_BUTTON_REDO, &CVideoMarker2Dlg::OnBnClickedButtonRedo)
	ON_BN_CLICKED(IDC_BUTTON_OPENPROJECT, &CVideoMarker2Dlg::OnBnClickedButtonProject)
	ON_BN_CLICKED(IDC_BUTTON_DELETEMARK, &CVideoMarker2Dlg::OnBnClickedButtonDeletemark)
	ON_BN_CLICKED(IDC_BUTTON_SELECTMARK, &CVideoMarker2Dlg::OnBnClickedButtonSelectMark)
	ON_LBN_SELCHANGE(IDC_LIST1, &CVideoMarker2Dlg::OnLbnSelchangeList1)
	ON_WM_TIMER()
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
	LoadUIConfig();
	SetState(INIT);
	SetRawFrame(cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0)));


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoMarker2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete m_pPresenter;
	delete m_pPictureBox;
	delete m_pNameDlg;
	for (auto& p : m_States)
	{
		delete p.second;
	}
	m_States.clear();
	m_bUIConfigLoaded = false;
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
	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_1);
	m_pState->SeekTo(pSlidCtrl->GetPos());
	m_pState->RefreshButton();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

}

void CVideoMarker2Dlg::Refresh()
{
	RefreshSlider();
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
	std::cout << "CurrentFrameIndex:" << m_nCurrentFrameIndex << std::endl;
}

void CVideoMarker2Dlg::SetTextFileOpenedStatus(bool status)
{
	m_bTextStatus = status;
}

void CVideoMarker2Dlg::RefreshSlider()
{

	CSliderCtrl* pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_1);
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
		Refresh();
		m_pState->RefreshButton();
		if (state == PLAY)
		{
			m_pState->Play();
		}

		return;
	}

	UIConfig config;
	bool bResult = GetUIConfig(config, state);
	assert(bResult);
	m_pState = CStateFactory::GetInstance().Create(state, this, config);
	m_States.insert(std::make_pair(state, m_pState));
	Refresh();
	m_pState->RefreshButton();

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




void CVideoMarker2Dlg::OnBnClickedBackOneFrame()
{
	m_pPictureBox->ClearUnsavedFaceInfo();
	m_pState->Pause();
	m_pState->BackOneFrame(m_nCurrentFrameIndex);
	m_pState->RefreshButton();
}

void CVideoMarker2Dlg::OnBnClickedForwardOneFrame()
{
	m_pPictureBox->ClearUnsavedFaceInfo();
	m_pState->Pause();

	m_pState->ForwardOneFrame(m_nCurrentFrameIndex);

	m_pState->RefreshButton();
}

void CVideoMarker2Dlg::OnBnClickedPlayVideoButton()
{
	//assert(!m_bPlaying);
	m_pState->Play();
}

void CVideoMarker2Dlg::OnBnClickedStopButton()
{
	m_pState->Stop();
}

void CVideoMarker2Dlg::OnBnClickedPauseButton()
{
	m_pState->Pause();
}

void CVideoMarker2Dlg::OnBnClickedAddMark()
{
	m_pState->AddSaveMarkBtnClicked();
}

void CVideoMarker2Dlg::Play()
{
	m_pPresenter->ForwardOneFrame(m_nCurrentFrameIndex);
}

void CVideoMarker2Dlg::ShowFrameInfoInListBox()
{
	//DataExchange de(&m_FrameInfo, &m_ListBox);
	DataExchange de(&m_NewFrameInfo, &m_ListBox);
	de.Update(true);
}

void CVideoMarker2Dlg::OnBnClickedButtonRevoke()
{
	m_pPictureBox->Undo();
}

void CVideoMarker2Dlg::OnBnClickedButtonRedo()
{
	m_pPictureBox->Redo();
}

void CVideoMarker2Dlg::ClearHighLight()
{
	m_pPictureBox->ClearHighLight();
}

void CVideoMarker2Dlg::ClearUnsavedFrameInfo()
{
	m_pPictureBox->ClearUnsavedFaceInfo();
}

unsigned int CVideoMarker2Dlg::ValidateFaceInfo()
{
	FrameInfo frameInfo = m_pPictureBox->GetFrameInfo();
	//std::cout << "待验证的 facesinfo 为: " << frameInfo.toString() << std::endl;
	return m_pPresenter->ValidateFacesInfo(frameInfo.facesInfo);
}

int CVideoMarker2Dlg::GetCurrentFrameIndex() const
{
	return m_nCurrentFrameIndex;
}

void CVideoMarker2Dlg::OnBnClickedButtonProject()
{
	CFileDialog fileDlg(TRUE, L"*.txt", NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST, L"文本文件|*.txt||", NULL);
	fileDlg.m_ofn.lpstrTitle = L"请选择工程文件";
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	m_strProjectFileName = CStringHelper::ConvertCStringToString(fileDlg.GetPathName());
	m_pState->OpenProject();

}

std::string CVideoMarker2Dlg::GetProjectFileName() const
{
	return m_strProjectFileName;
}

bool  CVideoMarker2Dlg::GetUIConfig(UIConfig& config, const std::string& state)
{
	auto iter = m_UIConfigs.find(state);
	if (iter == m_UIConfigs.end())
	{
		return false;
	}
	config = iter->second;
	return true;
}

void CVideoMarker2Dlg::LoadUIConfig()
{
	if (m_bUIConfigLoaded)
	{
		return;
	}
	std::vector<std::string> lines = GetLines("D:\\WorkSpace\\VideoMarker2\\UIConfig.txt");
	size_t numberOfLine = lines.size();
	assert(numberOfLine % 3 == 0 && numberOfLine != 0);
	for (size_t i = 0; i < numberOfLine; i += 3)
	{
		auto iter = m_UIConfigs.find(lines[i]);
		assert(iter == m_UIConfigs.end());
		std::vector<std::string> enables = CStringHelper::Split(lines[i + 1], " ");
		std::vector<std::string> disables = CStringHelper::Split(lines[i + 2], " ");
		m_UIConfigs.insert(std::make_pair(lines[i], UIConfig{enables,disables}));
	}
	m_bUIConfigLoaded = true;
}

std::vector<std::string> CVideoMarker2Dlg::GetLines(const std::string& filename)
{
	std::ifstream ifs(filename);
	assert(ifs.is_open());
	std::vector<std::string> ret;
	std::string line;
	while (std::getline(ifs,line))
	{
		ret.push_back(line);
	}
	return ret;
}

bool CVideoMarker2Dlg::GetUnsavedName2(std::string& unsavedName)
{
	if (m_pNameDlg->DoModal() == IDCANCEL)
	{
		return false;
	}
	unsavedName = CStringHelper::ConvertCStringToString(m_pNameDlg->m_strPersonName);
	return true;
}

void CVideoMarker2Dlg::OnBnClickedButtonDeletemark()
{
	m_pState->DeleteMarkBtnClicked();
}

FrameInfo CVideoMarker2Dlg::GetFrameInfo() const 
{
	return m_pPictureBox->GetFrameInfo();
}

void CVideoMarker2Dlg::OnBnClickedButtonSelectMark()
{
	m_pState->SelectMarkBtnClicked();
}

std::vector<FaceInfo> CVideoMarker2Dlg::GetUnsavedFacesInfo()
{
	return m_pPictureBox->GetUnsavedFrameInfo().facesInfo;
}

void CVideoMarker2Dlg::OnLbnSelchangeList1()
{
	m_pState->OnLbnSelchangeList1();
}

// void CVideoMarker2Dlg::Stop()
// {
// 	m_bPlaying = false;
// }
// 
// void CVideoMarker2Dlg::SetPlaying(bool bPlaying)
// {
// 	while (!m_PlayingMutex.try_lock())
// 	{
// 		return;
// 	}
// 	printf("[lock]---m_bPlaying has been locked by button!\n");
// 	m_bPlaying = bPlaying;
// 	m_PlayingMutex.unlock();
// 	printf("[unlock]---m_bPlaying has been unlocked!\n");
// }


void CVideoMarker2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == PLAY_TIMER)
	{
		Play();
	}

	__super::OnTimer(nIDEvent);
}

void CVideoMarker2Dlg::SetNewFrameInfo(const FrameInfo& newFrameInfo)
{
	m_NewFrameInfo = newFrameInfo;
}
