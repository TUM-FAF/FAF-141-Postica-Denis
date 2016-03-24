#include <windows.h>
#include "resource.h"

HINSTANCE hInst;
LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX  WndCls;
    static char szAppName[] = "LAB#3";
    MSG         Msg;

	hInst       = hInstance;
    WndCls.cbSize        = sizeof(WndCls);
    WndCls.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW | WS_THICKFRAME;
    WndCls.lpfnWndProc   = WindProcedure;
    WndCls.cbClsExtra    = 0;
    WndCls.cbWndExtra    = 0;
    WndCls.hInstance     = hInst;
    WndCls.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    WndCls.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndCls.lpszMenuName  = NULL;
    WndCls.lpszClassName = szAppName;
    WndCls.hIconSm       = LoadIcon(hInstance, IDI_APPLICATION);
    RegisterClassEx(&WndCls);

    CreateWindowEx(0,
                   szAppName,
                   "EasyPaint",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    800,
                    600,
                    NULL,
                    NULL,
                    NULL,
                    NULL);

    while( GetMessage(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessage( &Msg);
    }

    return static_cast<int>(Msg.wParam);
}

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg,
			   WPARAM wParam, LPARAM lParam)
{
    HWND d;
    HBITMAP hbit;

    switch(Msg)
    {
	case WM_DESTROY:
	    PostQuitMessage(WM_QUIT);
	    break;
	case WM_CREATE:
    d = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_BITMAP,
                10, 15,
                120, 20,
                hWnd,
                (HMENU)IDB_EXERCISING,
                hInst,
                NULL);
    hbit = LoadBitmap(hInst, "Bit");
    SendMessage(d, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbit);
	    break;
	default:
	    return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}
