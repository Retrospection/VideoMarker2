
// VideoMarker2Dlg.h : ͷ�ļ�
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

// CVideoMarker2Dlg �Ի���
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
	 * ��CInitState::Open()ʹ�ã����� m_matRawFrame / m_nTotalFrameCount / m_nCurrentFrameIndex / m_bStatus
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

//////////////////////////////////////////////////////  ʵ��  ////////////////////////////////////////////////

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


// ����ͼ����Ϣ��س�Ա
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


// �ؼ�
private:
	CSliderCtrl m_Slider;
	int m_nCurrentFrameIndex;
	CListBox m_ListBox;
	CPictureBox* m_pPictureBox;


// ״̬����
// ��SetState(const std::string& state)�и���
// ���m_States���У���ֱ�Ӵ�m_States��ȡ��ָ��
// ���û�У�new֮��insert��return��
private:
	CStateBase* m_pState;
	std::unordered_map<std::string, CStateBase*> m_States;



private:
	void SetState(const std::string& state);
	void ShowFrameInfoInListBox();
	std::wstring ConvertFromFrameInfo(const FaceInfo& faceInfo);
	cv::Point ConvertFromCPoint(const CPoint& point);

	std::vector<std::string> Split(const std::string& str, const std::string& delim);

//////////////////////////////////////////////////////  ʵ��  ////////////////////////////////////////////////


// ����
public:
	CVideoMarker2Dlg(CWnd* pParent = NULL);	
// �Ի�������
	enum { IDD = IDD_VIDEOMARKER2_DIALOG };
// DDX/DDV ֧��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// ��Ϣ��Ӧ����
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
