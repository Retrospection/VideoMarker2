
// VideoMarker2Dlg.h : 头文件
//

#pragma once

#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <features2d/features2d.hpp>
#include <video/video.hpp>
#include "afxcmn.h"

#include "VideoMarkerPresenter.h"
#include "FrameInfo.h"

#include "State.h"
#include "PictureBox.h"

#include <unordered_map>
#include "afxwin.h"
#include "NameInputDialog.h"

#include "IVideoMarker2.h"

#include "Transformer.h"


class CVideoMarkerPresenter;

class CStateBase;

class CPictureBox;


class IDrawable;
#include <vector>

// CVideoMarker2Dlg 对话框
class CVideoMarker2Dlg : public CDialogEx, public IVideoMarker2
{
public:
	friend class CStateBase;
	friend class CVideoMarkerPresenter;
	friend class CPictureBox;

public:
	void Refresh();

// Getter
public:
	int GetOutputFrameWidth() const;
	int GetOutputFrameHeight() const;
	CString GetFileName()const;
	CString GetTextFileName()const;
//	cv::Rect GetROIRect() const;

// States Manager
public:
	void SetTextFileOpenedStatus(bool status);

	/*
	 * 由CInitState::Open()使用，设置 m_matRawFrame / m_nTotalFrameCount / m_nCurrentFrameIndex / m_bStatus
	 * 
	 */
public:
	void SetRawFrame(const cv::Mat& frame);
	void SetTotalFrameCount(int nTotalFrameCount);
	void SetCurrentFrameIndex(int nCurrentFrameIndex);
	void SetFileOpenedStatus(bool status);

public:
	void SetFrameInfo(const FrameInfo& frameInfo);

	virtual std::vector<cv::Rect> GetUnsavedBox() override;

//////////////////////////////////////////////////////  实现  ////////////////////////////////////////////////

// Refresh
private:
	void Resize();
	void SetROI();
	void DrawFrameInfo();
	void PrepareImage();
	void RefreshSlider();


private:
	CVideoMarkerPresenter* m_pPresenter;
	CNameInputDialog* m_pNameDlg;
	Transformer m_Trans;

private:
	bool m_bStatus;
	bool m_bTextStatus;

private:
	CString m_cstrTextFileName;


// 绘制图像信息相关成员
private:
	int m_nOutputFrameWidth;
	int m_nOutputFrameHeight;
	int m_nTotalFrameCount;
	cv::Mat m_matRawFrame;
	cv::Mat m_matBackGround;
	cv::Mat m_matROI;

	FrameInfo m_FrameInfo;

	cv::Rect m_HighLight;

private:
	CString m_cstrVideoFileName;
	std::vector<CString> m_AddPersonName;
	bool m_bFirstFrame;
	
	std::vector<IDrawable*> drawables;


// 控件
private:
	CSliderCtrl m_Slider;
	int m_nCurrentFrameIndex;
	CListBox m_ListBox;
	CPictureBox* m_pPictureBox;


// 状态管理
// 在SetState(const std::string& state)中更新
// 如果m_States中有，则直接从m_States中取出指针
// 如果没有，new之后insert并return。
private:
	CStateBase* m_pState;
	std::unordered_map<std::string, CStateBase*> m_States;



private:
	void SetState(const std::string& state);
	void ShowFrameInfoInListBox();
	std::wstring ConvertFromFrameInfo(const FaceInfo& faceInfo);
	cv::Point ConvertFromCPoint(const CPoint& point);

	std::vector<std::string> Split(const std::string& str, const std::string& delim);

//////////////////////////////////////////////////////  实现  ////////////////////////////////////////////////


// 构造
public:
	CVideoMarker2Dlg(CWnd* pParent = NULL);	
// 对话框数据
	enum { IDD = IDD_VIDEOMARKER2_DIALOG };
// DDX/DDV 支持
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// 消息响应函数
public:
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOpenFileButton();
	afx_msg void OnBnClickedPlayVideoButton();
	afx_msg void OnBnClickedBackOneFrame();
	afx_msg void OnBnClickedForwardOneFrame();
	afx_msg void OnBnClickedOpenTextFile();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedPauseButton();
	afx_msg void OnBnClickedAddMark();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLbnDblclkList1();



};
