#include <windows.h>
#include <stdio.h>

#define IDC_EDIT	    101
#define IDC_OKB	        102
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
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
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
    static char szSubmitBuffer[512];
    const char * szMove = "MOVE";
    char string[] = "Done with Pride and Prejudice by Postica Denis";
    HDC hDC;
    HFONT hfont;
    RECT rcWindow, rcClient;
    int iScreenW, iScreenH, iClientWidth, iClientHeight;
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
			HWND hChB=CreateWindowEx(0,
				"BUTTON",
				NULL,
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
				420,
				250,
				150,
				40,
				hWnd,
				(HMENU)IDC_ChB,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
            HWND hChF=CreateWindowEx(0,
				"BUTTON",
				NULL,
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
				420,
				306,
				150,
				40,
				hWnd,
				(HMENU)IDC_ChF,
				GetModuleHandle(NULL),
				NULL);
            HWND hExit=CreateWindowEx(0,
				"BUTTON",
				NULL,
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
				420,
				418,
				150,
				40,
				hWnd,
				(HMENU)IDC_Exit,
				GetModuleHandle(NULL),
				NULL);
            HWND hOKB=CreateWindowEx(0,
				"BUTTON",
				NULL,
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
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
		case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
				case SC_CLOSE:
					return MessageBox(NULL,
						TEXT("Can't perform action "),
						TEXT("Close"),
						MB_OK | MB_ICONASTERISK);
					break;
				default:
					return DefWindowProc(hWnd, msg, wParam, lParam);
			}
		}
        case WM_DRAWITEM:

        {

            switch ((UINT)wParam)

            {

                case IDC_Exit:

                {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char text[256];
                    sprintf(text, "%s", "Exit");
                    GetTextExtentPoint32(lpdis->hDC, text, strlen(text), &size);
                    SetTextColor(lpdis->hDC, RGB(255, 255, 255));
                    SetBkColor(lpdis->hDC, RGB(255, 0, 0));
                    ExtTextOut(lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem, text, strlen(text), NULL);
                    DrawEdge(lpdis->hDC, &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ?
                    EDGE_SUNKEN : EDGE_RAISED ), BF_RECT);
                    return TRUE;
                }
                break;
                case IDC_ChB:

                {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char text[256];
                    sprintf(text, "%s", "Calibri");
                    GetTextExtentPoint32(lpdis->hDC, text, strlen(text), &size);
                    SetTextColor(lpdis->hDC, RGB(255, 255, 255));
                    SetBkColor(lpdis->hDC, RGB(176,196,255));
                    ExtTextOut(lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem, text, strlen(text), NULL);
                    DrawEdge(lpdis->hDC, &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ?
                    EDGE_SUNKEN : EDGE_RAISED ), BF_RECT);
                    return TRUE;
                }
                break;
                case IDC_ChF:

                {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char text[256];
                    sprintf(text, "%s", "Courier New");
                    GetTextExtentPoint32(lpdis->hDC, text, strlen(text), &size);
                    SetTextColor(lpdis->hDC, RGB(255, 255, 255));
                    SetBkColor(lpdis->hDC, RGB(0,100,0));
                    ExtTextOut(lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem, text, strlen(text), NULL);
                    DrawEdge(lpdis->hDC, &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ?
                    EDGE_SUNKEN : EDGE_RAISED ), BF_RECT);
                    return TRUE;
                }
                break;
                case IDC_OKB:

                {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char text[256];
                    sprintf(text, "%s", "OK");
                    GetTextExtentPoint32(lpdis->hDC, text, strlen(text), &size);
                    SetTextColor(lpdis->hDC, RGB(255, 255, 255));
                    SetBkColor(lpdis->hDC, RGB(199,21,133));
                    ExtTextOut(lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem, text, strlen(text), NULL);
                    DrawEdge(lpdis->hDC, &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ?
                    EDGE_SUNKEN : EDGE_RAISED ), BF_RECT);
                    return TRUE;
                }
                break;
            }
        }
        break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_OKB:
				{
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(szSubmitBuffer)/sizeof(szSubmitBuffer[0]),
						reinterpret_cast<LPARAM>(szSubmitBuffer));

					if (MessageBox(NULL,
							TEXT("Accept text?"),
							TEXT("Submission"),
							MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING) == IDYES) {
						SendMessage(hTextOut, WM_SETTEXT, 0, (LPARAM)szSubmitBuffer);
					};

					if ( strcmp(szMove, szSubmitBuffer) == 0 )
					{
						ShowWindow(hWnd, SW_MAXIMIZE);
					}
				}
				break;
				case IDC_ChB:
				{
					hfont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Calibri"));
                        SendMessage(hTextOut, WM_SETFONT, (WPARAM)hfont, 1);
				}
				break;
				case IDC_ChF:
				{
					hfont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Courier New"));
                        SendMessage(hTextOut, WM_SETFONT, (WPARAM)hfont, 1);
				}
				break;
				case IDC_Exit:
				{
					if (MessageBox(NULL, TEXT("Are you sure you want to exit?"), TEXT("Exit?"),MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING) == IDYES) {
                    SendMessage(hWnd, WM_DESTROY, 0, 0);

                    };
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

