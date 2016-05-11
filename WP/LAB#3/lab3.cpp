#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include "resource.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("WindowsApp");
HINSTANCE hInst;
HBITMAP hImageBttn = 0;
void updateColorControls(HDC, COLORREF, int, int);
int getFromInput(HWND);
POINT getCurrentPointPosition(int, int, RECT, int);
void fillWithGradient(HDC hdc, HBRUSH hBrush, RECT tempRect, RECT gradientRect, int colorNr);

COLORREF fillColor = RGB(255, 255, 255);
COLORREF borderColor = RGB(0, 0, 0);
COLORREF colorSelect(HWND hwnd, COLORREF color)
{
    COLORREF g_rgbCustom[16] = {0};
    CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};

    cc.Flags = CC_RGBINIT | CC_SOLIDCOLOR;
    cc.hwndOwner = hwnd;
    cc.rgbResult = color;
    cc.lpCustColors = g_rgbCustom;

    if(ChooseColor(&cc))
    {
        color = cc.rgbResult;
    }
    return color;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_icon));
    wincl.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_icon));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(150,150,200));

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           _T("EasyPaint3"),
           WS_OVERLAPPEDWINDOW| WS_BORDER | WS_SYSMENU,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           800,
           600,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hPencil, hLine, hBezier, hRectangle, hEllipse, hEraser, hFill, hBorderW, hEraserW, hClear;

    RECT rect ;
    PAINTSTRUCT ps;
    HDC hdc = GetDC(hwnd);

    int screenW, screenH;
    int xMouse, yMouse;

    int xFillPreview = 200;
    int yFillPreview = 515;
    int xStrokePreview = 310;
    int yStrokePreview = 515;

    HDC hdcMem;
    BITMAP bitmap;
    HBITMAP hbit;

    static RECT drawingArea = {25, 55, 750, 500};
    static RECT fillColorRect = {xFillPreview, yFillPreview, xFillPreview + 25, yFillPreview + 20};
    static RECT borderColorRect = {xStrokePreview, yStrokePreview, xStrokePreview + 25, yStrokePreview + 20};

    static RECT tempRect;

    HBRUSH hBrush;
    static POINT pointPen;
    POINT point;
    HPEN linePen;
    int width;

    static BOOL lineStarted, rectangleStarted, ellipseStarted;
    static RECT rectangle, ellipse;
    static int bezierStage = 0;
    static POINT line;
    static POINT bezierPoints[4];

    HPEN borderPen;
    HBRUSH fillBrush;

    switch (message)
    {

        case WM_CREATE:
            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_PENCIL));

            screenW = GetSystemMetrics(SM_CXSCREEN);
            screenH = GetSystemMetrics(SM_CYSCREEN);
            GetWindowRect(hwnd, &rect);
            SetWindowPos(hwnd, 0, (screenW - rect.right)/2, (screenH - rect.bottom)/2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
            hPencil = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE| WS_CHILD|BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP ,
                25, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_pencil,
                hInst,
                NULL);
                SendMessage(hPencil, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_LINE));
            hLine = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
                65, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_line,
                hInst,
                NULL);
                SendMessage(hLine, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BEZIER));
            hBezier = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
                105, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_bezier,
                hInst,
                NULL);
                SendMessage(hBezier, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_RECTANGLE));
            hRectangle = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
                145, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_rectangle,
                hInst,
                NULL);
                SendMessage(hRectangle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_ELLIPSE));
            hEllipse = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
                185, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_ellipse,
                hInst,
                NULL);
                SendMessage(hEllipse, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hImageBttn = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_ERASER));
            hEraser = CreateWindowEx(
                0,
                "Button",
                NULL,
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
                225, 15,
                32, 32,
                hwnd,
                (HMENU)IDB_eraser,
                hInst,
                NULL);
                SendMessage(hEraser, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImageBttn);

            hFill = CreateWindowEx(
                0,
                "Button",
                "Fill object",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                10, 515,
                100, 20,
                hwnd,
                (HMENU)IDB_fill,
                hInst,
                NULL);

            CreateWindowEx(
                0,
                "Static",
                "Fill color",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                120, 515,
                75, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            CreateWindowEx(
                0,
                "Static",
                "Line color",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                230, 515,
                70, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            CreateWindowEx(
                0,
                "Static",
                "Border width",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                350, 515,
                100, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            hBorderW = CreateWindowEx(
                0,
                "Edit",
                "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                460, 515,
                40, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            CreateWindowEx(
                0,
                "Static",
                "Eraser width",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                515, 515,
                100, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            hEraserW = CreateWindowEx(
                0,
                "Edit",
                "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                620, 515,
                40, 20,
                hwnd,
                (HMENU)0,
                hInst,
                NULL);

            hClear = CreateWindowEx(
                NULL,
                "Button",
                "Clear",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                680, 515,
                100, 20,
                hwnd,
                (HMENU)IDB_clear,
                GetModuleHandle(NULL),
                NULL);

            RegisterHotKey(hwnd, HK_dellipse, MOD_CONTROL, 0x45);
            RegisterHotKey(hwnd, HK_dblue, MOD_CONTROL, 0x43);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            updateColorControls(hdc, fillColor, xFillPreview, yFillPreview);
            updateColorControls(hdc, borderColor, xStrokePreview, yStrokePreview);

            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, drawingArea.left, drawingArea.top, drawingArea.right, drawingArea.bottom);

            EndPaint(hwnd, &ps);
            break;

         case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDB_clear:
                    Button_SetCheck(hPencil, BST_UNCHECKED);
                    Button_SetCheck(hLine, BST_UNCHECKED);
                    Button_SetCheck(hBezier, BST_UNCHECKED);
                    Button_SetCheck(hRectangle, BST_UNCHECKED);
                    Button_SetCheck(hEllipse, BST_UNCHECKED);
                    Button_SetCheck(hEraser, BST_UNCHECKED);

                    InvalidateRect(hwnd, &drawingArea, FALSE);
                    InvalidateRect(hwnd, &drawingArea, TRUE);
                    break;
                default:
                    DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
                    break;
            }
            break;

         case WM_GETMINMAXINFO:
         {
             LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
             mmi->ptMinTrackSize.x = 800;
             mmi->ptMinTrackSize.y = 550;
             mmi->ptMaxTrackSize.x = 850;
             mmi->ptMaxTrackSize.y = 600;
             break;
         }
         case WM_LBUTTONDOWN:
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);

            if(xMouse >= fillColorRect.left && xMouse <= fillColorRect.right)
            {
                if(yMouse >= fillColorRect.top && yMouse <= fillColorRect.bottom)
                {
                    fillColor = colorSelect(hwnd, fillColor);
                    updateColorControls(hdc, fillColor, xFillPreview, yFillPreview);
                }
                else if(yMouse >= borderColorRect.top && yMouse <= borderColorRect.bottom)
                {
                    borderColor = colorSelect(hwnd, borderColor);
                    updateColorControls(hdc, borderColor, xStrokePreview, yStrokePreview);
                }
                return 0;
            }

            if( (xMouse > drawingArea.left) && (xMouse < drawingArea.right) &&
                (yMouse > drawingArea.top) && (yMouse < drawingArea.bottom) )
            {
                width = getFromInput(hBorderW);
                point = getCurrentPointPosition(xMouse, yMouse, drawingArea, width);
                xMouse = point.x;
                yMouse = point.y;

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hPencil) == BST_CHECKED))
                {
                    pointPen.x = xMouse;
                    pointPen.y = yMouse;
                }

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hLine) == BST_CHECKED))
                {
                    line.x = xMouse;
                    line.y = yMouse;
                    lineStarted = true;
                }

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hRectangle) == BST_CHECKED))
                {
                    rectangle.left = xMouse;
                    rectangle.top = yMouse;
                    rectangleStarted = true;
                }

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hEllipse) == BST_CHECKED))
                {
                    ellipse.left = xMouse;
                    ellipse.top = yMouse;
                    ellipseStarted = true;
                }

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hBezier) == BST_CHECKED))
                {
                    if(bezierStage == 0)
                    {
                        bezierPoints[0] = point;
                        bezierStage = 1;
                    }
                    else
                    {
                        bezierPoints[2] = point;
                        bezierStage = 3;
                    }
                }
            }
            break;

        case WM_LBUTTONUP:
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);
            width = getFromInput(hBorderW);
            point = getCurrentPointPosition(xMouse, yMouse, drawingArea, width);
            xMouse = point.x;
            yMouse = point.y;

            borderPen = CreatePen(PS_SOLID, width, borderColor);
            if(Button_GetCheck(hFill) == BST_CHECKED)
                fillBrush = CreateSolidBrush(fillColor);
            else
                fillBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

            if(lineStarted)
            {
                SelectObject(hdc, borderPen);
                MoveToEx(hdc, xMouse, yMouse, NULL);
                LineTo(hdc, line.x, line.y);
                DeleteObject(borderPen);
                lineStarted = false;
            }

            if(rectangleStarted)
            {
                SelectObject(hdc, borderPen);
                SelectObject(hdc, fillBrush);
                Rectangle(hdc, rectangle.left, rectangle.top, xMouse, yMouse);

                DeleteObject(borderPen);
                DeleteObject(fillBrush);

                rectangleStarted = false;
            }

            if(ellipseStarted)
            {
                SelectObject(hdc, borderPen);
                SelectObject(hdc, fillBrush);

                Ellipse(hdc, ellipse.left, ellipse.top, xMouse, yMouse);
                DeleteObject(borderPen);
                DeleteObject(fillBrush);

                ellipseStarted = false;
            }

            if(bezierStage == 1)
            {
                bezierPoints[1] = point;
                bezierStage = 2;
            }

            if(bezierStage == 3)
            {
                bezierPoints[3] = point;
                bezierStage = 0;
                SelectObject(hdc, borderPen);
                PolyBezier(hdc, bezierPoints, 4);
                DeleteObject(borderPen);
            }
            break;

        case WM_MOUSEMOVE:
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);
            if( (xMouse > drawingArea.left) && (xMouse < drawingArea.right) &&
                (yMouse > drawingArea.top) && (yMouse < drawingArea.bottom) )
            {
                if((wParam == MK_LBUTTON) && (Button_GetCheck(hPencil) == BST_CHECKED))
                {
                    width = getFromInput(hBorderW);
                    point = getCurrentPointPosition(xMouse, yMouse, drawingArea, width);
                    xMouse = point.x;
                    yMouse = point.y;
                    linePen = CreatePen(PS_SOLID, width, borderColor);
                    SelectObject(hdc, linePen);
                    MoveToEx(hdc, xMouse, yMouse, NULL);
                    LineTo(hdc, pointPen.x, pointPen.y);
                    DeleteObject(linePen);
                    pointPen.x = xMouse;
                    pointPen.y = yMouse;
                }

                if((wParam == MK_LBUTTON) && (Button_GetCheck(hEraser) == BST_CHECKED))
                {
                    width = getFromInput(hEraserW);
                    point = getCurrentPointPosition(xMouse, yMouse, drawingArea, width);
                    xMouse = point.x;
                    yMouse = point.y;
                    rect.left = point.x - (width / 2);
                    rect.right = point.x + (width / 2);
                    rect.top = point.y - (width / 2);
                    rect.bottom = point.y + (width / 2);
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                }
            }
            break;

        case WM_CTLCOLORSTATIC:
            {
                HDC hDC = (HDC)wParam;
                SetBkColor(hDC, GetSysColor(COLOR_WINDOW));
                SetTextColor(hDC, RGB(255,69,0));
                SetBkMode(hDC, TRANSPARENT);
                return (INT_PTR)CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
            }
            break;

        case WM_HOTKEY:
        {
            switch(wParam)
            {
                case HK_dellipse:

                    Button_SetCheck(hPencil, BST_UNCHECKED);
                    Button_SetCheck(hLine, BST_UNCHECKED);
                    Button_SetCheck(hBezier, BST_UNCHECKED);
                    Button_SetCheck(hRectangle, BST_UNCHECKED);
                    Button_SetCheck(hEraser, BST_UNCHECKED);
                    Button_SetCheck(hEllipse, BST_CHECKED);
                    break;

                case HK_dblue:
                    Button_SetCheck(hFill, BST_CHECKED);
                    fillColor = RGB(0,0,255);
                    updateColorControls(hdc, fillColor, xFillPreview, yFillPreview);
                    break;
                default:
                    break;
            }
            break;
        }

        case WM_CLOSE:
            if(MessageBox(hwnd, "Close application?", "Quit", MB_YESNO) == IDYES) DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

void updateColorControls(HDC hdc, COLORREF rgb, int xLeft, int yTop)
{
    HBRUSH hBrush = CreateSolidBrush(rgb);
    HPEN hPen = CreatePen(PS_INSIDEFRAME, 2, RGB(10, 40, 140));

    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);

    Rectangle(hdc, xLeft, yTop, xLeft + 25, yTop + 20);

    hBrush = CreateSolidBrush(RGB(10, 40, 140));
    SelectObject(hdc, hBrush);

    Rectangle(hdc, xLeft+18, yTop+13, xLeft + 25, yTop + 20);

    DeleteObject(hPen);
    DeleteObject(hBrush);
}

int getFromInput(HWND input)
{
    int result, len;
    len = SendMessage(input, WM_GETTEXTLENGTH, 0, 0);

    char * temp = (char *)malloc(len + 1);
    SendMessage(input, WM_GETTEXT, len + 1, (LPARAM)temp);
    result = atoi(temp);
    free(temp);
    return result;
}

POINT getCurrentPointPosition(int xMouse, int yMouse, RECT limit, int width)
{
    POINT result;
    width = width / 2 + 1;

    if(xMouse + width > limit.right)
        result.x = limit.right - width;
    else if(xMouse - width < limit.left)
        result.x = limit.left + width;
    else
        result.x = xMouse;

    if(yMouse - width < limit.top)
        result.y = limit.top + width;
    else if(yMouse + width > limit.bottom)
        result.y = limit.bottom - width;
    else
        result.y = yMouse;

    return result;
}

void fillWithGradient(HDC hdc, HBRUSH hBrush, RECT tempRect, RECT gradientRect, int colorNr)
{
    for(int i = 0; i < (gradientRect.right - gradientRect.left); i++) {
        int color;
        color = i * 255 / (gradientRect.right - gradientRect.left);
        tempRect.left  = gradientRect.left  + i;
        tempRect.right = gradientRect.left + i + 1;
        if (colorNr == 0)
            hBrush = CreateSolidBrush(RGB(color, 0, 0));
        else if (colorNr == 1)
            hBrush = CreateSolidBrush(RGB(0, color, 0));
        else
            hBrush = CreateSolidBrush(RGB(0, 0, color));
        FillRect(hdc, &tempRect, hBrush);
        DeleteObject(hBrush);
    }
}
