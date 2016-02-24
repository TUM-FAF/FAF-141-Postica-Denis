#include <windows.h>

#define IDC_EDIT	    101
#define IDC_OKB	        102
#define IDC_ChC	        103
#define IDC_ChB	        104
#define IDC_ChF	        105
#define IDC_Exit	    106
#define IDC_TextOut	    107
HWND hEdit;
HWND hTextOut;

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=NULL;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=NULL;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=NULL;
	wClass.hIconSm=NULL;
	wClass.hInstance=hInst;
	wClass.lpfnWndProc=(WNDPROC)WinProc;
	wClass.lpszClassName="Window Class";
	wClass.lpszMenuName=NULL;
	wClass.style=CS_HREDRAW|CS_VREDRAW;

	if(!RegisterClassEx(&wClass))
	{
		int nResult=GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\n",
			"Window Class Failed",
			MB_ICONERROR);
	}

	HWND hWnd=CreateWindowEx(NULL,
			"Window Class",
			"Windows application for lab#1",
			WS_OVERLAPPEDWINDOW,
			200,
			200,
			600,
			500,
			NULL,
			NULL,
			hInst,
			NULL);

	if(!hWnd)
	{
		int nResult=GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\n",
			"Window Creation Failed",
			MB_ICONERROR);
	}

    ShowWindow(hWnd,nShowCmd);

	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT paintStruct;
    char string[] = "Done with Pride and Prejudice by Postica Denis";
    HDC hDC;
	switch(msg)
	{
		case WM_CREATE:
		{
			// Create an edit box
			hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				10,
				10,
				395,
				210,
				hWnd,
				(HMENU)IDC_EDIT,
				GetModuleHandle(NULL),
				NULL);
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			SendMessage(hEdit,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Insert text here...");
            hTextOut=CreateWindowEx((DWORD)NULL,
                TEXT("EDIT"),
                TEXT(NULL),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE,
                10,
				250,
				395,
				208,
                hWnd,
                (HMENU) IDC_TextOut,
                GetModuleHandle(NULL),
                NULL);
			HWND hChC=CreateWindowEx(NULL,
				"BUTTON",
				"Change color",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				420,
				250,
				150,
				40,
				hWnd,
				(HMENU)IDC_ChC,
				GetModuleHandle(NULL),
				NULL);
            HWND hChB=CreateWindowEx(NULL,
				"BUTTON",
				"Change background",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				420,
				306,
				150,
				40,
				hWnd,
				(HMENU)IDC_ChB,
				GetModuleHandle(NULL),
				NULL);
            HWND hChF=CreateWindowEx(NULL,
				"BUTTON",
				"Change font",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				420,
				362,
				150,
				40,
				hWnd,
				(HMENU)IDC_ChF,
				GetModuleHandle(NULL),
				NULL);
            HWND hExit=CreateWindowEx(NULL,
				"BUTTON",
				"Exit",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				420,
				418,
				150,
				40,
				hWnd,
				(HMENU)IDC_Exit,
				GetModuleHandle(NULL),
				NULL);
            HWND hOKB=CreateWindowEx(NULL,
				"BUTTON",
				"OK",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				420,
				180,
				150,
				40,
				hWnd,
				(HMENU)IDC_OKB,
				GetModuleHandle(NULL),
				NULL);
			SendMessage(hOKB,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
            SendMessage(hChB,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
            SendMessage(hChC,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
            SendMessage(hChF,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
            SendMessage(hExit,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_OKB:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
				case IDC_ChB:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
				case IDC_ChC:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
				case IDC_ChF:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
				case IDC_Exit:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
			}
			break;
        case WM_PAINT:
			hDC = BeginPaint(hWnd,&paintStruct);
			/*	Set txt color to blue*/
			SetTextColor(hDC, COLORREF(0x00FF0000));
			/*	Display text in middle of window*/
			TextOut(hDC,150,225,string,sizeof(string)-1);
			EndPaint(hWnd, &paintStruct);
			return 0;
			break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}

