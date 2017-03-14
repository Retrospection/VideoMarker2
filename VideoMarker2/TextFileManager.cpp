#include "stdafx.h"
#include "TextFileManager.h"

#include <fstream>
#include <codecvt>


CTextFileManager::CTextFileManager()
	:m_strTextFileName("")
{
}


CTextFileManager::~CTextFileManager()
{
}



bool CTextFileManager::Open(const std::string& strFileName)
{
	// If the text file has been opened, return true.
	if (m_strTextFileName == strFileName)
	{
		return true;
	}

	// try to open the text file.
	std::ifstream ifs(strFileName);
	if (!ifs.is_open())
	{
		return false;
	}

	// reset the text file name and the container.
	m_strTextFileName = strFileName;
	m_FrameInfos.clear(); 
	return ParseAllFrameInfoFromTextFile(ifs);
}

bool CTextFileManager::GetFrameInfoByPos(FrameInfo& frameInfo, int nPos)
{
	if (m_FrameInfos.empty())
	{
		return false;
	}
	frameInfo = m_FrameInfos[nPos];
	return true;
}

bool CTextFileManager::ParseAllFrameInfoFromTextFile(std::ifstream& ifs)
{
	std::string buffer;
	while (std::getline(ifs, buffer))
	{
		if (!SaveOneFrameInfo(buffer))
		{
			return false;
		}
		buffer.clear();
	}
	return true;
}

bool CTextFileManager::SaveOneFrameInfo(const std::string& buffer)
{
	std::vector<std::string> strFrameInfo = Split(buffer, " ");

	// One line should at least have a number "zero" imply that this frame has no face.
	if (strFrameInfo.empty())
	{
		return false;
	}

	// this frame don't have face.
	int nPersons = atoi(strFrameInfo[0].c_str());
	if (nPersons == 0)
	{
		m_FrameInfos.push_back({});
		return true;
	}

	// have many (>=1) faces.
	FrameInfo frameInfo;
	for (int i = 1; i <= nPersons * 5; i += 5)
	{
		frameInfo.facesInfo.push_back({ strFrameInfo[i + 4],{ atoi(strFrameInfo[i].c_str()), atoi(strFrameInfo[i + 1].c_str()), atoi(strFrameInfo[i + 2].c_str()), atoi(strFrameInfo[i + 3].c_str())}});
	}
	m_FrameInfos.push_back(frameInfo);
	return true;
}

std::vector<std::string> CTextFileManager::Split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> ret;
	std::string::size_type pos1, pos2;
	pos2 = str.find(delim);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		ret.push_back(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = str.find(delim, pos1);
	}
	if (pos1 != str.length())
	{
		ret.push_back(str.substr(pos1));
	}
	return std::move(ret);
}

void CTextFileManager::AddFaceInfo(int nPos, CString& strPersonName, const cv::Point& p1, const cv::Point& p2)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	m_FrameInfos[nPos].facesInfo.push_back({ conv.to_bytes(strPersonName.GetBuffer()), { p1, p2 } });
}

void CTextFileManager::AddFaceInfo(int nPos, CString& strPersonName, const cv::Rect boxes)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	m_FrameInfos[nPos].facesInfo.push_back({ conv.to_bytes(strPersonName.GetBuffer()), boxes });
}

void CTextFileManager::SaveToTextFile()
{
	std::ofstream ofs(m_strTextFileName);
	for (size_t i = 0; i < m_FrameInfos.size() - 1; ++i)
	{
		ofs << m_FrameInfos[i].toString() << std::endl; 
	}
	ofs << m_FrameInfos[m_FrameInfos.size() - 1].toString();
	ofs.close();
}