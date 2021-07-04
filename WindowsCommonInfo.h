#pragma once

#include <string>

//Class for getting general information about the current Windows version
class WindowsCommonInfo
{
public:
	//Print to the console the most complete OS name (windows version, bitness, build number). 
	//For example: "Operating System: Windows 7 x64 (Build 7601), Service Pack 1").
	static void TraceInfo();
public:
	//Get info from Registry
	static std::wstring GetPlatrofmNameFromRegistry();
	//Get info from winAPI
	static std::string GetPlatformName();
};
