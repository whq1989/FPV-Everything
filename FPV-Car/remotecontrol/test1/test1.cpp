#include <Windows.h>
#include <thread>
#include <stdio.h>
#include <iostream>
#include<winsock.h>

#include "LogitechSteeringWheelLib.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "LogitechSteeringWheelLib.lib")
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


using namespace std;

void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		//cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		//cout << "套接字库版本正确！" << endl;
	}

	
}


void WheelInit(HWND hwnd)
{

	//LogiSteeringInitialize(true);
	
	if (LogiSteeringInitializeWithWindow(true, hwnd))
	{
		bool LogiButtonTriggered(const int index, const int buttonNbr);
		
		printf("init secuss\n");
		//printf("%d\n", hwnd);
		

		HANDLE hCom = CreateFile(L"\\\\\.\\COM6", GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL); //与HC-06建立无线连接
		if (hCom == INVALID_HANDLE_VALUE) {
			std::cout << "Port unavailable!" << std::endl;
		}
		else {
			printf("GO\n");
		}
		//以下配置串口通信参数
		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = 9600;
		dcb.ByteSize = DATABITS_8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		BOOL br = SetCommState(hCom, &dcb);
		//COMMTIMEOUTS cto = { MAXDWORD, MAXDWORD, MAXDWORD, MAXDWORD, MAXDWORD };
		//br = SetCommTimeouts(hCom, &cto);

		
		int i = 0;
		int old = 0;
		SYSTEMTIME sys;	

		while (LogiUpdate() && LogiIsConnected(0))
		{
			//printf("connect secuss\n");
			Sleep(5);
			i++;
			SYSTEMTIME st = { 0 };
			
			DIJOYSTATE2 * wheel = LogiGetState(0);
			int x = wheel->lX;
			int y = wheel->lY;
			int z = wheel->lRz;
			x = round((x + 32767) * 90 / 32767);
			if (x != 0 && x != old) {
				GetLocalTime(&st);
				int sec = st.wSecond;
				int wsec = st.wMilliseconds;
				//输出角度，油门，刹车信息
				printf("Angle = %d   i = %d  old = %d\n", x,i,old);
				printf("%d-%02d-%02d %02d:%02d:%02d:%02d\n",
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond,
					st.wMilliseconds);
				old = x;
				//SendMessage(wheel->lX, wheel->lY, wheel->lRz);
				DWORD dwTransmitted;
				WriteFile(hCom, &x, sizeof(x), &dwTransmitted, NULL);
				

			}
			

			
			
		
		}

	}
	else
	{
		printf("init faild");
	}

}


LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
);

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
)
{
	// 类名  
	TCHAR cls_Name[] = L"My Class";
	// 设计窗口类  
	WNDCLASS wc = { sizeof(WNDCLASS) };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = cls_Name;
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	// 注册窗口类  
	RegisterClass(&wc);

	// 创建窗口
	HWND hwnd = CreateWindow(
		cls_Name,           //类名，要和刚才注册的一致  
		L"方向盘Demo",          //窗口标题文字  
		WS_OVERLAPPEDWINDOW,        //窗口外观样式  
		38,             //窗口相对于父级的X坐标  
		20,             //窗口相对于父级的Y坐标  
		500,                //窗口的宽度  
		500,                //窗口的高度  
		NULL,               //没有父窗口，为NULL  
		NULL,               //没有菜单，为NULL  
		hInstance,          //当前应用程序的实例句柄  
		NULL);              //没有附加数据，为NULL  
	if (hwnd == NULL)                //检查窗口是否创建成功  
		return 0;

	// 显示窗口  
	ShowWindow(hwnd, SW_SHOW);

	// 更新窗口  
	UpdateWindow(hwnd);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //设置窗口最前端
	thread GetWheelData(WheelInit, hwnd);
	GetWheelData.detach();






	//MessageBox(0, "调用了WinMain函数", "测试：", 0);


	// 消息循环  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// 在WinMain后实现  
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
)
{

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		/*PAINTSTRUCT     ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		int length;
		TCHAR buff[1024];
		length = wsprintf(buff, TEXT("the angle is : %d"), x);
		TextOut(hdc, 20, 20, buff, length);
		EndPaint(hwnd, &ps);
		break;*/

	}
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}