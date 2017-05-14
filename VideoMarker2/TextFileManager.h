#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "FrameInfo.h"


class CTextFileManager
{
public:
	CTextFileManager();
	~CTextFileManager();

	bool Open(const std::string& strFileName, size_t nBufferSize = 1500);

	bool IsOpened()const;
	 
	bool GetFrameInfoByPos(FrameInfo& frameInfo, size_t nPos);
	void UpdateFrameInfo(int nPos, const FrameInfo& frameInfo);
	void SaveToTextFile(); 




	


private:
	// This function parse all frameinfo from buffer string.
	// For each string, it calls the ParseFrameInfoFromString() function to 
	// get FrameInfo of one frame and save the FrameInfo into container.
	bool ParseAllFrameInfoFromBuffer();


	// Parse FrameInfo from string.
	// The format of string should like this: 
	// the number of faces in this frame  | space | the bounding box of one face (x y width height) | space | person name | ......
	// the amount of bounding box and person name should be equal to the declaration in the beginning of the string.
	bool ParseFrameInfoFromString(const std::string& buffer);



	// Find the nearest upper bound according to the line number,
	// and than calculate the amount of lines to read.
	size_t CalculateNumbersOfLineToRead(size_t nLineNumber);

	std::vector<std::string> Split(const std::string& str, const std::string& delim);

private:
	std::string m_strTextFileName;
	std::vector<FrameInfo> m_FrameInfos;

	std::fstream m_Ifs;
	std::vector<std::string> m_Buffer;
	size_t m_nBufferSize;

};

