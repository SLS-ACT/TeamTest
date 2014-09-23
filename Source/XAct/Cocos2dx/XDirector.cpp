#include "../Common.h"
#include "../XHelper/XMemRelease.h"
#include "XLoader/XLoader.h"
#include "sprite_nodes/XSpriteFrameCache.h"
#include "XFileUtils.h"
#include "../UpdateScene.h"

XCPP_DEFINED();

XStrConvert _stXStrCnvMainTh;

NS_CC_BEGIN

char			XDirector::m_szDebugInfo[DEBUG_INFO_MAX];
char*			XDirector::m_szNewInfo = m_szDebugInfo;
CCLabelTTF*		XDirector::m_pLabel = NULL;
unsigned char	XDirector::m_byDirty = 0x00;
unsigned char	XDirector::m_byShowDebug = XDirector::eShowNone;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	volatile unsigned char XDirector::m_bySDKInit = 0x00;
#endif
void XDirector::ShowDebug(unsigned char byType)
{
	m_byShowDebug = byType;
	XDirector* pDirector = XDirector::sharedDirector();
	if(XDirector::eShowNone == m_byShowDebug)
		pDirector->m_bDisplayStats = false;
	else
		pDirector->m_bDisplayStats = true;
	m_byDirty = 0xFF;
}
void XDirector::DebugLog(const char * pszFormat, ...)
{
	XFUNC_START();
	char szBuf[DEBUG_INFO_MAX];
	memset(szBuf, 0, sizeof(char) * DEBUG_INFO_MAX);

	va_list ap;
	va_start(ap, pszFormat);
	int nParse = vsnprintf(szBuf, DEBUG_INFO_MAX, pszFormat, ap);
	va_end(ap);

	szBuf[nParse] = '\0';
	static XStrConvert _stXStrCnvThread;
	const char* szUtf8 = _stXStrCnvThread.XGB2312_TO_UTF8(szBuf);
	if(!szUtf8)
		return;
	const char* szTmp = szUtf8;
	int nLen = 0;
	while(*szTmp++)
		++nLen;
	if(nLen >= DEBUG_INFO_MAX)
		return;
	int nUsed = m_szNewInfo - m_szDebugInfo;
	if(nUsed + nLen + 2 > DEBUG_INFO_MAX)			// '\n\0'
	{
		int nCpyHead = nUsed + nLen + 2 - DEBUG_INFO_MAX;
		int nCpyLen = nUsed - nCpyHead;
		if(nCpyLen)
			memcpy(m_szDebugInfo, &m_szDebugInfo[nCpyHead], nCpyLen);
		m_szNewInfo = &m_szDebugInfo[nCpyLen];
	}
	*m_szNewInfo++ = '\n';
	memcpy(m_szNewInfo, szUtf8, nLen);
	m_szNewInfo += nLen;
	*m_szNewInfo = '\0';

#	ifdef _WINDOWS_
	szBuf[nParse++] = '\n';
	szBuf[nParse++] = '\0';
	printf(szBuf);
	OutputDebugStringA(szBuf);
#	else
	static FILE* pFOut = NULL;
	if(0xFFFFFFFF != (unsigned long)pFOut)
	{
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // copy for upload in iOS
        static FILE* pFOutUL = NULL;
        if(0xFFFFFFFF != (unsigned long)pFOutUL)
        {
			std::string strLogUL;
            strLogUL = CCFileUtils::sharedFileUtils()->getWritablePath();
            strLogUL += "xdebug.log";
            pFOutUL = fopen(strLogUL.c_str(), "rb");
            if(pFOutUL)
            {
                strLogUL = CCFileUtils::sharedFileUtils()->getWritablePath();
                strLogUL += "xdebug.UL.log";
                FILE* pFW = fopen(strLogUL.c_str(), "wb");
                if(pFW)
                {
                    const int nULBLen = 1024;
                    char szULBuf[nULBLen];
                    int nRead;
                    while((nRead = (int)fread(szULBuf, sizeof(char), nULBLen, pFOutUL)) > 0)
                    {
                        fwrite(szULBuf, sizeof(char), nRead, pFW);
                    }
                    fclose(pFW);
                }
                fclose(pFOutUL);
            }
			pFOutUL = (FILE*)0xFFFFFFFF;
        }
#		endif
		if(!pFOut)
		{
			std::string strLog;
#			if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			strLog = "/mnt/sdcard/xdebug.log";
			pFOut = fopen(strLog.c_str(), "wb");
#			endif
			if(!pFOut)
			{
				strLog = CCFileUtils::sharedFileUtils()->getWritablePath();
				strLog += "xdebug.log";
				pFOut = fopen(strLog.c_str(), "wb");
			}
		}
		if(pFOut)
		{
			szBuf[nParse++] = '\n';
			szBuf[nParse++] = '\0';
			fwrite(szBuf, sizeof(char), nParse, pFOut);
			fflush(pFOut);
		}
		else
			pFOut = (FILE*)0xFFFFFFFF;
	}
#	endif
    
#   if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    cocos2d::CCLog(szBuf);
#	endif
    
	if(eShowDebug == m_byShowDebug)
		m_byDirty = 0xFF;
	XFUNC_END();
}
	
bool XDirector::m_bInited = false;

XDirector* XDirector::sharedDirector(void)
{
	XFUNC_START();
	if(!m_bInited)
	{
		m_bInited = XDirector::Instance().init();
		if(!m_bInited)
			return NULL;
		CCDirector::sharedDirector(XDirector::InstancePtr());
		CCFileUtils::setXPFFileUtils(XFileUtils::InstancePtr());
		XSpriteFrameCache::Instance().init();
		CCSpriteFrameCache::sharedSpriteFrameCache(XSpriteFrameCache::InstancePtr());
		XResLoader::Instance().StartLoadThread();
	}
	return XDirector::InstancePtr();
	XFUNC_END_RETURN(XDirector*, ptr, NULL);
}

void XDirector::startAnimation(void)
{
	XFUNC_START();
    if (CCTime::gettimeofdayCocos2d(m_pLastUpdate, NULL) != 0)
    {
        CCLOG("cocos2d: DisplayLinkDirector: Error on gettimeofday");
    }

    m_bInvalid = false;
    //CCApplication::sharedApplication()->setAnimationInterval(m_dAnimationInterval);
	XFUNC_END();
}

void XDirector::stopAnimation(void)
{
	XFUNC_START();
    m_bInvalid = true;
	XFUNC_END();
}

void XDirector::setAnimationInterval(double dValue)
{
	XFUNC_START();
    m_dAnimationInterval = dValue;
    if (! m_bInvalid)
    {
        stopAnimation();
        startAnimation();
    }    
	XFUNC_END();
}

bool XDirector::init(void)
{
	XFUNC_START();
    if (CCTime::gettimeofdayCocos2d(&m_stNow, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        return false;
    }
	bool bRes = CCDirector::init();
	if(bRes)
	{
		memset(m_szDebugInfo, 0, sizeof(char) * DEBUG_INFO_MAX);
		m_szNewInfo = m_szDebugInfo;
	}
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}

void XDirector::purgeDirector()
{
	XFUNC_START();
    // cleanup scheduler
    getScheduler()->unscheduleAll();
    
    // don't release the event handlers
    // They are needed in case the director is run again
    m_pTouchDispatcher->removeAllDelegates();

    if (m_pRunningScene)
    {
        m_pRunningScene->onExitTransitionDidStart();
        m_pRunningScene->onExit();
        m_pRunningScene->cleanup();
        m_pRunningScene->release();
    }
    
    m_pRunningScene = NULL;
    m_pNextScene = NULL;

    // remove all objects, but don't release it.
    // runWithScene might be executed after 'end'.
    m_pobScenesStack->removeAllObjects();

    stopAnimation();

    CC_SAFE_RELEASE_NULL(m_pFPSLabel);
    CC_SAFE_RELEASE_NULL(m_pSPFLabel);
    CC_SAFE_RELEASE_NULL(m_pDrawsLabel);

    // purge bitmap cache
    CCLabelBMFont::purgeCachedData();

    // purge all managed caches
    ccDrawFree();
    CCAnimationCache::purgeSharedAnimationCache();
    CCSpriteFrameCache::purgeSharedSpriteFrameCache();
    CCTextureCache::purgeSharedTextureCache();
    CCShaderCache::purgeSharedShaderCache();
    CCFileUtils::purgeFileUtils();
    CCConfiguration::purgeConfiguration();

    // cocos2d-x specific data structures
    CCUserDefault::purgeSharedUserDefault();
    CCNotificationCenter::purgeNotificationCenter();

    ccGLInvalidateStateCache();
    
    CHECK_GL_ERROR_DEBUG();
    
    // OpenGL view
    m_pobOpenGLView->end();
    m_pobOpenGLView = NULL;

	// Log
#	ifdef __XMEMPARSE_H__
	OGDP::XMemParse::Instance().Log();
#	endif
	XFUNC_END();
}

void XDirector::mainLoop(void)
{
	XFUNC_START();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		if(!XDirector::m_bySDKInit)
		{
			return;
		}
	#endif
//#	ifdef DEBUG_INFO
//	unsigned long dwMemUsed = XMemPoolRelease::GetMemPoolRelease().GetMemUsed();
//	unsigned long dwMB	= dwMemUsed / 1000000;
//	unsigned long dwKB	= (dwMemUsed % 1000000) / 1000;
//	unsigned long dwB	= (dwMemUsed % 1000000) % 1000;
//	if(dwMB)
//		SCRLOG("MemPool Type[%d], Mem Used[%d,%03d,%03d Bytes]",
//			XMemPoolRelease::GetMemPoolRelease().GetMemType(),
//			dwMB, dwKB, dwB);
//	else if(dwKB)
//		SCRLOG("MemPool Type[%d], Mem Used[%d,%03d Bytes]",
//			XMemPoolRelease::GetMemPoolRelease().GetMemType(),
//			dwKB, dwB);
//	else
//		SCRLOG("MemPool Type[%d], Mem Used[%d Bytes]",
//			XMemPoolRelease::GetMemPoolRelease().GetMemType(),
//			dwB);
//#endif
	if (m_bPurgeDirecotorInNextLoop)
	{
		CCLOG("¡¾ - ÓÎÏ·½áÊø - ¡¿");
		m_bPurgeDirecotorInNextLoop = false;
		purgeDirector();
		//XMemPoolRelease::GetMemPoolRelease().ReleaseAllPool();
	}
	
	{
		struct cc_timeval stNow;
		if (CCTime::gettimeofdayCocos2d(&stNow, NULL) != 0)
		{
			CCLOG("XDirector::mainLoop error in gettimeofday stNow.");
			return;
		}
		bool bRes = run(stNow);
		struct cc_timeval stNext;
		if (CCTime::gettimeofdayCocos2d(&stNext, NULL) != 0)
		{
			CCLOG("XDirector::mainLoop error in gettimeofday stNext.");
			return;
		}
		float fRunTime = (stNext.tv_sec - stNow.tv_sec) + (stNext.tv_usec - stNow.tv_usec) / 1000000.0f;
		if(m_dAnimationInterval > 0.001f)
		{
			if(m_dAnimationInterval <= fRunTime)
				fRunTime = 0.015f;
			fRunTime = m_dAnimationInterval - fRunTime;
			XResLoader::Instance().UpdateCallback(fRunTime);
			//CCLOG("UpdateCallback could use %f sec.", fRunTime);
		}
		if (!m_bInvalid)
			drawScene();
		if(bRes)
		{
			// release the objects
			CCPoolManager::sharedPoolManager()->pop();
		}
		if (CCTime::gettimeofdayCocos2d(&stNext, NULL) != 0)
		{
			CCLOG("XDirector::mainLoop error in gettimeofday stNext.");
			return;
		}
		fRunTime = (stNext.tv_sec - stNow.tv_sec) + (stNext.tv_usec - stNow.tv_usec) / 1000000.0f;
		//CCLOG("This frame used %f sec.", fRunTime);
		if (m_bInvalid)
		{
#			if defined(WIN32) && defined(_WINDOWS)
			Sleep(1000.f);
#			else
			usleep(1000.f);
#			endif
		}
		else
		{
			if(m_dAnimationInterval > 0.001f)
			{
				if(m_dAnimationInterval > fRunTime)
				{
					fRunTime = m_dAnimationInterval - fRunTime;
					fRunTime *= (1000.f);
#				if defined(WIN32) && defined(_WINDOWS)
					Sleep((unsigned long)fRunTime);
#				else
					usleep(fRunTime * 1000);
#				endif
				}
			}
		}
	}
	XFUNC_END();
}

bool XDirector::run(const cc_timeval& stNow)
{
	XFUNC_START();
	if(m_bDisplayStats && m_byShowDebug)
	{
		if(!m_pLabel)
		{
			static CCLabelTTF stLabel;
			stLabel.initWithString(m_szDebugInfo, "Arial", 18);
			stLabel.setAnchorPoint(ccp(0.f, 0.f));
			stLabel.setPosition(ccp(0.f, 0.f));
			stLabel.setColor(ccc3(0xFF, 0xFF, 0x99));
			//stLabel.setOpacity(0x7F);
			//stLabel.getTexture()->setAliasTexParameters();
			m_pLabel = &stLabel;
			m_byDirty = 0x00;
		}
		// ShowMemory
		if(eShowMemory == m_byShowDebug)
		{
			static float fLastDirty = m_fRunTime;
			if(m_fRunTime - fLastDirty >= 1.f)
			{
				fLastDirty = m_fRunTime;
				m_byDirty = 0xFF;
			}
		}
		if(m_pLabel && m_byDirty)
		{
			m_byDirty = 0x00;
			if(eShowDebug == m_byShowDebug)
				m_pLabel->setString(m_szDebugInfo);
			else if(eShowMemory == m_byShowDebug)
			{
				static std::string strDevice = "";
				if(strDevice.empty())
#				if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					strDevice = CCFileUtils::sharedFileUtils()->getDevice();
#				else
					strDevice = "Win or iOS";
#				endif
				CCLOG("Device:                 %s", strDevice.c_str());
				CCLOG("cocos2d: GL_VENDOR:     %s", glGetString(GL_VENDOR));
				CCLOG("cocos2d: GL_RENDERER:   %s", glGetString(GL_RENDERER));
				CCLOG("cocos2d: GL_VERSION:    %s", glGetString(GL_VERSION));
				char szMem[1024];
				sprintf(szMem, 
					"Device      : %s\n"
					"GL_VENDOR   : %s\n"
					"GL_RENDERER : %s\n"
					"GL_VERSION  : %s\n"
					"Scr : %d * %d\n"
					"Scene : %d * %d\n"
					"Update to gpu : %d Bytes\n"
					"Table : %d Bytes\n"
					"StrBuf : %d Bytes\n"
					"UI : %d Bytes\n"
					"Hash : %d Bytes\n"
					"Texture : %d Bytes\n"
					"New : %d Bytes\n"
					"Lua : %d Bytes\n"
					"XBN : %d / %d\n"
					"XSpr : %d / %d\n"
					"CPU : %d%%\n"
					"MemPhy : %d KB\n"
					"MemVtl : %d KB"
					,
					strDevice.c_str(),
					glGetString(GL_VENDOR),
					glGetString(GL_RENDERER),
					glGetString(GL_VERSION),
					SCRPIX_W, SCRPIX_H,
					SCR_W, SCR_H,
					XMemLog::m_dwUpdate,
					XMemLog::m_dwTable,
					XMemLog::m_dwStrBuf,
					XMemLog::m_dwUI,
					XMemLog::m_dwHash,
					XMemLog::m_dwTex,
					XMemLog::m_dwNew,
					XMemLog::m_dwLua,
					XMP_GETNUMUNUSE(XSpriteBatchNode), XMP_GETNUMALL(XSpriteBatchNode),
					XMP_GETNUMUNUSE(XSprite), XMP_GETNUMALL(XSprite),
					XMemLog::m_byCPU,
					XMemLog::m_dwMemPhy,
					XMemLog::m_dwMemVtl
					);
				m_pLabel->setString(szMem);
				XMemLog::m_dwUpdate = 0;
			}
		}
	}
	m_fLastFrameTime = (stNow.tv_sec - m_stNow.tv_sec) + (stNow.tv_usec - m_stNow.tv_usec) / 1000000.0f;
	m_fRunTime += m_fLastFrameTime;
	m_stNow = stNow;
	if(m_dAnimationInterval > 0.001f)
		m_dwFrameNum = (unsigned long)(m_fRunTime / m_dAnimationInterval);
	if(m_dwDrawFrame >= m_dwFrameNum)
        return false;
	unsigned long dwFrameRun = m_dwFrameNum - m_dwDrawFrame;
	m_dwDrawFrame = m_dwFrameNum;
	m_dwObjRunNum = 0;
	if(m_bIsXScene)
	{
		float fDeltaTime = m_fLastFrameTime / dwFrameRun;
		XScene* pScene = (XScene*)m_pRunningScene;
		for(unsigned long dwPos = 0; dwPos < dwFrameRun; ++dwPos)
			m_dwObjRunNum += pScene->OnFrame();
	}
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XDirector::calculateDeltaTime(void)
{
	XFUNC_START();
    // new delta time. Re-fixed issue #1277
    if (m_bNextDeltaTimeZero)
    {
        m_fDeltaTime = 0;
        m_bNextDeltaTimeZero = false;
    }
    else
    {
        m_fDeltaTime = (m_stNow.tv_sec - m_pLastUpdate->tv_sec) + (m_stNow.tv_usec - m_pLastUpdate->tv_usec) / 1000000.0f;
        m_fDeltaTime = MAX(0, m_fDeltaTime);
    }
#ifdef DEBUG
    // If we are debugging our code, prevent big delta time
    if(m_fDeltaTime > 0.2f)
    {
        m_fDeltaTime = 1 / 60.0f;
    }
#endif
    *m_pLastUpdate = m_stNow;
	XFUNC_END();
}

void XDirector::setNextScene(void)
{
	XFUNC_START();
    bool runningIsTransition = dynamic_cast<CCTransitionScene*>(m_pRunningScene) != NULL;
    bool newIsTransition = dynamic_cast<CCTransitionScene*>(m_pNextScene) != NULL;

    // If it is not a transition, call onExit/cleanup
     if (! newIsTransition)
     {
         if (m_pRunningScene)
         {
             m_pRunningScene->onExitTransitionDidStart();
             m_pRunningScene->onExit();
         }
 
         // issue #709. the root node (scene) should receive the cleanup message too
         // otherwise it might be leaked.
         if (m_bSendCleanupToScene && m_pRunningScene)
         {
             m_pRunningScene->cleanup();
         }
     }

    if (m_pRunningScene)
    {
        m_pRunningScene->release();
    }
    m_pRunningScene = m_pNextScene;
    m_pNextScene->retain();
    m_pNextScene = NULL;

	if(m_pRunningScene)
		m_bIsXScene = dynamic_cast<XScene*>(m_pRunningScene) != NULL;

    if ((! runningIsTransition) && m_pRunningScene)
    {
        m_pRunningScene->onEnter();
        m_pRunningScene->onEnterTransitionDidFinish();
    }
	XFUNC_END();
}

// Draw the Scene
void XDirector::drawScene(void)
{
	XFUNC_START();
    // calculate "global" dt
    calculateDeltaTime();

    //tick before glClear: issue #533
    if (! m_bPaused)
    {
        m_pScheduler->update(m_fDeltaTime);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0x00, 0x00, 0x00, 0x00);

    /* to avoid flickr, nextScene MUST be here: after tick and before draw.
     XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
    if (m_pNextScene)
    {
        setNextScene();
    }

    kmGLPushMatrix();

    // draw the scene
    if (m_pRunningScene)
    {
        m_pRunningScene->visit();
    }

    // draw the notifications node
    if (m_pNotificationNode)
    {
        m_pNotificationNode->visit();
    }

	// Show Fps
	if(m_bDisplayStats)
	{
		if(m_byShowDebug && m_pLabel)
		{
			if(eShowFps == m_byShowDebug)
				showStats();
			else
			{
				// Draw Debug Info
				CCSize stLblSize = m_pLabel->getTexture()->getContentSizeInPixels();
				ccDrawSolidRect(ccp(0.f, 0.f),
					ccp(SCR_W, SCR_H),
					ccc4f(0.f, 0.0f, 0.f, 0.4f));
				m_pLabel->visit();
			}
		}
		else
			showStats();
	}

    kmGLPopMatrix();

    m_uTotalFrames++;

    // swap buffers
    if (m_pobOpenGLView)
    {
        m_pobOpenGLView->swapBuffers();
    }
    
    if (m_bDisplayStats)
    {
        calculateMPF();
    }
	XFUNC_END();
}

void XDirector::end(void)
{
	//XLoader::Instance().End();
#ifdef X_EFFICIENCY
	OGDP::XEfficiency::Instance().Log();
#endif
	CCDirector::end();
}

void XDirector::jumpToUpdateScene()
{
	XScene* pScene = dynamic_cast<XScene*>(m_pRunningScene);
	if (pScene)
	{
		SplashScene* pSScene = (SplashScene*)pScene->getRunLayer();
		if (pSScene)
		{
			pSScene->jumpSenen();
		}	
	}
}


NS_CC_END