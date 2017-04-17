#pragma once

#include <opencv2/core/core.hpp>

#include <vector>

class IVideoMarker2
{
public:
	IVideoMarker2(){}
	
public:
	virtual std::vector<FaceInfo> GetUnsavedFacesInfo() = 0;
	virtual FrameInfo GetFrameInfo() const = 0;
//	virtual void ClearDeleteFrameInfo() = 0;
	virtual int GetCurrentFrameIndex() const = 0;
	virtual void SetFrameInfo(const FrameInfo& frameInfo) = 0;
	virtual void Refresh() = 0;

};