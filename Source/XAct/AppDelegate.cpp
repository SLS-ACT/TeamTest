#include "AppDelegate.h"
#include "UpdateScene.h"
#include "HelloWorldScene.h"
#include "CCLuaEngine.h"
#include "XBaseObj/XMoveObj.h"
#include "Game/XGameState.h"
#include "XSound/XSoundManager.h"
#include "XUI/XUIManger.h"
#include "XNet/XNetWork.h"
//#include "Update/XFtp.h"

USING_NS_CC;

unsigned short SCR_W = 800;
unsigned short SCR_H = 600;
unsigned short SCRPIX_W = 800;
unsigned short SCRPIX_H = 600;
unsigned char g_ShowUILine = 0x00;
float SCR_SCALE = 1.f;
float XMoveObj::m_ofGravity = -1600.f;
//float XMoveObj::m_ofMovScale = 0.2f;
float XMoveObj::m_ofMovScale = 0.f;	// 暂时去掉招式联动
unsigned char g_byShowLine = 0x00;
// 资源是否是裁剪了
unsigned char g_byIsTexCut = 0x00;
// 是否需要重载
unsigned char g_byReload = 0x00;
// 是否处于UpdateScene
unsigned char g_byUpdate = 0xFF;
// 是否掉线了
unsigned char g_byOffLine = 0x00;
// 是否小米
unsigned char g_byMI = 0x00;
// 渠道号
unsigned short g_wChannelNum = 0;
// 充值号
unsigned short g_wCharge = 0;
// 是否处于低内存模式【RGBA4444】
unsigned char g_byRGBA4444 = 0x00;

unsigned long XMemLog::m_dwTable = 0;
unsigned long XMemLog::m_dwStrBuf = 0;
unsigned long XMemLog::m_dwUI = 0;
unsigned long XMemLog::m_dwHash = 0;
unsigned long XMemLog::m_dwTex = 0;
unsigned long XMemLog::m_dwNew = 0;
unsigned long XMemLog::m_dwLua = 0;
unsigned long XMemLog::m_dwUpdate = 0;
unsigned char XMemLog::m_byCPU = 0;
unsigned long XMemLog::m_dwMemPhy = 0;
unsigned long XMemLog::m_dwMemVtl = 0;

extern "C" const char* LuaDump(lua_State* pLS);

extern "C" int IsFileExist(const char* szPath)
{
	char szXPFPath[MAX_PATH];
	unsigned long dwSeek = 0, dwSize = 0;
	CCFileUtils* pFU = CCFileUtils::sharedFileUtils();
	if(!pFU)
		return 0;
	const char* szPathC = szPath;
	if((char)'.' == *szPathC)
	{
		while((char)'.' == *szPathC || (char)'\\' == *szPathC || (char)'/' == *szPathC)
			++szPathC;
	}
	bool bRes = pFU->getFileInfoInXPF(szPathC, szXPFPath, dwSeek, dwSize);
	if(bRes)
	{
		return 2;
	}
	bRes = pFU->isFileExist(szPath);
	if(bRes)
		return 1;
	std::string strFP = pFU->fullPathForFilename(szPath);
	bRes = pFU->isFileExist(strFP.c_str());
	if(bRes)
		return 1;
	return 0;
}

extern "C" lua_XPF OpenXPFFile(const char* szPath)
{
	lua_XPF stBuf;
	stBuf.pBuf = NULL;
	stBuf.nLen = 0;
	const char* szPathC = szPath;
	if((char)'.' == *szPathC)
	{
		while((char)'.' == *szPathC || (char)'\\' == *szPathC || (char)'/' == *szPathC)
			++szPathC;
	}
	unsigned long dwSize = 0;
	unsigned char* abyBuf = CCFileUtils::sharedFileUtils()->getFileData(szPathC, "rb", &dwSize);
	if(!abyBuf || !dwSize)
		return stBuf;
	stBuf.pBuf = new char[dwSize + 1];
	memcpy(stBuf.pBuf, abyBuf, sizeof(char) * dwSize);
	stBuf.pBuf[dwSize] = '\0';
	stBuf.nLen = (int)dwSize;
	CCFileUtils::sharedFileUtils()->delFileData(abyBuf);
	return stBuf;
}

extern "C" void CloseXPFFile(char* pBuf)
{
	delete[] pBuf;
}

extern "C" const char* GetRealPath(const char* szPath)
{
	static char szRealPath[MAX_PATH];
	strncpy(szRealPath, CCFileUtils::sharedFileUtils()->fullPathForFilename(szPath).c_str(), sizeof(char) * MAX_PATH);
	return szRealPath;
}

AppDelegate::AppDelegate() {
	XMemLog::m_dwNew += sizeof(AppDelegate);
}

AppDelegate::~AppDelegate() 
{
	XMemLog::m_dwNew -= sizeof(AppDelegate);
	XMemPoolRelease::GetMemPoolRelease().ReleaseAllPool();
	XSoundManager::ReleaseSound();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    XDirector* pDirector = XDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pDirector->setOpenGLView(pEGLView);
	cocos2d::CCSize szView = pEGLView->getVisibleSize();
	if (szView.width < szView.height)
	{
		// for ios 6
		float fTmp = szView.width;
		szView.width = szView.height;
		szView.height = fTmp;
		pEGLView->setFrameSize(szView.width, szView.height);
	}
	SCRPIX_W = szView.width;
	SCRPIX_H = szView.height;
	float fScaleH = 1.f / szView.height;
	SCR_W = (unsigned short)(szView.width * fScaleH * SCR_H);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCR_W, SCR_H, kResolutionShowAll); // kResolutionNoBorder
	if(SCR_W)
		SCR_SCALE = szView.width / SCR_W;
    // turn on display FPS
    pDirector->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / FPS);
	//pDirector->setProjection(cocos2d::kCCDirectorProjection2D);	//指定cocos2d为2d模式

	//// 对于点、线的抗锯齿处理
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//// 多重采样方式
	//// 注意：如果启用多重采样，上面的点、线、多边形的平滑处理将会被忽略
	//glEnable(GL_MULTISAMPLE);

	srand(time(NULL));

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	CCString* pstrFileContent = CCString::createWithContentsOfFile("Script/hello.lua");
//	if (pstrFileContent)
//	{
//		pEngine->executeString(pstrFileContent->getCString());
//	}
//#else
//	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("Script/hello.lua");
//	pEngine->addSearchPath(path.substr(0, path.find_last_of("/")).c_str());
//	pEngine->executeScriptFile(path.c_str());
//#endif

	XNetWork::Instance().Init();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#   ifdef __XFTPSOCKET_H__
	// IOS下尝试上传日志
	std::string strLog;
	strLog = CCFileUtils::sharedFileUtils()->getWritablePath();
	strLog += "xdebug.UL.log";
	do 
	{
		if(!XFileAPIC::FDExist(strLog.c_str()))
			break;
		bool bRes;
		XHttp::XFtp stFtp;
		bRes = stFtp.Connect("http://ios.lx83.com:12000/");
		if(!bRes)
			break;
		bRes = stFtp.Login("log", "log");
		if(!bRes)
			break;
		bRes = stFtp.Upload(strLog.c_str(), "/xdebug.log");
		if(!bRes)
			break;
		bRes = stFtp.Quit();
		if(!bRes)
			break;
		stFtp.Close();
	} while (false);
#   endif
#endif

	CCTextureCache::sharedTextureCache(XTextureCache::InstancePtr());

#	if defined(ANDROID)
	char* pShmData = (char*)XShmAnd::xshmem_open();
	if(pShmData)
	{
		CCLOG("xdebug.android enabled!");
		sprintf(pShmData, "XAct started!");
	}
	else
		CCLOG("xdebug.android disabled!");
#	endif
	XScene *pScene = UpdateScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	/*if(g_byUpdate)
	{
	XDirector::sharedDirector()->end();
	#		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#		endif
	return;
	}
	*/
	CCLOG("【 - 切换后台 - 】");
    XDirector::sharedDirector()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    XSoundManager::PauseAllSound();
	/*if(HelloWorld::m_pLoading)
	{
	HelloWorld::m_pLoading->cleanup();
	HelloWorld::m_pLoading = NULL;
	}
	if(HelloWorld::m_pFrame)
	{
	HelloWorld::m_pFrame->cleanup();
	HelloWorld::m_pFrame = NULL;
	}*/
	//XUIManger::OnBeforReLoad();
	cocos2d::CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	XGameStateManger::OnEnterBackground();
	//XGameStateManger::NetClose();
//#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	CCLOG("【 - 切换前台 - 】");
    XDirector::sharedDirector()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
	XSoundManager::ResumeAllSound();
	//g_byReload = 0xFE;
	XUIManger::SetTouchBegin(0x00);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	XGameStateManger::OnEnterForeground();
//#endif
}
