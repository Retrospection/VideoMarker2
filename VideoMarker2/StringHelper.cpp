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
