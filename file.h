#pragma once
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

//获得文件的大小 返回字节数目
int getFileSize(char *filePath)
{
	struct _stat info;
	_stat(filePath, &info);
	int size = info.st_size;
	cout << "size=" << size;
	return size;
}

//剪切文件 将文件从srcFile转移至destFile
int myMoveFile(char *srcFile, char *destFile)
{
		DWORD getlastError;
		if (!MoveFile(srcFile, destFile))
		{
			
			getlastError = GetLastError();
			printf_s("文件剪切失败 Error: %d\n", getlastError);
			return -1;
		}
		return 0;
}