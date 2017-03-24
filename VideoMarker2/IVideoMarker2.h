#pragma once

#include <opencv2/core/core.hpp>

#include <vector>

class IVideoMarker2
{
public:
	IVideoMarker2(){}
	
public:
	virtual std::vector<cv::Rect> GetUnsavedBox() = 0;
	virtual std::vector<std::string> GetUnsavedName() = 0;

	virtual FrameInfo GetDeleteFrameInfo() = 0;
	virtual void ClearDeleteFrameInfo() = 0;


	virtual int GetCurrentFrameIndex() const = 0;

	virtual void SetFrameInfo(const FrameInfo& frameInfo) = 0;

	virtual void Refresh() = 0;

};