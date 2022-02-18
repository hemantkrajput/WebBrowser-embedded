#include "WebView2Browser.h"

// Pointer to WebViewController
static wil::com_ptr<ICoreWebView2Controller> webviewController;

// Pointer to WebView window
static wil::com_ptr<ICoreWebView2> webviewWindow;

WebView2Browser::WebView2Browser(HWND _hWndParent)
{
    ::SetRect(&rObject, -300, -300, 300, 300);
    wnd_ = _hWndParent;

    if (CreateBrowser() == FALSE)
    {
        return;
    }
}

bool WebView2Browser::CreateBrowser()
{
    HWND hWndParent = wnd_;
    // Locate the browser and set up the environment for WebView
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWndParent](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

        // Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
        env->CreateCoreWebView2Controller(hWndParent, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [hWndParent](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
            if (controller != nullptr) {
                webviewController = controller;
                webviewController->get_CoreWebView2(&webviewWindow);
            }

            // Add a few settings for the webview
            // The demo step is redundant since the values are the default settings
            ICoreWebView2Settings* Settings;
            webviewWindow->get_Settings(&Settings);
            Settings->put_IsScriptEnabled(TRUE);
            Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
            Settings->put_IsWebMessageEnabled(TRUE);

            // Resize the WebView2 control to fit the bounds of the parent window
            RECT bounds;
            GetClientRect(hWndParent, &bounds);
            RECT rcWV2Client = { bounds.right / 2 , 45, bounds.right, bounds.bottom - 100 };
            webviewController->put_Bounds(rcWV2Client);
            COREWEBVIEW2_COLOR transparentColor = { 0, 255, 0, 255 };
            wil::com_ptr<ICoreWebView2Controller2> controller2 =
                webviewController.query<ICoreWebView2Controller2>();
            result = controller2->put_DefaultBackgroundColor(transparentColor);


            return S_OK;
        }).Get());
        return S_OK;
    }).Get());

    return TRUE;
}

void WebView2Browser::Navigate(wstring szUrl)
{
    webviewWindow->Navigate(szUrl.c_str());
}
