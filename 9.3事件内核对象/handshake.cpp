#include "../../trunk/CmmHdr.h"
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <process.h>
#include "resource.h"

//客户端触发事件，服务器端的线程等待这个事件，以处理客户端请求
HANDLE g_heveRequestedSubmitted;
//服务器端触发事件，客户端线程等待这个事件，以处理服务器端返回的结果
HANDLE g_hevtResultReturned;
//服务器端和客户端共享缓存
TCHAR g_szSharedRequestAndResultBuffer[1024];
//客户端和服务器端约定的特殊字符串，以关闭服务器
TCHAR g_szServerShutDown[] = TEXT("Server ShutDown");
//如果服务器端接收到关闭消息接后，测试对话框是否关闭
HWND g_hMainDlg;

//服务器端线程
DWORD WINAPI ServerThread(PVOID pvParam)
{
	//假定服务器端一直运行
	BOOL fShutdown = FALSE;
	while (!fShutdown)
	{
		//等待客户端提交请求
		WaitForSingleObject(g_heveRequestedSubmitted, INFINITE);
		//判断是否客户端要求服务器端关闭
		fShutdown = (NULL == g_hMainDlg) &&
			(_tcscmp(g_szSharedRequestAndResultBuffer, g_szServerShutDown) == 0);
		//处理客户端请求（反转字符串）
		if (!fShutdown)
		{
			_tcsrev(g_szSharedRequestAndResultBuffer);
		}
		//让客户端处理请求
		SetEvent(g_hevtResultReturned);
	}
	//客户端要求服务器端关闭
	return 0;

}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	chSETDLGICONS(hwnd, IDI_HANDSHAKE);
	//初始化对话框
	Edit_SetText(GetDlgItem(hwnd, IDC_REQUEST), TEXT("Some test data"));
	//保存主对话框句柄
	g_hMainDlg = hwnd;

	return TRUE;
}

void Dlg_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDCANCEL:
		EndDialog(hWnd, id);
		break;
		//提交消息到服务器端
	case IDC_SUBMIT:
		//拷贝请求字符串到共享缓存
		Edit_GetText(GetDlgItem(hWnd, IDC_REQUEST), g_szSharedRequestAndResultBuffer, _countof(g_szSharedRequestAndResultBuffer));
		//让服务器线程知道客户端请求事件
		SetEvent(g_heveRequestedSubmitted);
		//等待服务器处理,返回服务器给个结果
		WaitForSingleObject(g_hevtResultReturned, INFINITE);
		//将结果放入编辑框
		Edit_SetText(GetDlgItem(hWnd, IDC_RESULT), g_szSharedRequestAndResultBuffer);

		break;
	default:
		break;
	}
}

INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
		chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
		chHANDLE_DLGMSG(hWnd, WM_COMMAND, Dlg_OnCommand);
	
	}
	return (FALSE);
}

int WINAPI _tWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR, int)
{
	//创建和初始化2个未重置，自动重置事件
	g_heveRequestedSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hevtResultReturned = CreateEvent(NULL, FALSE, FALSE, NULL);

	//服务器端线程
	DWORD dwThreadID;
	HANDLE hThreadServer = chBEGINTHREADEX(NULL, 0, ServerThread, NULL, 0, &dwThreadID);

	//客户端线程
	DialogBox(hInstanceExe, MAKEINTRESOURCE(IDD_HANDSHAKE), NULL, Dlg_Proc);
	g_hMainDlg = NULL;

	_tcscpy_s(g_szSharedRequestAndResultBuffer, _countof(g_szSharedRequestAndResultBuffer), g_szServerShutDown);
	SetEvent(g_heveRequestedSubmitted);

	//等待服务器端获取关闭线程和等待服务器端完全销毁
	HANDLE h[2];
	h[0] = g_hevtResultReturned;
	h[1] = hThreadServer;
	WaitForMultipleObjects(2, h, TRUE, INFINITE);

	CloseHandle(hThreadServer);
	CloseHandle(g_heveRequestedSubmitted);
	CloseHandle(g_hevtResultReturned);

	return 0;
}