#pragma once

#include <afx.h>
#include <string>

class CStringHelper
{
public:
	CStringHelper() = delete;
	~CStringHelper() = delete;


	static CString ConvertStringToCString(const std::string& str);
	static CString ConvertWStringToCString(const std::wstring& wstr);
	static std::wstring ConvertCStringToWString(const CString& cstr);
	static std::string ConvertCStringToString(const CString& cstr); 
	static std::string ConvertWStringToString(const std::wstring& wstr);
	static std::wstring ConvertStringToWString(const std::string& str);



};

