// Task.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Task.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int roundD(int d)
{
	return (int)(0.5 + d);
}

void DrawPolar(HDC hdc, int xc, int yc, int a, int b, COLORREF cl)
{
	// first
	SetPixel(hdc, xc + a, yc - b, cl);
	SetPixel(hdc, xc + b, yc - a, cl);
	// second
	//SetPixel(hdc, xc - a, yc - b, cl);
	//SetPixel(hdc, xc - b, yc - a, cl);
	// third
	SetPixel(hdc, xc - a, yc + b, cl);
	SetPixel(hdc, xc - b, yc + a, cl);
	// fourth
	//SetPixel(hdc, xc + a, yc + b, cl);
	//SetPixel(hdc, xc + b, yc + a, cl);
}

void DrawPoints(HDC hdc, int xc, int yc, int a, int b, COLORREF cl)
{
	// first
	SetPixel(hdc, xc + a, yc - b, cl);
	SetPixel(hdc, xc + b, yc - a, cl);
	// second
	SetPixel(hdc, xc - a, yc - b, cl);
	SetPixel(hdc, xc - b, yc - a, cl);
	// third
	SetPixel(hdc, xc - a, yc + b, cl);
	SetPixel(hdc, xc - b, yc + a, cl);
	// fourth
	SetPixel(hdc, xc + a, yc + b, cl);
	SetPixel(hdc, xc + b, yc + a, cl);
}

void DrawMid(HDC hdc, int xc, int yc, int a, int b, COLORREF cl)
{
	// first
	//SetPixel(hdc, xc + a, yc - b, cl);
	//SetPixel(hdc, xc + b, yc - a, cl);
	// second
	SetPixel(hdc, xc - a, yc - b, cl);
	SetPixel(hdc, xc - b, yc - a, cl);
	// third
	//SetPixel(hdc, xc - a, yc + b, cl);
	//SetPixel(hdc, xc - b, yc + a, cl);
	// fourth
	SetPixel(hdc, xc + a, yc + b, cl);
	SetPixel(hdc, xc + b, yc + a, cl);
}

void polar(HDC hdc, int xc, int yc, int R, COLORREF cl)
{
	for (int i = 0; i <= R; i++)
	{
		double theta = 0;
		double dtheta = 1.0 / i;
		double x = i, y = 0;
		while (x > y)
		{
			DrawPolar(hdc, xc, yc, roundD(x), roundD(y), cl);
			x = i * cos(theta);
			y = i * sin(theta);
			theta += dtheta;
		}
	}
}

void mid(HDC hdc, int xc, int yc, int radius, int radius2, COLORREF color) {
	int x = 0;
	int y = radius;
	int currentMidPoint = 1 - radius;
	int C1 = 3;
	int C2 = 5 - 2 * radius;
	while (x < y) {
		DrawPoints(hdc, xc, yc, x, y, color);
		if (currentMidPoint < 0) {
			currentMidPoint += C1;
			C2 += 2;
		}
		else {
			currentMidPoint += C2;
			C2 += 4;
			y--;
		}
		C1 += 2;
		x++;
	}

	for (int R = radius2; R < radius; R++)
	{
		int x = 0;
		int y = R;
		int currentMidPoint = 1 - R;
		int C1 = 3;
		int C2 = 5 - 2 * R;
		while (x < y) {
			DrawMid(hdc, xc, yc, x, y, color);
			if (currentMidPoint < 0) {
				currentMidPoint += C1;
				C2 += 2;
			}
			else {
				currentMidPoint += C2;
				C2 += 4;
				y--;
			}
			C1 += 2;
			x++;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int xc = -1, yc = -1;
	static int x1 = -1, y1 = -1;
	static int x2, y2;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	 case WM_LBUTTONDOWN:
	 {
		 if (xc == -1)
		 {
			 xc = LOWORD(lParam);
			 yc = HIWORD(lParam);
		 }
		 else if (x1 == -1)
		 {
			 x1 = LOWORD(lParam);
			 y1 = HIWORD(lParam);
		 }
		 else
		 {
			 x2 = LOWORD(lParam);
			 y2 = HIWORD(lParam);
			 HDC hdc = GetDC(hWnd);
			 double R1 = sqrt((xc - x1) * (xc - x1) + (yc - y1) * (yc - y1));
			 double R2 = sqrt((xc - x2) * (xc - x2) + (yc - y2) * (yc - y2));
			 polar(hdc, xc, yc, R1, RGB(0, 0, 0));
			 mid(hdc, xc, yc, R2, R1, RGB(0, 0, 0));
			 //Sleep(1000);
			 //polar(hdc, xc, yc, R1, RGB(255, 255, 255));
			 //mid(hdc, xc, yc, R2, RGB(255, 255, 255));
			 ReleaseDC(hWnd, hdc);
			 xc = -1, x1 = -1;
		 }
	 }
	 break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
