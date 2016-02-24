/*	Trim fat from windows*/
#define WIN32_LEAN_AND_MEAN
#pragma comment(linker, "/subsystem:windows")
/*	Pre-processor directives*/

#include <windows.h>
/*	Windows Procedure Event Handler*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	/*	Device Context*/
	HDC hDC;
	/*	Text for display*/
	char string[] = "Done with Pride and Prejudice by Postica Denis";
	/*	Switch message, condition that is met will execute*/
	switch(message)
	{
		/*	Window is being created*/
		case WM_CREATE:
		    CreateWindowEx(
               WS_EX_CLIENTEDGE,
              TEXT("EDIT"),
              TEXT("0"),
              WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
              50,
              50,
              100,
              50,
              hwnd,
              (HMENU) 10,
              GetModuleHandle(NULL),
              NULL
            );
			return 0;
			break;
		/*	Window is closing*/
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
		/*	Window needs update*/
		case WM_PAINT:
			hDC = BeginPaint(hwnd,&paintStruct);
			/*	Set txt color to blue*/
			SetTextColor(hDC, COLORREF(0x00FF0000));
			/*	Display text in middle of window*/
			TextOut(hDC,150,150,string,sizeof(string)-1);
			EndPaint(hwnd, &paintStruct);
			return 0;
			break;
		default:
			break;
	}
	return (DefWindowProc(hwnd,message,wParam,lParam));
}
/*	Main function*/
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASSEX  windowClass;		//window class
	HWND		hwnd;				//window handle
	MSG			msg;				//message
	bool		done;				//flag saying when app is complete
	/*	Fill out the window class structure*/
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "MyClass";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	/*	Register window class*/
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}
	/*	Class registerd, so now create window*/
	hwnd = CreateWindowEx(NULL,		//extended style
		"MyClass",			//class name
		"WP lab#1",		//app name
		WS_OVERLAPPEDWINDOW |		//window style
		WS_VISIBLE |
		WS_SYSMENU,
		100,100,			//x/y coords
		600,400,			//width,height
		NULL,				//handle to parent
		NULL,				//handle to menu
		hInstance,			//application instance
		NULL);				//no extra parameter's
	/*	Check if window creation failed*/
	if (!hwnd)
		return 0;
	done = false; //initialize loop condition variable
	/*	main message loop*/
	while(!done)
	{
		PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
		if (msg.message == WM_QUIT) //check for a quit message
		{
			done = true; //if found, quit app
		}
		else
		{
			/*	Translate and dispatch to event queue*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}


