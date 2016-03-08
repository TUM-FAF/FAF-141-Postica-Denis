#include <windows.h>
#include <stdio.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

    hwnd = CreateWindow(
    szAppName,
    "Laboratory Work #2",
    WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL
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

            HWND hInf=CreateWindowEx(0,
                    "BUTTON",
                    NULL,
                    WS_TABSTOP|WS_VISIBLE|
                    WS_CHILD|BS_DEFPUSHBUTTON|BS_OWNERDRAW,
                    420,
                    250,
                    150,
                    40,
                    hwnd,
                    (HMENU)IDC_inf,
                    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                    NULL);
            return 0;
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

    case WM_SIZE :
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
           // iVscrollMax = max(0, NUMLINES + 2 - cyClient / cyChar);
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

    case WM_DESTROY :
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
