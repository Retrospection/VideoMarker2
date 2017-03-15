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
	CString cstrFileName = m_pDlg->GetFileName();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wstrFileName(cstrFileName.GetBuffer()); 
	std::string strFileName = conv.to_bytes(wstrFileName);
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
	CString cstrFileName = m_pDlg->GetTextFileName();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wstrFileName(cstrFileName.GetBuffer());
	std::string strFileName = conv.to_bytes(wstrFileName);
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
	if (m_pDlg->m_pPictureBox->m_boxes.empty())
	{
		return;
	}

	std::vector<cv::Rect> unsavedBox = m_pDlg->GetUnsavedBox();

	for (size_t i = 0; i < unsavedBox.size(); ++i)
	{
		m_pTextMgr->AddFaceInfo(m_pVideoPlayer->m_nCurrentFrameIndex, m_pDlg->m_AddPersonName[i], unsavedBox[i]);
	}
	m_pTextMgr->SaveToTextFile();
	m_pDlg->m_AddPersonName.clear();
	m_pDlg->m_pPictureBox->m_boxes.clear();
	FrameInfo frameInfo;
	m_pTextMgr->GetFrameInfoByPos(frameInfo, m_pVideoPlayer->m_nCurrentFrameIndex);
	m_pDlg->SetFrameInfo(frameInfo);
	m_pDlg->Refresh();
}
