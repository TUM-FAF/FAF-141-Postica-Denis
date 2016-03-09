#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "Laboratory Work #2";
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wndclass);

    wndclass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_icon));
    wndclass.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_icon));
    wndclass.hCursor = LoadCursor (NULL, IDC_HAND);
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDM_menu);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;

    //if (!RegisterClassEx (&wndclass))
       //return 0;

    hwnd = CreateWindow(
    szAppName,
    "Laboratory Work #2",
    WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
    CW_USEDEFAULT, CW_USEDEFAULT,
    630, 500,
    HWND_DESKTOP, NULL, hInstance, NULL
    );
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth,
     iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax;
    static HINSTANCE hInstance;
    char szBuffer[10];
    char string[] = "Copyright by Postica Denis";
    char string1[] = "Information";
    HDC hdc;
    int i, x, y, iPaintBeg, iPaintEnd, iVscrollInc, iHscrollInc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    switch(iMsg)
    {
    case WM_CREATE :
        {
            hdc = GetDC(hwnd);
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hwnd, hdc);
            iMaxWidth = 40 * cxChar + 22 * cxCaps;

            HWND hEditBox=CreateWindowEx(
                (DWORD)NULL,
                TEXT("ListBox"),
                NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_DISABLENOSCROLL,
                335, 15, 250, 354, hwnd,
                (HMENU)IDC_edit,
                GetModuleHandle(NULL),
                NULL);

            HWND hElement= CreateWindowEx(
                (DWORD)NULL,
                TEXT("Edit"),
                TEXT(""),
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                15, 15, 250, 350, hwnd,
                (HMENU)IDC_element,
                GetModuleHandle(NULL),
                NULL);

            HWND hwndAdd = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Add"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                15, 400, 100, 40, hwnd,
                (HMENU)IDC_add,
                GetModuleHandle(NULL),
                NULL);

            HWND hwndRmv = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Remove"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                140, 400, 100, 40, hwnd,
                (HMENU)IDC_rmv,
                GetModuleHandle(NULL),
                NULL);

            HWND hInf=CreateWindowEx(
                    0,
                    "BUTTON",
                    NULL,
                    WS_TABSTOP|WS_VISIBLE|
                    WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
                    270,
                    400,
                    100,
                    40,
                    hwnd,
                    (HMENU)IDC_inf,
                    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                    NULL);
            return 0;
        }
        break;
    case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT)
                {
                    SetCursor(LoadCursor (NULL, IDC_HAND));
                    return TRUE;
                }
        }
        break;
    case WM_DRAWITEM:

        {

            switch ((UINT)wParam)

            {
                case IDC_inf:
                {
                    LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                    SIZE size;
                    char text[256];
                    sprintf(text, "%s", "Info");
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
            }
        }
        break;
          case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO MM = (LPMINMAXINFO)lParam;
            MM->ptMinTrackSize.x = 630;
            MM->ptMinTrackSize.y = 500;
            MM->ptMaxTrackSize.x = 1200;
            MM->ptMaxTrackSize.y = 800;
        }
        break;
    case WM_SIZE :
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            iVscrollMax = max(0, 2 + (900 - cyClient) / cyChar);
            iVscrollPos = min(iVscrollPos, iVscrollMax);
            SetScrollRange(hwnd, SB_VERT, 0, iVscrollMax, FALSE);
            SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
            iHscrollMax = max(0, 2 +(iMaxWidth - cxClient) / cxChar);
            iHscrollPos = min(iHscrollPos, iHscrollMax);
            SetScrollRange(hwnd, SB_HORZ, 0, iHscrollMax, FALSE);
            SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
            return 0;
        }
        break;
    case WM_COMMAND:
        {
			switch(LOWORD(wParam))
            {
				case IDC_inf:
				{
					int msgboxID = MessageBox(
                        NULL,
                        string,
                        string1,
                        MB_ICONWARNING | MB_OK | MB_DEFBUTTON2);
				}
				break;
				case ID_ABOUT:
                {
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, AboutDlgProc);
                    break;
                }
                break;
            }
            return 0;
            }
        break;

    case WM_VSCROLL :
        {
            switch(LOWORD(wParam))
            {
                case SB_TOP :
                    iVscrollInc = -iVscrollPos;
                    break;
                case SB_BOTTOM :
                    iVscrollInc = iVscrollMax - iVscrollPos;
                    break;
                case SB_LINEUP :
                    iVscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iVscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iVscrollInc = min(-1, -cyClient / cyChar);
                    break;
                case SB_PAGEDOWN :
                    iVscrollInc = max(1, cyClient / cyChar);
                    break;
                case SB_THUMBTRACK :
                    iVscrollInc = HIWORD(wParam) - iVscrollPos;
                    break;
                default :
                    iVscrollInc = 0;
            }
                iVscrollInc = max(
                -iVscrollPos,
                min(iVscrollInc, iVscrollMax - iVscrollPos)
                );
                if (iVscrollInc != 0)
                {
                    iVscrollPos += iVscrollInc;
                    ScrollWindow(hwnd, 0, -cyChar * iVscrollInc, NULL, NULL);
                    SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
                    UpdateWindow(hwnd);
                }
            return 0;
            }
        break;
    case WM_HSCROLL :
            {switch(LOWORD(wParam))
            {
                case SB_LINEUP :
                    iHscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iHscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iHscrollInc = -8;
                    break;
                case SB_PAGEDOWN :
                    iHscrollInc = 8;
                    break;
                case SB_THUMBPOSITION :
                    iHscrollInc = HIWORD(wParam) - iHscrollPos;
                    break;
                default :
                    iHscrollInc = 0;
            }
            iHscrollInc = max(
            -iHscrollPos,
            min(iHscrollInc, iHscrollMax - iHscrollPos)
            );
            if (iHscrollInc != 0)
            {
                iHscrollPos += iHscrollInc;
                ScrollWindow(hwnd, -cxChar * iHscrollInc, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
            }
            return 0;
        }
        break;

    case WM_HOTKEY:
        {
            switch(wParam)
            {
                case HK_EXIT:
                    PostQuitMessage(0);
                    break;

                case HK_ABOUT:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, AboutDlgProc);
                    break;
                default:
                    break;
            }
        }
         break;
    case WM_DESTROY :
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
    }
    return FALSE;
}
