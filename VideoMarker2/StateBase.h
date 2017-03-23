 
#pragma once

#include <opencv2/core/core.hpp>


#include <string>
#include <unordered_map>

class CVideoMarkerPresenter;

class CVideoMarker2Dlg;



struct UIConfig
{
	std::vector<std::string> enables;
	std::vector<std::string> disnables;
};


class CUI
{
public:
	CUI();
	CUI(CVideoMarker2Dlg* pDlg,  const UIConfig& config);
	~CUI();


	void RefreshButton();

	CUI& Enable(const std::string& strItemName);
	bool FindID(int& id, const std::string& strItemName);
	CUI& Disable(const std::string& strItemName);

private:
	CUI& Enable(const std::string& strItemName, bool bEnable);

private:
	CVideoMarker2Dlg* m_pDlg;
	std::unordered_map<std::string, int> m_IDByName;
	UIConfig m_Config;
};



class CStateBase
{
public:
	CStateBase(CVideoMarker2Dlg* pDlg);
	CStateBase(CVideoMarker2Dlg* pDlg, const UIConfig& config);

	virtual ~CStateBase();

	virtual void RefreshButton();


	virtual void OpenProject();
	virtual void AddMark();
	virtual void SaveMark();

	virtual void OnLButtonDown(int nFlags, cv::Point point);
	virtual void OnMouseMove(int nFlags, cv::Point point);
	virtual void OnLButtonUp(int nFlags, cv::Point point);

	virtual void SeekTo(int nPos);
	virtual void ForwardOneFrame(int nCurrentFrameIndex);
	virtual void BackOneFrame(int nCurrentFrameIndex);

	virtual void Undo();
	virtual void Redo();


	virtual void Open();
	virtual void Play();
	virtual void Stop();
	virtual void Pause();

	virtual void OpenTextFile();

	virtual bool CanDraw() const;


	


protected:
	void SetState(const std::string& state);
	CWnd* GetDlgItem(int nID);
	bool IsTextFileOpened() const;
	CVideoMarkerPresenter* GetPresenter();

protected:
	CVideoMarker2Dlg* _pDlg;
	CUI m_ui;
	

};