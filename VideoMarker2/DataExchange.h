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

		//if (m_AddPersonName.empty())
		//{
		//	return;
		//}
		//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		//for (size_t i = 0; i < m_AddPersonName.size(); ++i)
		//{
		//	m_pBox->AddString(ConvertFromFrameInfo({ conv.to_bytes(m_AddPersonName[i].GetBuffer()), m_pPictureBox->m_boxes[i] }).c_str());
		//}

	}

	std::wstring ConvertFromFrameInfo(const FaceInfo& faceInfo)
	{
		std::wstringstream wss;
		wss << faceInfo.strPersonName.c_str() << " ";
		wss << faceInfo.box.height << " " << faceInfo.box.width << " " << faceInfo.box.y << " " << faceInfo.box.x;
		// 	wss << faceInfo.box.x << " " << faceInfo.box.y << " " << faceInfo.box.width << " " << faceInfo.box.height << " ";
		// 	wss << faceInfo.strPersonName.c_str();
		return wss.str();
	}


private:
	FrameInfo* m_pInfo;
	CListBox* m_pBox;
};