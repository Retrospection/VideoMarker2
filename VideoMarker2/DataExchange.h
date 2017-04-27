#pragma once

#include <sstream>
#include "FrameInfo.h"

class DataExchange
{
public:
	DataExchange(FrameInfo* pInfo, CListBox* pBox):m_pInfo(pInfo), m_pBox(pBox){}
	DataExchange(CVideoMarker2Dlg::ListBoxFrameInfo* pInfo, CListBox* pBox) : m_pListBoxInfo(pInfo), m_pBox(pBox){}

public:
	void Update(bool)
	{
		m_pBox->ResetContent();
		for (auto faceInfo : m_pInfo->facesInfo)
		{
			std::wstring str = ConvertFromFrameInfo(faceInfo);
			m_pBox->AddString(str.c_str());
		}

	}

	void Update2(bool)
	{
		m_pBox->ResetContent();
		for (auto faceInfo : m_pListBoxInfo->listBoxFacesInfo)
		{
			std::wstring str = ConvertFromListBoxFaceInfo(faceInfo);
			m_pBox->AddString(str.c_str());
		}
	}

	std::wstring ConvertFromFrameInfo(const FaceInfo& faceInfo)
	{
		std::wstringstream wss;
		wss << faceInfo.strPersonName.c_str() << " ";
		wss << faceInfo.box.x << " " << faceInfo.box.y << " " << faceInfo.box.width << " " << faceInfo.box.height;
		return wss.str();
	}

	std::wstring ConvertFromListBoxFaceInfo(const CVideoMarker2Dlg::ListBoxFaceInfo& faceInfo)
	{
		std::wstringstream wss;
		assert(!(faceInfo.bDeleted && faceInfo.bNew));

		if (faceInfo.bDeleted)
		{
			wss << L"-" << L" ";
		}
		else if (faceInfo.bNew)
		{
			wss << L"+" << L" ";
		}
		wss << faceInfo.faceInfo.strPersonName.c_str() << L" ";
		wss << faceInfo.faceInfo.box.x << L" " << faceInfo.faceInfo.box.y << L" " << faceInfo.faceInfo.box.width << L" " << faceInfo.faceInfo.box.height;
		return wss.str();
	}


private:
	FrameInfo* m_pInfo;
	CVideoMarker2Dlg::ListBoxFrameInfo* m_pListBoxInfo;
	CListBox* m_pBox;
};