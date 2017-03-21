 
#pragma once

#include "VideoMarker2Dlg.h"
#include "VideoMarkerPresenter.h"

class CVideoMarkerPresenter;

class CVideoMarker2Dlg;



class CStateBase
{
public:
	CStateBase(CVideoMarker2Dlg* pDlg);

	virtual ~CStateBase();

	virtual void RefreshButton();

	virtual void Init();

	virtual void Open();
	virtual void Play();
	virtual void Stop();
	virtual void Pause();
	virtual void SeekTo(int nPos);
	virtual void ForwardOneFrame(int nCurrentFrameIndex);
	virtual void BackOneFrame(int nCurrentFrameIndex);
	virtual void OpenTextFile();
	virtual void SaveTextFile();
	virtual void AddMark();
	virtual void OnLButtonDown(int nFlags, cv::Point point);
	virtual void OnMouseMove(int nFlags, cv::Point point);
	virtual void OnLButtonUp(int nFlags, cv::Point point);
	virtual void SaveMark();

	virtual void OpenProject();


protected:
	void SetState(const std::string& state);
	CWnd* GetDlgItem(int nID);
	bool IsTextFileOpened() const;



protected:
//	CVideoMarkerPresenter* m_pPresenter;
	CVideoMarkerPresenter* GetPresenter();

protected:
	CVideoMarker2Dlg* _pDlg;
	

};