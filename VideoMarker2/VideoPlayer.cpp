#include "stdafx.h"
#include "VideoPlayer.h"

CVideoPlayer::CVideoPlayer()
{
}

CVideoPlayer::~CVideoPlayer()
{
}

void CVideoPlayer::Init()
{
	m_nTotalFrames = -1;
	m_nFrameWidth = -1;
	m_nFrameHeight = -1;
	m_nFrameRate = -1;
	m_nCurrentFrameIndex = -1;

	return;
}

bool CVideoPlayer::Open(const std::string& strFileName)
{
	m_strFileName = strFileName;
	if (m_VideoCapture.isOpened())
	{
		m_VideoCapture.release();
	}
	m_VideoCapture.open(m_strFileName);
	if (!m_VideoCapture.isOpened())
	{
		Init();
		return false;
	}
	m_nTotalFrames = static_cast<int>(m_VideoCapture.get(CV_CAP_PROP_FRAME_COUNT));
	m_nFrameWidth = static_cast<int>(m_VideoCapture.get(CV_CAP_PROP_FRAME_WIDTH));
	m_nFrameHeight = static_cast<int>(m_VideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT));
	m_nFrameRate = static_cast<int>(m_VideoCapture.get(CV_CAP_PROP_FPS));

	m_dVideoAspectRatio = ((double)m_nFrameWidth) / m_nFrameHeight;
	m_nCurrentFrameIndex = -1;
	return true;
}

bool CVideoPlayer::GetNextFrame(cv::Mat& frame)
{
	if (!m_VideoCapture.isOpened())
	{
		return false;
	}
	if (m_nCurrentFrameIndex >= m_nTotalFrames)
	{
		return false;
	}
	if (!m_VideoCapture.read(frame))
	{
		Init();
		return false;
	}
	++m_nCurrentFrameIndex;
	return true;
}

bool CVideoPlayer::SeekTo(int nPos)
{
	if (!m_VideoCapture.isOpened())
	{
		return false;
	}
	if (nPos < 0 || nPos > m_nTotalFrames)
	{
		return false;
	}
	m_VideoCapture.set(CV_CAP_PROP_POS_FRAMES,nPos);
	m_nCurrentFrameIndex = nPos - 1;
	return true;
}

bool CVideoPlayer::IsOpened() const
{
	return m_VideoCapture.isOpened();
}




