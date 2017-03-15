#pragma once

#include <sstream>
#include "FrameInfo.h"

class DataExchange
{
public:
	DataExchange(FrameInfo* pInfo, CListBox* pBox):m_pInfo(pInfo), m_pBox(pBox){}

public:
	void Update(bool)
	{
		m_pBox->ResetContent();
		for (auto faceInfo : m_pInfo->facesInfo)
		{
			m_pBox->AddString(ConvertFromFrameInfo(faceInfo).c_str());
		}

	}

	std::wstring ConvertFromFrameInfo(const FaceInfo& faceInfo)
	{
		std::wstringstream wss;
		wss << faceInfo.strPersonName.c_str() << " ";
		wss << faceInfo.box.height << " " << faceInfo.box.width << " " << faceInfo.box.y << " " << faceInfo.box.x;
		return wss.str();
	}


private:
	FrameInfo* m_pInfo;
	CListBox* m_pBox;
};