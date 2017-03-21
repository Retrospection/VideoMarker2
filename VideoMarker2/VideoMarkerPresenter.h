#pragma once

#include "VideoMarker2.h"
#include "VideoPlayer.h"
#include "VideoMarker2Dlg.h"
#include "TextFileManager.h"
#include "FrameInfo.h"


#include <functional>

class CVideoMarker2Dlg;

struct ProjectSetting
{
	std::string m_strVideoFileName;
	std::string m_strTextFileName;
};

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

	int OpenProject();

	bool Open(const std::string& strVideoFileName);
	bool OpenTextFile(const std::string& strTextFileName);
	
	bool LoadProject(ProjectSetting& ret, const std::string& strProjectFileName);

public:
	int GetTotalFrameCount() const;

	unsigned int ValidateFaceInfo(const FaceInfo& info);
	unsigned int ValidateFacesInfo(const std::vector<FaceInfo>& facesInfo);
	


private:
	unsigned int CheckStage1(const std::vector<FaceInfo>& facesInfo);
	unsigned int CheckStage2(const std::vector<FaceInfo>& facesInfo);


	bool CheckNameExist(const std::vector<FaceInfo>& facesInfo);
	bool CheckOverlapping(const std::vector<FaceInfo>& facesInfo);

private:
	std::pair<unsigned int,std::function<bool(const FaceInfo&, const FrameInfo& existed)>> m_validator[3];
	std::pair<unsigned int, std::function<bool(const std::vector<FaceInfo>&)>> m_validator2[2];
	//std::function<bool(const std::vector<FaceInfo>&)> m_Functions2[2];






private:
	CVideoPlayer* m_pVideoPlayer;
	CTextFileManager* m_pTextMgr;
	CVideoMarker2Dlg* m_pDlg;
};

