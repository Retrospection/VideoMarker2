#include "stdafx.h"
#include "VideoMarkerPresenter.h"

#include <string>
#include <codecvt>

#include "NameInputDialog.h"



CVideoMarkerPresenter::CVideoMarkerPresenter(CVideoMarker2Dlg* pDlg)
	:m_pDlg(pDlg)
{
	this->m_pVideoPlayer = new CVideoPlayer();
	this->m_pTextMgr = new CTextFileManager();
}


CVideoMarkerPresenter::~CVideoMarkerPresenter()
{
	delete m_pTextMgr;
	delete m_pVideoPlayer;
}

void CVideoMarkerPresenter::Open()
{
	std::string strFileName = m_pDlg->GetFileName();
	m_pDlg->SetFileOpenedStatus(m_pVideoPlayer->Open(strFileName));
	cv::Mat frame;
	m_pVideoPlayer->GetNextFrame(frame);
	m_pDlg->SetRawFrame(frame);
	m_pDlg->SetTotalFrameCount(m_pVideoPlayer->m_nTotalFrames);
	m_pDlg->SetCurrentFrameIndex(0);
	m_pDlg->Refresh();
}

void CVideoMarkerPresenter::Play()
{
	assert(m_pVideoPlayer->IsOpened());
	cv::Mat frame;
	m_pVideoPlayer->GetNextFrame(frame);
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetRawFrame(frame);
	m_pDlg->SetTotalFrameCount(m_pVideoPlayer->m_nTotalFrames);
	m_pDlg->SetCurrentFrameIndex(m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
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
		m_pDlg->MessageBox(L"Ìø×ª´íÎó!");
		m_pDlg->Refresh();
		return;
	}
	cv::Mat rawFrame;
	m_pVideoPlayer->GetNextFrame(rawFrame);
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
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
	m_pTextMgr->GetFrameInfoByPos(frameInfo,m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
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
		if (IsValidateFaceInfo({unsavedName[i],unsavedBox[i]}))
		{
			newFrameInfo.facesInfo.push_back({ unsavedName[i], unsavedBox[i] });
		}
		else
		{
			m_pDlg->m_pPictureBox->SetIllegal({ unsavedName[i], unsavedBox[i] }, i);
			m_pDlg->m_pPictureBox->DecreaseEndIndex();
		}
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

bool CVideoMarkerPresenter::IsValidateFaceInfo(const FaceInfo& faceInfo)
{
	if (faceInfo.box.width < 100 || faceInfo.box.height < 100)
	{
		return false;
	}
	return true;
}
