#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <string.h>

#pragma comment(lib, "user32.lib")

extern unsigned char g_byActionOut;

USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	char* szCmd = GetCommandLineA();
	if(strstr(szCmd, "action"))
		g_byActionOut = 0xFF;

	float fScale = 1.f;
	int nPerc = 10;
#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	CCLOG("请设置游戏窗口在屏幕的百分比【10~100】:");
	scanf("%d", &nPerc);
	if(nPerc < 10)
		nPerc = 10;
	if(nPerc > 100)
		nPerc = 100;
	fScale = (float)nPerc * 0.01f;
#endif

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView(100 == nPerc);
    eglView->setViewName("XAct");
	int nScrW = GetSystemMetrics(SM_CXSCREEN);
	int nScrH = GetSystemMetrics(SM_CYSCREEN);
	nScrW = (int)(nScrW * fScale);
	nScrH = (int)(nScrH * fScale);
    eglView->setFrameSize(nScrW, nScrH);
    return CCApplication::sharedApplication()->run();
}
