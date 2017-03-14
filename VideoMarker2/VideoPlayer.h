#pragma once


#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <features2d/features2d.hpp>
#include <video/video.hpp>


class CVideoPlayer
{
public:
	CVideoPlayer();
	~CVideoPlayer();

public:
	void Init();
	bool Open(const std::string& strFileName);
	bool GetNextFrame(cv::Mat& frame);
	bool SeekTo(int nPos);
	bool IsOpened()const;


public:
	int m_nFrameWidth;					 // ��Ƶ֡��
	int m_nFrameHeight;                  // ��Ƶ֡��
	int m_nFrameRate;                    // ��Ƶ֡��
	int m_nCurrentFrameIndex;            // ��ǰ֡��
	int m_nTotalFrames;					 // ��Ƶ��֡��
	double m_dVideoAspectRatio;			 // ��Ƶ�Ŀ�߱�
	std::string m_strFileName;			 // Ҫ���ŵ��ļ���

private:
	cv::VideoCapture m_VideoCapture;



};

