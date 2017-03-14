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
	int m_nFrameWidth;					 // 视频帧宽
	int m_nFrameHeight;                  // 视频帧高
	int m_nFrameRate;                    // 视频帧率
	int m_nCurrentFrameIndex;            // 当前帧号
	int m_nTotalFrames;					 // 视频总帧数
	double m_dVideoAspectRatio;			 // 视频的宽高比
	std::string m_strFileName;			 // 要播放的文件名

private:
	cv::VideoCapture m_VideoCapture;



};

