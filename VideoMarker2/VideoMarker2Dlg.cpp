
// VideoMarker2Dlg.cpp : 实现文件


#include "stdafx.h"
#include "VideoMarker2.h"
#include "VideoMarker2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>
#include <codecvt>
#include <cassert>

#include "CvvImage.h"
#include "StateFactory.h"
#include "DBox.h"
#include "DataExchange.h"



// cv::Rect CalcMaxFit(cv::Size target, cv::Size src, double& ratio)
// {
// 	// 宽高计算
// 	int width = 0;
// 	int height = 0;
// 	int x = 0;
// 	int y = 0;
// 	if (RatioOf(src) > RatioOf(target))
// 	{
// 		width = target.width;
// 		ratio = ((double)src.width) / target.width;
// 		height = src.height / ratio;
// 		x = 0;
// 		y = ((target.height / 2) - (height / 2));
// 	}
// 	else
// 	{
// 		height = target.height;
// 		ratio = ((double)src.height) / target.height;
// 		width = src.width / ratio;
// 		y = 0;
// 		x = ((target.width / 2) - (width / 2));
// 	}
// 	return  cv::Rect{ x, y, width, height };
// }

const cv::Scalar Green{ 0, 255, 0 };
const cv::Scalar Red{ 0, 0, 255 };
const cv::Scalar Black{ 0, 0, 0 };

const cv::Scalar ColorUnsaved = Red;
const cv::Scalar ColorSaved = Red;
const cv::Scalar ColorHighLight = Green;



// CVideoMarker2Dlg 对话框

CVideoMarker2Dlg::CVideoMarker2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoMarker2Dlg::IDD, pParent), m_Trans(Transformer::Default())
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
END_MESSAGE_MAP()

// CVideoMarker2Dlg 消息处理程序

BOOL CVideoMarker2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 滑条初始化
	m_Slider.SetRange(1, 100);
	m_Slider.SetTicFreq(20);

	CRect rc;
	GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(&rc);
	m_nOutputFrameHeight = rc.Height();
	m_nOutputFrameWidth = rc.Width();
	m_matBackGround = cv::Mat(m_nOutputFrameHeight, m_nOutputFrameWidth, CV_8UC3, Black);

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
	GetDlgItem(IDC_BUTTON7)->EnableWindow((m_nCurrentFrameIndex > 0) ? TRUE : FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow((m_nCurrentFrameIndex < m_nTotalFrameCount) ? TRUE : FALSE);
	PrepareImage();
	m_pPictureBox->SetImage(m_matBackGround);
	ShowFrameInfoInListBox();
	Invalidate(FALSE);
	
}

int CVideoMarker2Dlg::GetOutputFrameWidth() const
{
	CRect rc;
	GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(&rc);
	return rc.Width();
}

int CVideoMarker2Dlg::GetOutputFrameHeight() const
{
	CRect rc;
	GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(&rc);
	return rc.Height();
}

CString CVideoMarker2Dlg::GetFileName() const
{
	return m_cstrVideoFileName;
}

void CVideoMarker2Dlg::SetFileOpenedStatus(bool status)
{
	m_bStatus = status;
	m_bFirstFrame = status;
	m_matBackGround = cv::Mat(m_nOutputFrameHeight, m_nOutputFrameWidth, CV_8UC3, cv::Scalar(0, 0, 0));
}

void CVideoMarker2Dlg::SetRawFrame(const cv::Mat& frame)
{
	assert(m_bStatus);
	m_matRawFrame = frame;
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

void CVideoMarker2Dlg::Resize()
{
	if (m_bFirstFrame)
	{
		SetROI();
		m_bFirstFrame = false;
	}
	cv::resize(m_matRawFrame, m_matROI, m_matROI.size());


}

void CVideoMarker2Dlg::SetROI()
{
	assert(m_bFirstFrame);
	m_Trans = Transformer::Make({ m_nOutputFrameWidth, m_nOutputFrameHeight }, m_matRawFrame.size());
	m_matROI = m_matBackGround(m_Trans.GetRoiRect());
}

//void CVideoMarker2Dlg::CalcRect()
//{
//	//	m_ROIRect = cv::Rect(0, 0, 100, 100);
//
//	double nOutputFrameAspectRatio = ((double)m_nOutputFrameWidth) / m_nOutputFrameHeight;
//	double nRawFrameAspectRatio = ((double)m_matRawFrame.cols) / m_matRawFrame.rows;
//
//	// 宽高计算
//	int width = 0;
//	int height = 0;
//	int x = 0;
//	int y = 0;
//	if (nRawFrameAspectRatio > nOutputFrameAspectRatio)
//	{
//		width = m_nOutputFrameWidth;
//		m_dScaleRatio = ((double)m_matRawFrame.cols) / m_nOutputFrameWidth;
//		height = m_matRawFrame.rows / m_dScaleRatio;
//		x = 0;
//		y = ((m_nOutputFrameHeight / 2) - (height / 2));
//	}
//	else
//	{
//		height = m_nOutputFrameHeight;
//		m_dScaleRatio = ((double)m_matRawFrame.rows) / m_nOutputFrameHeight;
//		width = m_matRawFrame.cols / m_dScaleRatio;
//		y = 0;
//		x = ((m_nOutputFrameWidth / 2) - (width / 2));
//	}
//	m_ROIRect = cv::Rect{ x, y, width, height };
//}

void CVideoMarker2Dlg::RefreshSlider()
{
	CSliderCtrl* pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlidCtrl->SetPos(m_nCurrentFrameIndex);
	pSlidCtrl->SetRange(0, m_nTotalFrameCount == 0 ? 0 : m_nTotalFrameCount - 1);
}

CString CVideoMarker2Dlg::GetTextFileName() const
{
	return m_cstrTextFileName;
}

void CVideoMarker2Dlg::SetFrameInfo(const FrameInfo& frameInfo)
{
	m_FrameInfo = frameInfo;
}

void CVideoMarker2Dlg::PrepareImage()
{
	if (!m_bStatus)
	{
		return;
	}
	Resize();
	if (!m_bTextStatus)
	{
		return;
	}
	DrawFrameInfo();
}

void CVideoMarker2Dlg::DrawFrameInfo()
{
	for (const auto& faceInfo: m_FrameInfo.facesInfo)
	{
		drawables.push_back(new DBox(m_Trans.Trans(faceInfo.box, Transformer::Coordinate::Raw, Transformer::Coordinate::Roi),ColorSaved));
	}
	for (const auto& rect : m_pPictureBox->GetUnsavedBoxes())
	{
		drawables.push_back(new DBox(rect, ColorSaved));
	}
	//const cv::Point* pPoints = m_pPictureBox->GetActivePoints();
	const cv::Rect* pRect = m_pPictureBox 
	if (pPoints)
	{
		drawables.push_back(new DBox({ pPoints[0], pPoints[1] }, ColorUnsaved));
	}
	drawables.push_back(new DBox(m_HighLight, ColorHighLight));
	for (const auto& drawable : drawables)
	{
		drawable->Draw(m_matROI);
		delete drawable;
	}
	drawables.clear();
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

// void CVideoMarker2Dlg::ShowImage(const cv::Mat& frame, UINT ID)
// {
// 	if (!m_bStatus)
// 	{
// 		return;
// 	}
// 	CDC* pDC = GetDlgItem(ID)->GetDC();
// 	HDC hDC = pDC->GetSafeHdc();
// 
// 	CRect rect;
// 	GetDlgItem(ID)->GetClientRect(&rect);
// 
// 	int clientWidth = rect.right - rect.left; 
// 	int clientHeight = rect.bottom - rect.top;
// 	int iwidth = frame.cols;
// 	int iheight = frame.rows;
// 	int tx = (int)(clientWidth - iwidth) / 2;
// 	int ty = (int)(clientHeight - iheight) / 2;
// 
// 	SetRect(rect, tx, ty, tx + iwidth, ty + iheight);
// 	CvvImage cimg;
// 	cimg.CopyOf(&IplImage(frame));
// 	cimg.DrawToHDC(hDC, &rect);
// 	ReleaseDC(pDC);
// }

void CVideoMarker2Dlg::OnBnClickedOpenFileButton()
{
	CFileDialog fileDlg(TRUE, L"*.mp4", NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST, L"*.mp4|*.mp4|*.avi|*.avi||", NULL);
	fileDlg.m_ofn.lpstrTitle = L"选择要编辑的视频文件";
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	m_cstrVideoFileName = fileDlg.GetPathName();

	m_pState->Open();
}

const int PLAY_TIMER = 1;

void CVideoMarker2Dlg::OnBnClickedPlayVideoButton()
{
	SetTimer(PLAY_TIMER, 40, NULL);
}

void CVideoMarker2Dlg::OnBnClickedBackOneFrame()
{
}

void CVideoMarker2Dlg::OnBnClickedForwardOneFrame()
{
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

	m_cstrTextFileName = fileDlg.GetPathName();
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

	//m_ListBox.ResetContent();

	//for (auto faceInfo : m_FrameInfo.facesInfo)
	//{
	//	m_ListBox.AddString(ConvertFromFrameInfo(faceInfo).c_str());
	//}
	//if (m_AddPersonName.empty())
	//{
	//	return;
	//}
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	//for (size_t i = 0; i < m_AddPersonName.size(); ++i)
	//{
	//	m_ListBox.AddString(ConvertFromFrameInfo({ conv.to_bytes(m_AddPersonName[i].GetBuffer()), m_pPictureBox->m_boxes[i] }).c_str());
	//}

}


std::wstring CVideoMarker2Dlg::ConvertFromFrameInfo(const FaceInfo& faceInfo)
{
	std::wstringstream wss;
	wss << faceInfo.strPersonName.c_str() << " ";
	wss << faceInfo.box.height << " " << faceInfo.box.width << " " << faceInfo.box.y << " " << faceInfo.box.x;
	// 	wss << faceInfo.box.x << " " << faceInfo.box.y << " " << faceInfo.box.width << " " << faceInfo.box.height << " ";
	// 	wss << faceInfo.strPersonName.c_str();
	return wss.str();
}


//void CVideoMarker2Dlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	m_pState->OnLButtonDown(nFlags, ConvertFromCPoint(point));
//	CDialogEx::OnLButtonDown(nFlags, point);
//}


//void CVideoMarker2Dlg::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	m_pState->OnMouseMove(nFlags, ConvertFromCPoint(point));
//	CDialogEx::OnMouseMove(nFlags, point);
//}


//void CVideoMarker2Dlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	m_pState->OnLButtonUp(nFlags, ConvertFromCPoint(point));
//	CDialogEx::OnLButtonUp(nFlags, point);
//}

cv::Point CVideoMarker2Dlg::ConvertFromCPoint(const CPoint& point)
{
	return{ point.x, point.y };
}

//void CVideoMarker2Dlg::SetStartPoint(const cv::Point& startPoint)
//{
//	m_pPictureBox->m_TempPoints[0] = startPoint;
//}
//
//void CVideoMarker2Dlg::SetEndPoint(const cv::Point& endPoint)
//{
//	m_pPictureBox->m_TempPoints[1] = endPoint;
//}

// cv::Rect CVideoMarker2Dlg::GetROIRect() const
// {
// 	return m_ROIRect;
// }


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

	std::vector<std::string> info = Split(strItem, " ");

// 	m_HighLight.x = atoi(info[4].c_str()) / m_dScaleRatio;
// 	m_HighLight.y = atoi(info[3].c_str()) / m_dScaleRatio;
// 	m_HighLight.width = atoi(info[2].c_str()) / m_dScaleRatio;
// 	m_HighLight.height = atoi(info[1].c_str()) / m_dScaleRatio;

	Refresh();

}

std::vector<std::string> CVideoMarker2Dlg::Split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> ret;
	std::string::size_type pos1, pos2;
	pos2 = str.find(delim);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		ret.push_back(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = str.find(delim, pos1);
	}
	if (pos1 != str.length())
	{
		ret.push_back(str.substr(pos1));
	}
	return std::move(ret);

}

std::vector<cv::Rect> CVideoMarker2Dlg::GetUnsavedBox()
{
	return m_pPictureBox->GetUnsavedBoxes();
}
