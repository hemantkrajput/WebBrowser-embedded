#include <Windows.h>
#include <tchar.h>

#include "WebBrowser.h"
#include "WebView2Browser.h"

HINSTANCE hInst;
HWND hWndMain;
TCHAR* szWndTitleMain = _T("Embedded WebBrowser in Pure C++");
TCHAR* szWndClassMain = _T("WndClass_EmbeddedWB");

ATOM RegMainClass();
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hWndAddressBar;

#define btnBack 1
#define btnNext 2
#define btnRefresh 3
#define btnGo 4

WebBrowser *webBrowser1;
WebView2Browser * webBrowser2;

INT WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   INT nCmdShow)
{

	OleInitialize(NULL);


	MSG msg;

	hInst = hInstance;

	if (!RegMainClass())
	{
		MessageBox(NULL, _T("Cannot register main window class"),
			         _T("Error No. 1"),
					 MB_ICONERROR);
		return 1;
	}

	hWndMain = CreateWindowEx(0, szWndClassMain,
						      szWndTitleMain,
							  WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, CW_USEDEFAULT,
						      CW_USEDEFAULT, CW_USEDEFAULT,
							  NULL, NULL, hInst, NULL);

    SetWindowLong(hWndMain, GWL_STYLE, GetWindowLong(hWndMain, GWL_STYLE) | WS_POPUP);
    SetWindowLong(hWndMain, GWL_EXSTYLE, GetWindowLong(hWndMain, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hWndMain, 0xFF00FF, 0, LWA_COLORKEY);

	ShowWindow(hWndMain, nCmdShow);

	RECT rcClient;
	GetClientRect(hWndMain, &rcClient);

	webBrowser1 = new WebBrowser(hWndMain);
    webBrowser2 = new WebView2Browser(hWndMain);
    RECT rc;
	rc.left = 0;
	rc.top = 45;
	rc.right = rcClient.right/2;
	rc.bottom = rcClient.bottom - 100;
	webBrowser1->SetRect(rc);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

ATOM RegMainClass()
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) CreateSolidBrush(0xFF00FF);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szWndClassMain;
	wc.lpszMenuName = NULL;
	wc.style = 0;

	return RegisterClass(&wc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
    {
        WCHAR rawPath[MAX_PATH];
        GetModuleFileNameW(hInst, rawPath, MAX_PATH);
        std::wstring path = std::wstring(_T("file:///")) + rawPath;
        std::size_t index = path.find_last_of(L"\\") - 1;
        index = path.find_last_of(L"\\", index) + 1;
        path.replace(index, path.length(), L"SamplePage.htm");
        hWndAddressBar =
		CreateWindowEx(0, _T("EDIT"),
                       path.c_str(),
					   WS_CHILD | WS_VISIBLE | WS_BORDER,
					   5, 10,
					   500, 20,
					   hWnd, NULL, hInst, NULL);

		CreateWindowEx(0, _T("BUTTON"),
					   _T("Go"),
					   WS_CHILD | WS_VISIBLE,
					   525, 5,
					   80, 30,
					   hWnd, (HMENU) btnGo, hInst, NULL);
    }
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case btnGo:
			TCHAR *szUrl = new TCHAR[MAX_PATH];
			GetWindowText(hWndAddressBar, szUrl, MAX_PATH);
			webBrowser1->Navigate(szUrl);
            webBrowser2->Navigate(szUrl);
            break;
		}
		break;
	case WM_DESTROY:
		ExitProcess(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}