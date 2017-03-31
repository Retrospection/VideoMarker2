#include "stdafx.h"
#include "UnsavedFrameInfo.h"


CUnsavedFrameInfo::CUnsavedFrameInfo()
{
}


CUnsavedFrameInfo::~CUnsavedFrameInfo()
{
}

void CUnsavedFrameInfo::Add(const FaceInfo& faceInfo)
{
	m_FrameInfo.facesInfo.push_back(faceInfo);
}

void CUnsavedFrameInfo::Clear()
{
	m_FrameInfo.facesInfo.clear();
}

FrameInfo CUnsavedFrameInfo::GetUnsavedFrameInfo() const
{
	return m_FrameInfo;
}
