#include "stdafx.h"
#include "VideoMarkerPresenter.h"
#include "VideoMarker2Dlg.h"
#include <string>
#include <codecvt>
#include <fstream>

#include "NameInputDialog.h"

#include "constant.h"


bool IsOverlapping(const cv::Rect& rc1, const cv::Rect& rc2)
{
	double intersection = (double)(rc1 & rc2).area();
	double per1 = intersection / rc1.area();
	double per2 = intersection / rc2.area();
	return per1 > 0.5 || per2 > 0.5;
}

CVideoMarkerPresenter::CVideoMarkerPresenter(CVideoMarker2Dlg* pDlg)
	:m_pDlg(pDlg), m_pView(pDlg)
{
	this->m_pVideoPlayer = new CVideoPlayer();
	this->m_pTextMgr = new CTextFileManager();

	m_validator[0] =std::make_pair(BOX_SMALL_ERROR, [](const FaceInfo& info, const FrameInfo&)
	{
		return (info.box.width >= 100 && info.box.height >= 100);
	});

	m_validator[1] = std::make_pair(NAME_EXIST_ERROR, [](const FaceInfo& info, const FrameInfo& existed)
	{
		return std::none_of(existed.facesInfo.begin(), existed.facesInfo.end(), [&](const FaceInfo& faceInfo){ return info.strPersonName == faceInfo.strPersonName; });
	});
	m_validator[2] = std::make_pair(IOU_ERROR, [](const FaceInfo& info, const FrameInfo& existed)
	{
		return std::none_of(existed.facesInfo.begin(), existed.facesInfo.end(), [&](const FaceInfo& faceInfo){ return IsOverlapping(info.box, faceInfo.box); });
	});

	m_validator2[0] = std::make_pair(NAME_EXIST_ERROR, std::bind(&CVideoMarkerPresenter::CheckNameExist, this, std::placeholders::_1));
	m_validator2[1] = std::make_pair(IOU_ERROR, std::bind(&CVideoMarkerPresenter::CheckOverlapping, this, std::placeholders::_1));
}


CVideoMarkerPresenter::~CVideoMarkerPresenter()
{
	delete m_pTextMgr;
	delete m_pVideoPlayer;
}

void CVideoMarkerPresenter::OpenVideo()
{
	std::string strFileName = m_pDlg->GetFileName();
	OpenVideo(strFileName); 
}


void CVideoMarkerPresenter::Close()
{
	if (!m_pVideoPlayer->IsOpened())
	{
		return;
	}
	m_pVideoPlayer->Close();
	m_pDlg->SetFileOpenedStatus(false);
	cv::Mat blackframe(1, 1, CV_8UC3, cv::Scalar(0, 0, 0));
	m_pDlg->SetRawFrame(blackframe);
	m_pDlg->SetCurrentFrameIndex(0);
	m_pDlg->SetTotalFrameCount(0);

	m_pDlg->SetTextFileOpenedStatus(false);
	m_pDlg->SetFrameInfo({});


	m_pDlg->Refresh();
}


bool CVideoMarkerPresenter::OpenVideo(const std::string& strVideoFileName)
{
	bool bResult = m_pVideoPlayer->Open(strVideoFileName);
	if (!bResult)
	{
		return false;
	}
	m_pDlg->SetFileOpenedStatus(bResult);
	cv::Mat frame;
	m_pVideoPlayer->GetNextFrame(frame);
	m_pDlg->SetRawFrame(frame);
	m_pDlg->SetTotalFrameCount(m_pVideoPlayer->m_nTotalFrames);
	m_pDlg->SetCurrentFrameIndex(0);
	m_pDlg->Refresh();
	return true;
}

#include <iostream>
void CVideoMarkerPresenter::Play()
{
	assert(m_pVideoPlayer->IsOpened());

	cv::Mat frame;
	m_pVideoPlayer->GetNextFrame(frame);
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	std::cout << "总帧数为:" << m_pVideoPlayer->m_nTotalFrames << "当前帧号：" << m_pVideoPlayer->m_nCurrentFrameIndex << std::endl;
	m_pDlg->SetRawFrame(frame);
	m_pDlg->SetTotalFrameCount(m_pVideoPlayer->m_nTotalFrames);
	m_pDlg->SetCurrentFrameIndex(m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
	if (m_pVideoPlayer->m_nCurrentFrameIndex + 1 >= m_pVideoPlayer->m_nTotalFrames)
	{
		m_pDlg->OnBnClickedStopButton();
		return;
	}
}


void CVideoMarkerPresenter::Pause()
{
}


void CVideoMarkerPresenter::Stop()
{
	SeekTo(0);
}



int CVideoMarkerPresenter::GetTotalFrameCount() const
{
	return m_pVideoPlayer->m_nTotalFrames;
}

void CVideoMarkerPresenter::SeekTo(int nPos)
{
	if (!m_pVideoPlayer->SeekTo(nPos))
	{
		m_pDlg->MessageBox(L"跳转错误!");
		m_pDlg->Refresh();
		return;
	}
	cv::Mat rawFrame;
	m_pVideoPlayer->GetNextFrame(rawFrame);
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	std::cout << "总帧数为:" << m_pVideoPlayer->m_nTotalFrames << "当前帧号：" << m_pVideoPlayer->m_nCurrentFrameIndex << std::endl;
	m_pDlg->SetCurrentFrameIndex(nPos);
	m_pDlg->SetRawFrame(rawFrame);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
}

void CVideoMarkerPresenter::BackOneFrame(int nCurrentFrameIndex)
{
	SeekTo(nCurrentFrameIndex - 1);
}

void CVideoMarkerPresenter::ForwardOneFrame(int nCurrentFrameIndex)
{
	SeekTo(nCurrentFrameIndex + 1);
}

void CVideoMarkerPresenter::OpenTextFile()
{
	std::string strFileName = m_pDlg->GetTextFileName();
	m_pDlg->SetTextFileOpenedStatus(m_pTextMgr->Open(strFileName));

	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
}

bool CVideoMarkerPresenter::OpenTextFile(const std::string& strTextFileName)
{
	std::string strFileName = strTextFileName;
	bool bResult = m_pTextMgr->Open(strFileName);
	if (!bResult)
	{
		return false;
	}
	m_pDlg->SetTextFileOpenedStatus(bResult);
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
	return true;
}

void CVideoMarkerPresenter::SaveTextFile()
{
	// TODO
}

void CVideoMarkerPresenter::AddMark()
{
	// TODO
}


void CVideoMarkerPresenter::SaveMark()
{
	std::vector<cv::Rect> unsavedBox = m_pDlg->GetUnsavedBox();
	std::vector<std::string> unsavedName = m_pDlg->GetUnsavedName();
	assert(unsavedName.size() == unsavedBox.size());
	if (unsavedBox.empty())
	{
		return;
	}

	FrameInfo newFrameInfo;
//	newFrameInfo.facesInfo.resize(unsavedName.size());
//	std::transform(unsavedBox.begin(), unsavedBox.end(), unsavedName.begin(), newFrameInfo.facesInfo.begin(), [&](const cv::Rect& rc, const std::string& name)->FaceInfo { ValidateFaceInfo({ name, rc }) ? return{ name, rc } : m_pDlg->m_pPictureBox->m_IllegalFaceInfo.push_back({ name, rc }), return{}; });

	for (size_t i = 0; i < unsavedBox.size(); ++i)
	{
		//if (IsValidateFaceInfo({unsavedName[i],unsavedBox[i]}))
		//{
			newFrameInfo.facesInfo.push_back({ unsavedName[i], unsavedBox[i] });
		//}
		//else
		//{
		//	m_pDlg->m_pPictureBox->SetIllegal({ unsavedName[i], unsavedBox[i] }, i);
		//	m_pDlg->m_pPictureBox->DecreaseEndIndex();
		//}
	}
	assert(m_pVideoPlayer->m_nCurrentFrameIndex >= 0);
	size_t frameIndex = static_cast<size_t>(m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pTextMgr->AddFaceInfo(frameIndex, newFrameInfo);
	m_pDlg->ClearUnsavedFrameInfo();
	FrameInfo frameInfo;
	bool result = m_pTextMgr->GetFrameInfoByPos(frameInfo, frameIndex);
	assert(result);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
}

unsigned int CVideoMarkerPresenter::ValidateFaceInfo(const FaceInfo& info)
{
	FrameInfo frameInfo;
	int currentFrameIndex = m_pDlg->GetCurrentFrameIndex();
	bool result = m_pTextMgr->GetFrameInfoByPos(frameInfo, currentFrameIndex);
	assert(result);
	auto pos = std::find_if_not(std::begin(m_validator), std::end(m_validator), [&](const std::pair<int, std::function<bool(const FaceInfo&, const FrameInfo&)>>& p){ return p.second(info, frameInfo); });
	return (pos == std::end(m_validator)) ? VALID : pos->first;
}

#include <set>
unsigned int CVideoMarkerPresenter::ValidateFacesInfo(const std::vector<FaceInfo>& facesInfo)
{
	unsigned int ret = CheckStage1(facesInfo);
	if (ret != VALID)
	{
		return ret;
	}

	return CheckStage2(facesInfo);

}

bool CVideoMarkerPresenter::CheckNameExist(const std::vector<FaceInfo>& facesInfo)
{
	std::set<std::string> personNames;
	for (auto& faceinfo : facesInfo)
	{
		if (!personNames.insert(faceinfo.strPersonName).second)
		{
			return false;
		}
	}
	return true;
}

unsigned int CVideoMarkerPresenter::CheckStage1(const std::vector<FaceInfo>& facesInfo)
{
	for (auto& faceinfo: facesInfo)
	{
		unsigned int result = ValidateFaceInfo(faceinfo);
		if (result != VALID)
		{
			return result;
		}
	}
	return VALID;
}

unsigned int CVideoMarkerPresenter::CheckStage2(const std::vector<FaceInfo>& facesInfo)
{
	auto pos = std::find_if_not(std::begin(m_validator2), std::end(m_validator2), [&](const std::pair<unsigned int, std::function<bool(const std::vector<FaceInfo>&)>>& p){ return p.second(facesInfo); });
	return (pos == std::end(m_validator2)) ? VALID : pos->first;
}

bool CVideoMarkerPresenter::CheckOverlapping(const std::vector<FaceInfo>& facesInfo)
{
	for (size_t i = 0; i < facesInfo.size(); ++i)
	{
		for (size_t j = i + 1; j < facesInfo.size(); ++j)
		{
			if (IsOverlapping(facesInfo[i].box, facesInfo[j].box))
			{
				return false;
			}
		}
	}
	return true;
}

#include <iostream>

int CVideoMarkerPresenter::OpenProject()
{
	std::string strProjectFileName = m_pDlg->GetProjectFileName();
	std::cout << strProjectFileName << std::endl;
	ProjectSetting setting;
	if (!LoadProject(setting, strProjectFileName))
	{
		return LOAD_PROJECT_ERROR;
	}
	if (!OpenVideo(setting.m_strVideoFileName))
	{
		return OPEN_VIDEO_ERROR;
	}
	if (!OpenTextFile(setting.m_strTextFileName))
	{
		return OPEN_TEXTFILE_ERROR;
	}
	return SUCCESS;
}

bool CVideoMarkerPresenter::LoadProject(ProjectSetting& ret, const std::string& strProjectFileName)
{
	std::ifstream ifs(strProjectFileName);
	std::getline(ifs, ret.m_strVideoFileName);
	std::getline(ifs, ret.m_strTextFileName);
	return true;
}

void CVideoMarkerPresenter::Delete()
{
	std::vector<size_t> indexes = m_pView->GetDeleteFrameInfo();
//	std::cout << "presenter's deleteframeinfo is " << info.toString() << std::endl;
	m_pView->ClearDeleteFrameInfo();
	size_t frameIndex = m_pView->GetCurrentFrameIndex();
	m_pTextMgr->DeleteFrameInfo(frameIndex, indexes);
	FrameInfo newInfo;
	bool result = m_pTextMgr->GetFrameInfoByPos(newInfo, frameIndex);
	assert(result);
	m_pView->SetFrameInfo(newInfo);
	m_pView->Refresh();
}

void CVideoMarkerPresenter::UpdateFrameInfo()
{
	FrameInfo frameinfo = m_pDlg->GetFrameInfo();
	std::cout << "从DLG获得的新 FrameInfo 为：" << frameinfo.toString() << std::endl;
	m_pTextMgr->UpdateFrameInfo(m_pDlg->GetCurrentFrameIndex(), frameinfo);
}
