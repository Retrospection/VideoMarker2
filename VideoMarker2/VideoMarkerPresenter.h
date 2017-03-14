#pragma once

#include "VideoMarker2.h"
#include "VideoPlayer.h"
#include "VideoMarker2Dlg.h"
#include "TextFileManager.h"
#include "FrameInfo.h"


class CVideoMarker2Dlg;

class CVideoMarkerPresenter
{
public:
	CVideoMarkerPresenter(CVideoMarker2Dlg* pDlg);
	~CVideoMarkerPresenter();

public:
	void Open();
	void Play();
	void Stop();
	void Pause();
	void SeekTo(int nPos);
	void ForwardOneFrame(int nCurrentFrameIndex);
	void BackOneFrame(int nCurrentFrameIndex);
	void OpenTextFile();
	void SaveTextFile();
	void AddMark();
	void SaveMark(); 

public:
	int GetTotalFrameCount() const;


// private:
// 	cv::Point ConvertMousePointToPicturePoint(const cv::Point& point);

private:
//	cv::Point m_AddPoints[2];
	std::vector<cv::Point> m_AddPoints;
	std::vector<CString> m_AddPersonNames;
	CString m_cstrAddPersonName;
	bool m_bDrawing;

private:
	CVideoPlayer* m_pVideoPlayer;
	CTextFileManager* m_pTextMgr;
	CVideoMarker2Dlg* m_pDlg;
};
