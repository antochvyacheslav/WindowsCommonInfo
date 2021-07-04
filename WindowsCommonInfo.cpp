//GetVersionExA to work correctly on windows 8.1 and higher, you need to add the manifest file verifyversioninfo.manifest to the project 
//https://docs.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1
//and need to disable warning compiler 
#pragma warning(disable : 4996)

#include "WindowsCommonInfo.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

using namespace std;

void WindowsCommonInfo::TraceInfo()
{
	wprintf(L"Operating System: %ls.\n", GetPlatrofmNameFromRegistry().c_str());
	printf("Operating System: %s.\n", GetPlatformName().c_str());
}

wstring WindowsCommonInfo::GetPlatrofmNameFromRegistry()
{
	TCHAR keyCurrentVersion[256] = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	TCHAR keyEnvironment[256] = _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
	TCHAR keyWindows[256] = _T("SYSTEM\\CurrentControlSet\\Control\\Windows");

	wchar_t str_buf[128] = {};
	DWORD len = sizeof(str_buf);

	wstring Result = L"Microsoft ";

	//Name
	RegGetValueW(HKEY_LOCAL_MACHINE, keyCurrentVersion, L"ProductName", RRF_RT_REG_SZ, nullptr, &str_buf, &len);
	Result += str_buf;

	//Architecture
	//The PROCESSOR_ARCHITECTURE string data will either be AMD64 (meaning 64-bit) or x86 (meaning 32-bit).
	//This is the same for both Intel and AMD hardware. Its OS architecture. A 64-bit processor with Windows x86 installed will report the value x86.
	RegGetValueW(HKEY_LOCAL_MACHINE, keyEnvironment, L"PROCESSOR_ARCHITECTURE", RRF_RT_REG_SZ, nullptr, &str_buf, &len);
	DWORD arch = wcsstr(str_buf, L"64") ? 64 : 86;
	Result += wstring(L" x") + to_wstring(arch);

	//Build
	RegGetValueW(HKEY_LOCAL_MACHINE, keyCurrentVersion, L"CurrentBuild", RRF_RT_REG_SZ, nullptr, &str_buf, &len);
	Result += wstring(L" (Build ") + str_buf + wstring(L")");

	//Service Pack
	RegGetValueW(HKEY_LOCAL_MACHINE, keyWindows, L"CSDVersion", RRF_RT_REG_SZ, nullptr, &str_buf, &len);
	//shift byte to the right 
	DWORD service_pack = _wtoi(str_buf) >> 8;
	if (service_pack)
	{
		Result += wstring(L", Service Pack ") + to_wstring(service_pack);
	}

	return Result;
}

string WindowsCommonInfo::GetPlatformName()
{
	SYSTEM_INFO SystemInfo;
	GetNativeSystemInfo(&SystemInfo);

	OSVERSIONINFOEXA VersionInfo;
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	if (!GetVersionExA((OSVERSIONINFOA*)&VersionInfo))
	{
		return string("Error! GetVersionExA!");
	}

	DWORD ProductType;
	if (!GetProductInfo(VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, 0, 0, &ProductType))
	{
		return string("Error! GetProductInfo!");
	}

	string Result = "Microsoft ";

	if (VER_PLATFORM_WIN32_NT == VersionInfo.dwPlatformId && VersionInfo.dwMajorVersion > 4)
	{
		if (VersionInfo.dwMajorVersion >= 6)
		{
			if (VersionInfo.dwMajorVersion == 6 && VersionInfo.dwMinorVersion == 0)
			{
				if (VersionInfo.wProductType == VER_NT_WORKSTATION)
				{
					Result += "Windows Vista";
				}
				else
				{
					Result += "Windows Server 2008";
				}
			}
			else if (VersionInfo.dwMinorVersion == 0 || VersionInfo.dwMinorVersion == 1 || VersionInfo.dwMinorVersion == 2 || VersionInfo.dwMinorVersion == 3)
			{
				if (VersionInfo.dwMajorVersion == 6)
				{
					if (VersionInfo.wProductType == VER_NT_WORKSTATION && VersionInfo.dwMinorVersion == 1)
					{
						Result += "Windows 7";
					}
					else if (VersionInfo.wProductType == VER_NT_WORKSTATION && VersionInfo.dwMinorVersion == 2)
					{
						Result += "Windows 8";
					}
					else if (VersionInfo.wProductType == VER_NT_WORKSTATION && VersionInfo.dwMinorVersion == 3)
					{
						Result += "Windows 8.1";
					}
					else
					{
						Result += "Windows Server 2008 R2";
					}
				}
				else if (VersionInfo.dwMajorVersion == 10)
				{
					if (VersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						Result += "Windows 10";
					}
					else
					{
						Result += "Windows Server 2016";
					}
				}
				else
				{
					Result += "Unknown Windows Version";
				}

				Result += " ";

				switch (ProductType)
				{
				case PRODUCT_BUSINESS:							Result += "Business"; break;
				case PRODUCT_BUSINESS_N:						Result += "Business N"; break;
				case PRODUCT_CLUSTER_SERVER:					Result += "HPC Edition"; break;
				case PRODUCT_CLUSTER_SERVER_V:					Result += "Server Hyper Core V"; break;
				case PRODUCT_CORE:								Result += "Home"; break;
				case PRODUCT_CORE_COUNTRYSPECIFIC:				Result += "Home China"; break;
				case PRODUCT_CORE_N:							Result += "Home N"; break;
				case PRODUCT_CORE_SINGLELANGUAGE:				Result += "Home Single Language"; break;
				case PRODUCT_DATACENTER_EVALUATION_SERVER:		Result += "Server Datacenter(evaluation installation)"; break;
				case PRODUCT_DATACENTER_SERVER:					Result += "Server Datacenter(full installation)"; break;
				case PRODUCT_DATACENTER_SERVER_CORE:			Result += "Server Datacenter(core installation)"; break;
				case PRODUCT_DATACENTER_SERVER_CORE_V:			Result += "Server Datacenter without Hyper - V(core installation)"; break;
				case PRODUCT_DATACENTER_SERVER_V:				Result += "Server Datacenter without Hyper - V(full installation)"; break;
				case PRODUCT_EDUCATION:							Result += "Education"; break;
				case PRODUCT_EDUCATION_N:						Result += "Education N"; break;
				case PRODUCT_ENTERPRISE:						Result += "Enterprise"; break;
				case PRODUCT_ENTERPRISE_E:						Result += "Enterprise E"; break;
				case PRODUCT_ENTERPRISE_EVALUATION:				Result += "Enterprise Evaluation"; break;
				case PRODUCT_ENTERPRISE_N:						Result += "Enterprise N"; break;
				case PRODUCT_ENTERPRISE_N_EVALUATION:			Result += "Enterprise N Evaluation"; break;
				case PRODUCT_ENTERPRISE_S:						Result += "Enterprise 2015 LTSB"; break;
				case PRODUCT_ENTERPRISE_S_EVALUATION:			Result += "Enterprise 2015 LTSB Evaluation"; break;
				case PRODUCT_ENTERPRISE_S_N:					Result += "Enterprise 2015 LTSB N"; break;
				case PRODUCT_ENTERPRISE_S_N_EVALUATION:			Result += "Enterprise 2015 LTSB N Evaluation"; break;
				case PRODUCT_ENTERPRISE_SERVER:					Result += "Server Enterprise(full installation)"; break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:			Result += "Server Enterprise(core installation)"; break;
				case PRODUCT_ENTERPRISE_SERVER_CORE_V:			Result += "Server Enterprise without Hyper - V(core installation)"; break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:			Result += "Server Enterprise for Itanium - based Systems"; break;
				case PRODUCT_ENTERPRISE_SERVER_V:				Result += "Server Enterprise without Hyper - V(full installation)"; break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:		Result += "Windows Essential Server Solution Additional"; break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:	Result += "Windows Essential Server Solution Additional SVC"; break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT:		Result += "Windows Essential Server Solution Management"; break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:	Result += "Windows Essential Server Solution Management SVC"; break;
				case PRODUCT_HOME_BASIC:						Result += "Home Basic"; break;
				case PRODUCT_HOME_BASIC_E:						Result += "Not supported (Home Basic E)"; break;
				case PRODUCT_HOME_BASIC_N:						Result += "Home Basic N"; break;
				case PRODUCT_HOME_PREMIUM:						Result += "Home Premium"; break;
				case PRODUCT_HOME_PREMIUM_E:					Result += "Not supported (Home Premium E)"; break;
				case PRODUCT_HOME_PREMIUM_N:					Result += "Home Premium N"; break;
				case PRODUCT_HOME_PREMIUM_SERVER:				Result += "Windows Home Server 2011"; break;
				case PRODUCT_HOME_SERVER:						Result += "Windows Storage Server 2008 R2 Essentials"; break;
				case PRODUCT_HYPERV:							Result += "Microsoft Hyper - V Server"; break;
				case PRODUCT_IOTUAP:							Result += "IoT Core"; break;
					//case PRODUCT_IOTUAPCOMMERCIAL:					Result += "IoT Core Commercial"; break;
				case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:	Result += "Windows Essential Business Server Management Server"; break;
				case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:	Result += "Windows Essential Business Server Messaging Server"; break;
				case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:	Result += "Windows Essential Business Server Security Server"; break;
					//case PRODUCT_MOBILE_CORE:						Result += "Mobile"; break;
					//case PRODUCT_MOBILE_ENTERPRISE:					Result += "Mobile Enterprise"; break;
				case PRODUCT_MULTIPOINT_PREMIUM_SERVER:			Result += "Windows MultiPoint Server Premium(full installation)"; break;
				case PRODUCT_MULTIPOINT_STANDARD_SERVER:		Result += "Windows MultiPoint Server Standard(full installation)"; break;
				case PRODUCT_PROFESSIONAL:						Result += "Pro"; break;
				case PRODUCT_PROFESSIONAL_E:					Result += "Not supported (Pro E)"; break;
				case PRODUCT_PROFESSIONAL_N:					Result += "Pro N"; break;
				case PRODUCT_PROFESSIONAL_WMC:					Result += "Professional with Media Center"; break;
				case PRODUCT_SB_SOLUTION_SERVER:				Result += "Windows Small Business Server 2011 Essentials"; break;
				case PRODUCT_SB_SOLUTION_SERVER_EM:				Result += "Server For SB Solutions EM"; break;
				case PRODUCT_SERVER_FOR_SB_SOLUTIONS:			Result += "Server For SB Solutions"; break;
				case PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM:		Result += "Server For SB Solutions EM"; break;
				case PRODUCT_SERVER_FOR_SMALLBUSINESS:			Result += "Windows Server 2008 for Windows Essential Server Solutions"; break;
				case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:		Result += "Windows Server 2008 without Hyper - V for Windows Essential Server Solutions"; break;
				case PRODUCT_SERVER_FOUNDATION:					Result += "Server Foundation"; break;
				case PRODUCT_SMALLBUSINESS_SERVER:				Result += "Windows Small Business Server"; break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:		Result += "Small Business Server Premium"; break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE:	Result += "Small Business Server Premium(core installation)"; break;
				case PRODUCT_SOLUTION_EMBEDDEDSERVER:			Result += "Windows MultiPoint Server"; break;
				case PRODUCT_STANDARD_EVALUATION_SERVER:		Result += "Server Standard(evaluation installation)"; break;
				case PRODUCT_STANDARD_SERVER:					Result += "Server Standard"; break;
				case PRODUCT_STANDARD_SERVER_CORE:				Result += "Server Standard(core installation)"; break;
				case PRODUCT_STANDARD_SERVER_CORE_V:			Result += "Server Standard without Hyper - V(core installation)"; break;
				case PRODUCT_STANDARD_SERVER_V:					Result += "Server Standard without Hyper - V"; break;
				case PRODUCT_STANDARD_SERVER_SOLUTIONS:			Result += "Server Solutions Premium"; break;
				case PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE:	Result += "Server Solutions Premium(core installation)"; break;
				case PRODUCT_STARTER:							Result += "Starter"; break;
				case PRODUCT_STARTER_E:							Result += "Not supported (Starter E)"; break;
				case PRODUCT_STARTER_N:							Result += "Starter N"; break;
				case PRODUCT_STORAGE_ENTERPRISE_SERVER:			Result += "Storage Server Enterprise"; break;
				case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:	Result += "Storage Server Enterprise(core installation)"; break;
				case PRODUCT_STORAGE_EXPRESS_SERVER:			Result += "Storage Server Express"; break;
				case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:		Result += "Storage Server Express(core installation)"; break;
				case PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER:Result += "Storage Server Standard(evaluation installation)"; break;
				case PRODUCT_STORAGE_STANDARD_SERVER:			Result += "Storage Server Standard"; break;
				case PRODUCT_STORAGE_STANDARD_SERVER_CORE:		Result += "Storage Server Standard(core installation)"; break;
				case PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER:Result += "Storage Server Workgroup(evaluation installation)"; break;
				case PRODUCT_STORAGE_WORKGROUP_SERVER:			Result += "Storage Server Workgroup"; break;
				case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:		Result += "Storage Server Workgroup(core installation)"; break;
				case PRODUCT_ULTIMATE:							Result += "Ultimate"; break;
				case PRODUCT_ULTIMATE_E:						Result += "Not supported (Ultimate E)"; break;
				case PRODUCT_ULTIMATE_N:						Result += "Ultimate N"; break;
				case PRODUCT_UNDEFINED:							Result += "An unknown product"; break;
				case PRODUCT_WEB_SERVER:						Result += "Web Server(full installation)"; break;
				case PRODUCT_WEB_SERVER_CORE:					Result += "Web Server(core installation)"; break;
				default:										Result += "Unknown product type"; break;
				}
			}
		}
		else if (VersionInfo.dwMajorVersion == 5 && VersionInfo.dwMinorVersion == 2)
		{
			if (GetSystemMetrics(SM_SERVERR2))
			{
				Result += "Windows Server 2003 R2";
			}
			else if (VersionInfo.wSuiteMask & VER_SUITE_STORAGE_SERVER)
			{
				Result += "Windows Storage Server 2003";
			}
			else if (VersionInfo.wSuiteMask & VER_SUITE_WH_SERVER)
			{
				Result += "Windows Home Server";
			}
			else if (VersionInfo.wProductType & VER_NT_WORKSTATION && SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			{
				Result += "Windows XP Professional x64 Edition";
			}
			else
			{
				Result += "Windows Server 2003";
			}
		}
		else if (VersionInfo.dwMajorVersion == 5 && VersionInfo.dwMinorVersion == 1)
		{
			Result += "Windows XP";
			if (VersionInfo.wSuiteMask & VER_SUITE_PERSONAL)
			{
				Result += " Home Edition";
			}
			else
			{
				Result += " Professional";
			}
		}
		else if (VersionInfo.dwMajorVersion == 5 && VersionInfo.dwMinorVersion == 0)
		{
			Result += "Windows 2000 ";

			if (VersionInfo.wProductType == VER_NT_WORKSTATION)
			{
				Result += " Professional";
			}
			else
			{
				if (VersionInfo.wSuiteMask & VER_SUITE_DATACENTER)
				{
					Result += " Datacenter Server";
				}
				else if (VersionInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				{
					Result += " Advanced Server";
				}
				else
				{
					Result += " Server";
				}
			}
		}

		if (VersionInfo.dwMajorVersion >= 6)
		{
			if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				Result += " x64";
			}
			else if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			{
				Result += " x32";
			}
		}

		Result += string(" (Build ") + to_string((int)VersionInfo.dwBuildNumber) + string(")");

		if (strlen(VersionInfo.szCSDVersion) > 0)
		{
			Result += string(", Service Pack ") + to_string(atoi(VersionInfo.szCSDVersion) >> 8);
		}
	}

	return Result;
}
