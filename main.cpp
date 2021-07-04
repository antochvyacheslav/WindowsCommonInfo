//Сделайте консольную программу для MS Windows, которая будет выводить консоль наиболее полное название ОС (версия windows, битность, номер билда).
//Например: "Operating System: Microsofr Windows 7 x64 (Build 7601), Service Pack 1").
//В задание должны входить файлы проекта для сборки, на выбор Visual Studio(sln) либо CMake.Задание нужно прислать в виде ссылки на GitHub.

#include "WindowsCommonInfo.h"

int main()
{
	WindowsCommonInfo::TraceInfo();
	return 0;
}