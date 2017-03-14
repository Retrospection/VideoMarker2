#pragma once
#include <string>
#include <vector>

#include "FrameInfo.h"


class CTextFileManager
{
public:
	CTextFileManager();
	~CTextFileManager();

	bool Open(const std::string& strFileName);
	bool GetFrameInfoByPos(FrameInfo& frameInfo, int nPos);
	void AddFaceInfo(int nPos, CString& strPersonName, const cv::Point& p1, const cv::Point& p2);
	void AddFaceInfo(int nPos, CString& strPersonName, const cv::Rect boxes);
	void SaveToTextFile(); 

private:
	// This function take an ifstream, parse it line by line.
	// For each line, it calls the ParseOneFrameInfo() function to 
	// get FrameInfo of one frame and save the FrameInfo into container.
	bool ParseAllFrameInfoFromTextFile(std::ifstream& ifs);


	// Parse FrameInfo from string.
	// The format of string should like this: 
	// the number of faces in this frame  | space | the bounding box of one face (x y width height) | space | person name | ......
	// the amount of bounding box and person name should be equal to the declaration in the beginning of the string.
	bool SaveOneFrameInfo(const std::string& buffer);


	std::vector<std::string> Split(const std::string& str, const std::string& delim);

private:
	std::string m_strTextFileName;
	std::vector<FrameInfo> m_FrameInfos;

};
