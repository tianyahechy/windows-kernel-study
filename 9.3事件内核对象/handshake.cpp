#include "../../trunk/CmmHdr.h"
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <process.h>
#include "resource.h"

//�ͻ��˴����¼����������˵��̵߳ȴ�����¼����Դ���ͻ�������
HANDLE g_heveRequestedSubmitted;
//�������˴����¼����ͻ����̵߳ȴ�����¼����Դ���������˷��صĽ��
HANDLE g_hevtResultReturned;
//�������˺Ϳͻ��˹�����
TCHAR g_szSharedRequestAndResultBuffer[1024];
//�ͻ��˺ͷ�������Լ���������ַ������Թرշ�����
TCHAR g_szServerShutDown[] = TEXT("Server ShutDown");
//����������˽��յ��ر���Ϣ�Ӻ󣬲��ԶԻ����Ƿ�ر�
HWND g_hMainDlg;

//���������߳�
DWORD WINAPI ServerThread(PVOID pvParam)
{
	//�ٶ���������һֱ����
	BOOL fShutdown = FALSE;
	while (!fShutdown)
	{
		//�ȴ��ͻ����ύ����
		WaitForSingleObject(g_heveRequestedSubmitted, INFINITE);
		//�ж��Ƿ�ͻ���Ҫ��������˹ر�
		fShutdown = (NULL == g_hMainDlg) &&
			(_tcscmp(g_szSharedRequestAndResultBuffer, g_szServerShutDown) == 0);
		//����ͻ������󣨷�ת�ַ�����
		if (!fShutdown)
		{
			_tcsrev(g_szSharedRequestAndResultBuffer);
		}
		//�ÿͻ��˴�������
		SetEvent(g_hevtResultReturned);
	}
	//�ͻ���Ҫ��������˹ر�
	return 0;

}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	chSETDLGICONS(hwnd, IDI_HANDSHAKE);
	//��ʼ���Ի���
	Edit_SetText(GetDlgItem(hwnd, IDC_REQUEST), TEXT("Some test data"));
	//�������Ի�����
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
		//�ύ��Ϣ����������
	case IDC_SUBMIT:
		//���������ַ�����������
		Edit_GetText(GetDlgItem(hWnd, IDC_REQUEST), g_szSharedRequestAndResultBuffer, _countof(g_szSharedRequestAndResultBuffer));
		//�÷������߳�֪���ͻ��������¼�
		SetEvent(g_heveRequestedSubmitted);
		//�ȴ�����������,���ط������������
		WaitForSingleObject(g_hevtResultReturned, INFINITE);
		//���������༭��
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
	//�����ͳ�ʼ��2��δ���ã��Զ������¼�
	g_heveRequestedSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hevtResultReturned = CreateEvent(NULL, FALSE, FALSE, NULL);

	//���������߳�
	DWORD dwThreadID;
	HANDLE hThreadServer = chBEGINTHREADEX(NULL, 0, ServerThread, NULL, 0, &dwThreadID);

	//�ͻ����߳�
	DialogBox(hInstanceExe, MAKEINTRESOURCE(IDD_HANDSHAKE), NULL, Dlg_Proc);
	g_hMainDlg = NULL;

	_tcscpy_s(g_szSharedRequestAndResultBuffer, _countof(g_szSharedRequestAndResultBuffer), g_szServerShutDown);
	SetEvent(g_heveRequestedSubmitted);

	//�ȴ��������˻�ȡ�ر��̺߳͵ȴ�����������ȫ����
	HANDLE h[2];
	h[0] = g_hevtResultReturned;
	h[1] = hThreadServer;
	WaitForMultipleObjects(2, h, TRUE, INFINITE);

	CloseHandle(hThreadServer);
	CloseHandle(g_heveRequestedSubmitted);
	CloseHandle(g_hevtResultReturned);

	return 0;
}