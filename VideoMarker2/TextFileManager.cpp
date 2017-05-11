#include "stdafx.h"
#include "TextFileManager.h"

#include <fstream>
#include <codecvt>
#include <iostream>


CTextFileManager::CTextFileManager()
	:m_strTextFileName("")
{
}


CTextFileManager::~CTextFileManager()
{
}


bool CTextFileManager::Open(const std::string& strFileName, size_t nBufferSize)
{
	m_Ifs.open(strFileName);
	if (!m_Ifs.is_open())
	{
		return false;
	}

	m_nBufferSize = nBufferSize;
	m_strTextFileName = strFileName;

	m_FrameInfos.clear();
	m_Buffer.clear();

	for (size_t i = 0; i < m_nBufferSize; ++i)
	{
		std::string buf;
		if (!std::getline(m_Ifs,buf))
		{
			break;
		}
		m_Buffer.push_back(buf);
		buf.clear();
	}

	ParseAllFrameInfoFromBuffer();
	return true;
}

bool CTextFileManager::IsOpened() const
{
	return m_Ifs.is_open();
}


bool CTextFileManager::GetFrameInfoByPos(FrameInfo& frameInfo, size_t nPos)
{
	if (nPos > m_FrameInfos.size())
	{
		size_t nLinesToBeRead = CalculateNumbersOfLineToRead(nPos);
		std::string buf;
		for (size_t i = 0; i < nLinesToBeRead; ++i)
		{
			if (!std::getline(m_Ifs, buf))
			{
				break;
			}
			m_Buffer.push_back(buf);
			buf.clear();
		}
		ParseAllFrameInfoFromBuffer();
	}
	if (nPos >= m_FrameInfos.size())
	{
		return false;
	}
	frameInfo = m_FrameInfos[nPos];
	return true;
}


void CTextFileManager::UpdateFrameInfo(int nPos, const FrameInfo& frameInfo)
{
	std::cout << "TextMgr 要更新的 FrameInfo 为：" << frameInfo.toString() << std::endl;
	m_FrameInfos[nPos] = frameInfo;
	SaveToTextFile();
}



// TODO : 在保存之前，完全载入文件，然后从头输出FrameInfo信息。
void CTextFileManager::SaveToTextFile()
{
	std::string buf;
	while (std::getline(m_Ifs, buf))
	{
		m_Buffer.push_back(buf);
	}
	ParseAllFrameInfoFromBuffer();

	std::ofstream ofs(m_strTextFileName);
	for (size_t i = 0; i < m_FrameInfos.size() - 1; ++i)
	{
		ofs << m_FrameInfos[i].toString() << std::endl;
	}
	ofs << m_FrameInfos[m_FrameInfos.size() - 1].toString();
	ofs.close();
}


bool CTextFileManager::ParseAllFrameInfoFromBuffer()
{
	for (auto str : m_Buffer)
	{
		if (!ParseFrameInfoFromString(str))
		{
			return false;
		}
	}
	m_Buffer.clear();
	return true;
}



bool CTextFileManager::ParseFrameInfoFromString(const std::string& buffer)
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
		frameInfo.facesInfo.push_back({ strFrameInfo[i + 4],*(reinterpret_cast<RectEx*>(&cv::Rect{ atoi(strFrameInfo[i].c_str()), atoi(strFrameInfo[i + 1].c_str()), atoi(strFrameInfo[i + 2].c_str()), atoi(strFrameInfo[i + 3].c_str())}))});
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






size_t CTextFileManager::CalculateNumbersOfLineToRead(size_t nLineNumber)
{
	return (((nLineNumber / m_nBufferSize) + 1) * m_nBufferSize) - m_FrameInfos.size();
}


