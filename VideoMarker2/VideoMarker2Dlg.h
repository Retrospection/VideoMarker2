
// VideoMarker2Dlg.h : 头文件
//

#pragma once

#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <features2d/features2d.hpp>

#include <unordered_map>
#include <vector>


#include "afxcmn.h"
#include "afxwin.h"

#include "VideoMarkerPresenter.h"
#include "FrameInfo.h"
#include "State.h"
#include "PictureBox.h"
#include "NameInputDialog.h"
#include "IVideoMarker2.h"

class CVideoMarkerPresenter;
class CStateBase;
class CPictureBox;

const int PLAY_TIMER = 1;



// CVideoMarker2Dlg 对话框
class CVideoMarker2Dlg : public CDialogEx, public IVideoMarker2
{
public:
	friend class CStateBase;
	friend class CVideoMarkerPresenter;
	friend class CPictureBox;

public:
	virtual void Refresh() override;
	void RefreshSlider();

public:
	std::string GetFileName() const;
	std::string GetTextFileName()const;
	std::string GetProjectFileName() const;

public:
	virtual int GetCurrentFrameIndex() const override;
	virtual std::vector<FaceInfo> GetUnsavedFacesInfo() override;
	virtual void ClearDeleteFrameInfo() override;
	virtual FrameInfo GetFrameInfo() const override;


	bool GetUnsavedName2(std::string& unsavedName);


public:
	virtual void SetFrameInfo(const FrameInfo& frameInfo) override;

	void SetTextFileOpenedStatus(bool status);
	void SetRawFrame(const cv::Mat& frame);
	void SetTotalFrameCount(int nTotalFrameCount);
	void SetCurrentFrameIndex(int nCurrentFrameIndex);
	void SetFileOpenedStatus(bool status);
	void ClearUnsavedFrameInfo();


	unsigned int ValidateFaceInfo();



//////////////////////////////////////////////////////  实现  ////////////////////////////////////////////////
private:
	CVideoMarkerPresenter* m_pPresenter;
	CNameInputDialog* m_pNameDlg;

private:
	bool m_bStatus;
	bool m_bTextStatus;

private:
 	int m_nTotalFrameCount;
	FrameInfo m_FrameInfo;


private:
	std::string m_strTextFileName;
	std::string m_strVideoFileName;
	std::string m_strProjectFileName;

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
	std::unordered_map<std::string, UIConfig> m_UIConfigs;
	bool m_bUIConfigLoaded;

private:
	void SetState(const std::string& state);

	// ListBox 相关
	void ShowFrameInfoInListBox();
	void ClearHighLight();

	// UIConfig
	bool GetUIConfig(UIConfig& config, const std::string& state);
	void LoadUIConfig();
	std::vector<std::string> GetLines(const std::string& filename);

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
	afx_msg void OnBnClickedButtonRevoke();
	afx_msg void OnBnClickedButtonRedo();
	afx_msg void OnBnClickedButtonProject();
	afx_msg void OnBnClickedButtonDeletemark();
	afx_msg void OnBnClickedButtonSelectMark();
	afx_msg void OnLbnSelchangeList1();
};
