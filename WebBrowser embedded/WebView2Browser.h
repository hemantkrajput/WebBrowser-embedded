#ifndef __WEBVIEW2_BROWSER
#define __WEBVIEW2_BROWSER

#include <wrl.h>
#include <wil/com.h>
// include WebView2 header
#include "WebView2.h"
#include <string>
using namespace std;

using namespace Microsoft::WRL;

class WebView2Browser
{

public:

    WebView2Browser(HWND hWndParent);

    bool CreateBrowser();

    void Navigate(wstring szUrl);

protected:

    RECT rObject;
    HWND wnd_;
    HWND hWndControl;
};

#endif
