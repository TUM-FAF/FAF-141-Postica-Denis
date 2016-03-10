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
int iMinW  = 600;
int iMinH = 500;
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
int WINAPI WinMain (HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;

    wndclass.hInstance = hInstance;
    wndclass.lpszClassName = szClassName;
    wndclass.lpfnWndProc = WndProc;
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndclass.cbSize = sizeof (WNDCLASSEX);

    wndclass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_icon));
    wndclass.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_icon));
    wndclass.hCursor = LoadCursor (NULL, IDC_HAND);
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDM_menu);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;

    wndclass.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(173,216,230));

    if (!RegisterClassEx (&wndclass))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           _T("Laboratory Work #2"),
           WS_OVERLAPPEDWINDOW ,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           iMinW,
           iMinH,
           HWND_DESKTOP,
           NULL,
           hInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage (&msg, NULL, 0, 0))
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
    SCROLLINFO si;
    RECT rcWindow, rcClient,rect;
    static HWND hChild[9];
    int  iClientWidth, iClientHeight, iWListBox, iHListBox,iWinWidth,iWinHeight;
    int i, x, y, iPaintBeg, iPaintEnd, iVscrollInc, iHscrollInc,iSysWidth, iSysHeight;
    static int xPos, xMin, xMax;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    char* szText;
    int iTextLength;
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

            hChild[0]=CreateWindowEx(
                (DWORD)NULL,
                TEXT("ListBox"),
                NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_DISABLENOSCROLL,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDC_edit,
                GetModuleHandle(NULL),
                NULL);

            hChild[1]= CreateWindowEx(
                (DWORD)NULL,
                TEXT("Edit"),
                TEXT(""),
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDC_element,
                GetModuleHandle(NULL),
                NULL);

            hChild[2] = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Add"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDC_add,
                GetModuleHandle(NULL),
                NULL);

            hChild[3] = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Remove"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDC_rmv,
                GetModuleHandle(NULL),
                NULL);

            hChild[4]=CreateWindowEx(
                    0,
                    "BUTTON",
                    NULL,
                    WS_TABSTOP|WS_VISIBLE|
                    WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
                    0,
                    0,
                    0,
                    0,
                    hwnd,
                    (HMENU)IDC_inf,
                    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                    NULL);

            hChild[5] = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Width"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDM_label_1,
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

            hChild[6]= CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Height"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)IDM_label_2,
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

            hChild[7] = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)ID_wscroll,
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);
            SetScrollRange(hChild[6], SB_CTL, 0, 100, TRUE);

            hChild[8] = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0,
                0,
                0,
                0,
                hwnd,
                (HMENU)ID_hscroll,
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);
            SetScrollRange(hChild[7], SB_CTL, 0, 100, TRUE);

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
            MM->ptMinTrackSize.x = 410;
            MM->ptMinTrackSize.y = 250;
            MM->ptMaxTrackSize.x = 1200;
            MM->ptMaxTrackSize.y = 800;
        }
        break;
        case WM_SIZE :
        {
            cxClient  = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            iWListBox = cxClient/2 - 50;
            iHListBox = cyClient - 150;

            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = ((iMinH - 150) / cyChar);
            si.nPage = cyClient / cyChar;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = ((iMinW - 100) / cxChar);
            si.nPage = cxClient / cxChar;
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

            MoveWindow(hChild[0], iWListBox+90, 20, iWListBox, iHListBox, TRUE);
            MoveWindow(hChild[1], 20, 20, iWListBox, iHListBox, TRUE);
            MoveWindow(hChild[2], 20, iHListBox+40, iWListBox/5*2, 40, TRUE);

            MoveWindow(hChild[3], iWListBox/5*2+40, iHListBox+40, iWListBox/5*2, 40, TRUE);
            MoveWindow(hChild[4], 2*iWListBox/5*2+60, iHListBox+40, iWListBox/5*2, 40, TRUE);

            MoveWindow(hChild[5], 20, iHListBox+90,  cxClient/3, 20, TRUE);
            MoveWindow(hChild[6], cxClient/2-20, iHListBox+90,  cxClient/3, 20, TRUE);

            MoveWindow(hChild[7], 20, iHListBox+120,  cxClient/3, 20, TRUE);
            MoveWindow(hChild[8], cxClient/2-20, iHListBox+120,  cxClient/3, 20, TRUE);

            GetWindowRect(hwnd, &rect);
            iWinWidth = rect.right - rect.left;
            iWinHeight = rect.bottom - rect.top;
            iSysWidth = GetSystemMetrics(SM_CXSCREEN);
            iSysHeight = GetSystemMetrics(SM_CYSCREEN);

            xPos = 0;
            xMin = 0;
            xMax = 255;
            SetScrollPos(hChild[6], SB_CTL, (iWinWidth * 100 / iSysWidth), TRUE);
            SetScrollPos(hChild[7], SB_CTL, (iWinHeight * 100 / iSysHeight), TRUE);
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
                case IDC_edit:
                {
                    if(HIWORD(wParam) == LBN_DBLCLK)
                    {
                        int index = SendMessage(GetDlgItem(hwnd, IDC_edit), LB_GETCURSEL, 0, 0);
                        int textLength = SendMessage(hChild[0], LB_GETTEXTLEN, (WPARAM)index, 0);
                        TCHAR * buffer = new TCHAR[textLength + 1];
                        buffer[textLength + 1] = _T('\0');
                        SendMessage(hChild[0], LB_GETTEXT, (WPARAM)index, (LPARAM)buffer);
                        char buffer1[50] = "You've selected \0";
                        char * buffer2 = new char[textLength + 1 + strlen(buffer1) + 20];
                        buffer2[0] = '\0';
                        strcat(buffer2, buffer1); strcat(buffer2, buffer);
                        MessageBox(NULL, buffer2, "About", MB_OK);
                        delete [] buffer;
                        delete [] buffer2;
                    }
                    break;
                }

                case IDC_add:
                    iTextLength = SendMessage(hChild[2], WM_GETTEXTLENGTH, 0, 0);
                    szText = (char*)malloc(iTextLength+1);
                    SendMessage(hChild[1], WM_GETTEXT, iTextLength+1, (LPARAM)szText);
                    SendMessage(hChild[0], LB_ADDSTRING, 0, (LPARAM)szText);
                    SendMessage(hChild[1], WM_SETTEXT, 0, (LPARAM)"");
                    free(szText);
                    break;

                case IDC_rmv:
                    i = SendMessage(hChild[0], LB_GETCURSEL, 0, 0);
                    if(i != LB_ERR) {
                        SendMessage(hChild[0], LB_DELETESTRING, i, 0);
                    }
                    break;

                case ID_EXIT:
                    SendMessage(hwnd, WM_DESTROY, 0, 0);
                    break;

                case ID_DATA:
                    SendMessage(hChild[0], LB_RESETCONTENT, 0, 0);
                    break;

                case ID_ABOUT:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, AboutDlgProc);
                    break;

                default:
                    DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
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
