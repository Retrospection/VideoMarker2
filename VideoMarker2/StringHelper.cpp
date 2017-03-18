#include "stdafx.h"
#include "StringHelper.h"

#include <codecvt>


CString CStringHelper::ConvertStringToCString(const std::string& str)
{
	return{};
}

CString CStringHelper::ConvertWStringToCString(const std::wstring& wstr)
{
	return{}; 
}

std::wstring CStringHelper::ConvertCStringToWString(const CString& cstr)
{
	return{};
}

std::string CStringHelper::ConvertCStringToString(const CString& cstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	CString str = cstr;
	std::wstring wstr(str.GetBuffer());
	return conv.to_bytes(wstr);
}

std::string CStringHelper::ConvertWStringToString(const std::wstring& wstr)
{
	return{};
}

std::wstring CStringHelper::ConvertStringToWString(const std::string& str)
{
	return{};
}

std::vector<std::string> CStringHelper::Split(const std::string& str, const std::string& delim)
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
