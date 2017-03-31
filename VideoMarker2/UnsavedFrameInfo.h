#pragma once

#include "FrameInfo.h"
#include <opencv2/core/core.hpp>

class CUnsavedFrameInfo
{
public:
	CUnsavedFrameInfo();
	~CUnsavedFrameInfo();

	void Add(const FaceInfo& faceInfo);
	void Clear();

	FrameInfo GetUnsavedFrameInfo() const;


private:
	FrameInfo m_FrameInfo;


};

