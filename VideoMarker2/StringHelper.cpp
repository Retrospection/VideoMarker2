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
	return{};
}

std::string CStringHelper::ConvertWStringToString(const std::wstring& wstr)
{
	return{};
}

std::wstring CStringHelper::ConvertStringToWString(const std::string& str)
{
	return{};
}
