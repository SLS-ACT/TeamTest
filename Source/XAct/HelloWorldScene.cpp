#include "HelloWorldScene.h"
#include "Game/XGameState.h"
#include "Game/GameScript.h"
#include "XUI/XWindowType.h"
#include "CommonFile.h"
#include "Role/XEffect.h"
#include "Role/XRole.h"
#include "XCamera/XCamera.h"
#include "XControl/XJSCtrl.h"
#include "XControl/XJSGesture.h"
#include "XNumEffect/XNumEffect.h"
#include "XNumEffect/XComboEffect.h"
#include "XNet/XProtocol.h"
#include "XNet/XNetWork.h"
#include "XConfig.h"
#include "Cocos2dx/XWeather.h"
#include "Cocos2dx/XLoader/XLoader.h"
#include "XUI/XFont/XFont.h"
#include "XHelper/XName.h"
#include "Update/XPackage.h"

#ifdef X_SDK_SWITCH
#include "SDK/XSdkManager.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <sys/types.h>
#include <unistd.h>
#endif
lua_Alloc _pfnLuaAlloc = NULL;

XCPP_DEFINED();

USING_NS_CC;

extern unsigned char g_byReload;
extern unsigned char g_byUpdate;
extern unsigned char g_byMI;

cocos2d::XSprite* HelloWorld::m_pFrame = NULL;
cocos2d::XSprite* HelloWorld::m_pLoading = NULL;

extern "C" void * lua_MyAlloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
	if(!nsize && XMemLog::m_dwLua >= osize)
		XMemLog::m_dwLua -= osize;
	XMemLog::m_dwLua += nsize;
	return _pfnLuaAlloc(ud, ptr, osize, nsize);
}

int lua_GetName(lua_State* pLS)
{
	static char szName[21];
	OGDP::XName::GetNameGB(szName);
	lua_pushstring(pLS, szName);
	return 1;
}

int lua_Utf8ToGB2312(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_Utf8ToGB2312] param must be 1!");
	const char* szUtf = lua_tostring(pLS, 1);
	//LuaDump(pLS);
	const char* szGB = _stXStrCnvMainTh.XUTF8_TO_GB2312(szUtf);
	lua_pushstring(pLS, szGB);
	return 1;
}

int lua_GB2312ToUtf8(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_GB2312ToUtf8] param must be 1!");
	const char* szGB = lua_tostring(pLS, 1);
	const char* szUtf = _stXStrCnvMainTh.XGB2312_TO_UTF8(szGB);
	lua_pushstring(pLS, szUtf);
	return 1;
}

int lua_XGB2312_TO_UNICODE(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_XGB2312_TO_UNICODE] param must be 1!");
	const char* szGB = lua_tostring(pLS, 1);
	const unsigned short* szUni = (const unsigned short*)szGB;
	if(szUni && szUni[0] != '\0')
	{
		int nLen = OGDP::xstr_wcslen(szUni) + 1;
		lua_pushlstring(pLS,(const char*)szGB, (nLen << 1));
		return 1;
	}
	return 0;
}
int lua_XUNICODE_TO_GB2312(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_XUNICODE_TO_GB2312] param must be 1!");
	size_t nLenth = 0;
	const char* szGB = lua_tolstring(pLS, 1, &nLenth);
	const char* szUtf = _stXStrCnvMainTh.XUNICODE_TO_GB2312(szGB);
	lua_pushstring(pLS, szUtf);
	return 1;
}
int lua_XUTF8_TO_UNICODE(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_XUTF8_TO_UNICODE] param must be 1!");
	const char* szGB = lua_tostring(pLS, 1);
	const char* szUtf = _stXStrCnvMainTh.XUTF8_TO_UNICODE(szGB);
	const unsigned short* szUni = (const unsigned short*)szUtf;
	if(szUni && szUni[0] != '\0')
	{
		int nLen = OGDP::xstr_wcslen(szUni) + 1;
		lua_pushlstring(pLS,(const char*)szUtf, (nLen << 1));
		return 1;
	}
	return 0;
}
int lua_XUNICODE_TO_UTF8(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
		RETURN_MSG(0, "<-- ERROR -->[lua_XUNICODE_TO_UTF8] param must be 1!");
	//unsigned int nLenth = 0;
	size_t nLenth = 0;
	const char* szGB = lua_tolstring(pLS, 1, &nLenth);
	const char* szUtf = _stXStrCnvMainTh.XUNICODE_TO_UTF8(szGB);
	lua_pushstring(pLS, szUtf);
	return 1;
}

int GetUnicodeLenth(lua_State* pLS)
{
	int nTop = lua_gettop(pLS);
	if (1 != nTop)
	{
		CCLOG("<-- ERROR -->[GetUnicodeLenth] param must be 1!");
		return 0;
	}
	size_t nLenth = 0;
	const char* szUin = lua_tolstring(pLS, 1, &nLenth);
	lua_pushinteger(pLS, nLenth);
	return 1;
}

int DivideExactly(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(2 != lua_gettop(tolua_S))
	{
		CCLOG("[DivideExactly] lua param must be 2 !");
		return 0;
	}
	int nNum1 = (int)lua_tointeger(tolua_S, -1);
	int nNum2 = (int)lua_tointeger(tolua_S, -2);
	int nRes = 0;
	if(nNum1 != 0)
		nRes = nNum2 / nNum1;
	lua_pushinteger(tolua_S, nRes);
	return 1;
}

int DebugInfo(lua_State* tolua_S)
{
	static unsigned char byShow = cocos2d::XDirector::eShowNone;
	++byShow;
	if(byShow >= cocos2d::XDirector::eShowNum)
		byShow = cocos2d::XDirector::eShowNone;
	cocos2d::XDirector::ShowDebug(byShow);
	return 0;
}
#define IZ_STATE_NUM			(512)
unsigned char m_abyIZState[IZ_STATE_NUM];

int SetIZStateBits(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(2 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】 lua param must be 1 !", __XFUNCNAME__);
		return 0;
	}
	memset(m_abyIZState, 0, sizeof(unsigned char) * IZ_STATE_NUM);
	int nNum = lua_tointeger(tolua_S, -2);
	if(nNum > IZ_STATE_NUM)
	{
		nNum = IZ_STATE_NUM;
	}
	for (int nIndex = 0; nIndex < nNum; ++nIndex)
	{
		lua_rawgeti(tolua_S, -1, nIndex);
		m_abyIZState[nIndex] = (unsigned char)lua_tointeger(tolua_S, -1);
		lua_pop(tolua_S, 1);
	}
	return 0;
}

int MarkIZStateBit(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(2 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】lua param must be 1!", __XFUNCNAME__);
		return 0;
	}
	unsigned char byPos = (unsigned char)lua_tointeger(tolua_S, -2);
	unsigned char byBitPos = (unsigned char)lua_tointeger(tolua_S, -1);
	if(byPos >= IZ_STATE_NUM)
	{
		CCLOG("【%s】lua byPos is over!", __XFUNCNAME__);
		return 0;
	}
	if(byBitPos >= 8)
	{
		CCLOG("【%s】lua byBitPos is over!", __XFUNCNAME__);
		return 0;	
	}
	XBIT_MARK(&m_abyIZState[byPos], byBitPos);
	return 0;
}

int IZStateBitValue(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(2 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】lua param must be 1!", __XFUNCNAME__);
		return 0;
	}
	unsigned char byPos = (unsigned char)lua_tointeger(tolua_S, -2);
	unsigned char byBitPos = (unsigned char)lua_tointeger(tolua_S, -1);
	if(byPos >= IZ_STATE_NUM)
	{
		CCLOG("【%s】lua byPos is over!", __XFUNCNAME__);
		return 0;
	}
	if(byBitPos >= 8)
	{
		CCLOG("【%s】lua byBitPos is over!", __XFUNCNAME__);
		return 0;
	}
	int nValue = (int)XBIT_VALUE(&m_abyIZState[byPos], byBitPos);
	lua_pushinteger(tolua_S, nValue);
	return 1;
}

#define MAX_SHOP_ITEM			(64)
unsigned char m_abyShop[MAX_SHOP_ITEM];

int SetShopBits(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】 lua param must be 1 !", __XFUNCNAME__);
		return 0;
	}
	memset(m_abyShop, 0, sizeof(unsigned char) * MAX_SHOP_ITEM);
	for (unsigned char byIndex = 0; byIndex < MAX_SHOP_ITEM; ++byIndex)
	{
		lua_rawgeti(tolua_S, -1, byIndex);
		m_abyShop[byIndex] = (unsigned char)lua_tointeger(tolua_S, -1);
		lua_pop(tolua_S, 1);
	}
	return 0;
}

int MarkShopBit(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】lua param must be 1!", __XFUNCNAME__);
		return 0;
	}
	unsigned char byPos = (unsigned char)lua_tointeger(tolua_S, -1);
	if(byPos >= MAX_SHOP_ITEM)
	{
		CCLOG("【%s】lua byIndex is over!", __XFUNCNAME__);
		return 0;
	}
	XBIT_MARK(m_abyShop, byPos);
	return 0;
}

int ShopBitValue(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("【%s】lua param must be 1!", __XFUNCNAME__);
		return 0;
	}
	unsigned char byPos = (unsigned char)lua_tointeger(tolua_S, -1);
	if(byPos >= MAX_SHOP_ITEM)
	{
		CCLOG("【%s】lua byIndex is over!", __XFUNCNAME__);
		return 0;
	}
	lua_pushinteger(tolua_S, (int)XBIT_VALUE(m_abyShop, byPos));
	return 1;
}

#define  HUILD_NUM	(16)
unsigned char g_byGuildMax = 0;
unsigned char g_abyGuild[HUILD_NUM];
int SetGuildState(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[SetGuildState] lua param must be 1!");
		return 0;
	}
	memset(g_abyGuild, 0, sizeof(unsigned char) * HUILD_NUM);
	for (int nIndex = 0; nIndex < HUILD_NUM; ++nIndex)
	{
		lua_rawgeti(tolua_S, 1, nIndex);
		unsigned char byState = (unsigned char)lua_tointeger(tolua_S, -1);
		g_abyGuild[nIndex] = byState;
		lua_pop(tolua_S, 1);		
	}
	return 0;
}
int SetGuildMax(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[SetGuildMax] lua param must be 1!");
		return 0;
	}
	g_byGuildMax = (unsigned char)lua_tointeger(tolua_S, 1);

	return 0;
}
int MarkGuild(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[MarkGuild] lua param must be 1!");
		return 0;
	}
	int nPos = lua_tointeger(tolua_S, 1);
	if(nPos >= g_byGuildMax)
	{
		CCLOG("[MarkGuild] pos is over!");
		return 0;
	}
	XBIT_MARK(g_abyGuild, nPos);
	return 0;
}

int GuildValue(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[GuildValue] lua param must be 1!");
		return 0;
	}
	int nPos = lua_tointeger(tolua_S, 1);
	if(nPos >= g_byGuildMax)
	{
		CCLOG("[GuildValue] pos is over!");
		return 0;
	}
	bool bState = false;
	if(XBIT_VALUE(g_abyGuild, nPos))
		bState = true;

	lua_pushboolean(tolua_S, bState);
	return 1;
}

int ClearGuild(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[ClearGuild] lua param must be 1!");
		return 0;
	}
	int nPos = lua_tointeger(tolua_S, 1);
	if(nPos >= g_byGuildMax)
	{
		CCLOG("[ClearGuild] pos is over!");
		return 0;
	}
	XBIT_CLR(g_abyGuild, nPos);
	return 0;
}

int OnExitGame(lua_State* tolua_S)
{
	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	XGameStateManger::SetNextFrameExitGame();
	//#endif
	return 0;
}

int OpenBloom(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(2 != lua_gettop(tolua_S))
	{
		CCLOG("[OpenBloom] lua param must be 2!");
		return 0;
	}
	int nOpen = lua_tointeger(tolua_S, 1);
	int nParam = lua_tointeger(tolua_S, 2);
	if(nOpen)
	{
		cocos2d::XScrSprite::SetShaderType(cocos2d::XScrSprite::eShaderBloom);
		cocos2d::XScrSprite::SetShaderParam((unsigned char)nParam);
	}
	else
	{
		cocos2d::XScrSprite::SetShaderType(cocos2d::XScrSprite::eShaderNone);
		cocos2d::XScrSprite::SetShaderParam(0);
	}
	return 0;
}

int ChangeRGBState(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(g_byRGBA4444)
	{
		g_byRGBA4444 = 0x00;
		CCLOG("ChangeRGBState %d", g_byRGBA4444);
		return 0;
	}
	if(!g_byRGBA4444)
	{
		g_byRGBA4444 = 0xFF;
		CCLOG("ChangeRGBState %d", g_byRGBA4444);
		return 0;
	}
	return 0;
}

int XLog(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[XLog] lua param must be 1!");
		return 0;
	}
	const char* szText = lua_tostring(tolua_S, 1);
	if(szText)
	{
		CCLOG(szText);
	}
	return 0;
}

int ShowUILine(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	if(g_ShowUILine)
	{
		g_ShowUILine = 0x00;
		return 0;
	}
	if(!g_ShowUILine)
	{
		g_ShowUILine = 0xFF;
		return 0;
	}
	return 0;
}

// 充值
int SendCharge(lua_State* tolua_S)
{
#ifdef X_SDK_SWITCH
	if(1 != lua_gettop(tolua_S))
	{
		CCLOG("[SendCharge] lua param must be 1!");
		return 0;
	}
	int nId = lua_tointeger(tolua_S, 1); 
	XSdkManager::SendCharge(nId);
	XEventManger::ExecuteEvent(XCommonEvent::CHARGE_EVENT, 0);
#endif
	return 0;
}

// 反馈登录信息
int GetSeverListInfo(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
#ifdef X_SDK_SWITCH
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(2 != lua_gettop(tolua_S))
		{
			CCLOG("[GetSeverListInfo] lua param must be 2!");
			return 0;
		}
		int nServerId = lua_tointeger(tolua_S, 1);
		const char* szServerName = lua_tostring(tolua_S, 2);
		if(2 != g_wChannelNum)
			XSdkManager::SetZoneID(nServerId);
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		if(2 != lua_gettop(tolua_S))
		{
			CCLOG("[GetSeverListInfo] lua param must be 2!");
			return 0;
		}
		int nServerId = lua_tointeger(tolua_S, 1);
		const char* szServerName = lua_tostring(tolua_S, 2);
		if(2 != g_wChannelNum)
			XSdkManager::SetZoneID(nServerId);
	#endif
#endif
	return 0;
}

// 是否已掉线
int IsOffLine(lua_State* tolua_S)
{
	if(!tolua_S)
		return 0;
	lua_pushinteger(tolua_S, g_byOffLine);
	return 1;
}

// 启动SDK
int StartSDK(lua_State* tolua_S)
{
#ifdef X_SDK_SWITCH
	XSdkManager::StartSDKLogin();
#endif
	return 0;
}

XScene* HelloWorld::scene()
{
	XFUNC_START();
    // 'scene' is an autorelease object
    XScene *scene = XScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    // add layer as a child to scene
	scene->addChild(layer);

    // return the scene
	return scene;
	XFUNC_END_RETURN(XScene*, ptr, NULL);
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern unsigned char g_byLoadPerc;
void HelloWorld::keyBackClicked(void)
{
	if(0xFF == g_byLoadPerc && !XUIManger::GetGuild())
	{
		XEventManger::ExecuteEvent(XCommonEvent::EXIT_GAME, 0);
	}
}
void HelloWorld::keyMenuClicked(void)
{
	if(0xFF == g_byLoadPerc && !XUIManger::GetGuild())
	{
		XEventManger::ExecuteEvent(XCommonEvent::SHOW_SETTING, 0);
	}
}
#endif

HelloWorld *HelloWorld::create()
{
	XFUNC_START();
	HelloWorld *pRet = new HelloWorld();
	if (pRet && pRet->init())
	{
		g_byUpdate = 0x00;
		pRet->setTouchEnabled(true);
		pRet->setTouchMode(kCCTouchesOneByOne);
		pRet->autorelease();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		XGameStateManger::m_pHelloWorld = pRet;
#endif
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
	XFUNC_END_RETURN(HelloWorld*, ptr, NULL);
}

unsigned long HelloWorld::OnFrame()
{
	XFUNC_START();
	m_stCheckLock.Lock();
	XMemLog::m_byCPU = m_byCPU;
	XMemLog::m_dwMemPhy = m_dwMemPhy;
	XMemLog::m_dwMemVtl = m_dwMemVtl;
	m_stCheckLock.Unlock();
	XMemLog::m_dwTex = cocos2d::CCTexture2D::_dwTexMem;
	XMemLog::m_dwUpdate += cocos2d::CCTexture2D::_dwUpdate;
	cocos2d::CCTexture2D::_dwUpdate = 0;
	// time
	float fDeltaTime = XDirector::Instance().getDeltaTime();
	// update
	XGameStateManger::OnFrame();
	// net
	XNetWork::Instance().Run();
	// reload
	if(XFontManager::Instance().NeedReload())
		XFontManager::Instance().OnReload();
	if(0xFF == g_byReload)
	{
		g_byReload = 0x00;
		XShader::ReloadAll();
		XScrSprite::Reload();
		XSpriteFrameCache::Instance().Reload();
		XResLoader::Instance().Reload();
		XFontManager::Instance().Reload();
		XWeather::Instance().Reload();
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		m_dwReloadSt = XResLoader::_dwReTotal;
		m_fPerc = 0.f;
		m_pFrame = cocos2d::XSprite::create("Core/frame.png");
		if(m_pFrame)
		{
			m_pFrame->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pFrame->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
			m_pFrame->setOpacity(0xFF);
			m_pFrame->setScale(1.5f);
		}
		m_pLoading = cocos2d::XSprite::create("Core/loading.png");
		if(m_pLoading)
		{
			m_pLoading->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pLoading->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
			m_pLoading->setOpacity(0xFF);
			m_pLoading->setScale(1.5f);
		}
		m_fRotation = 0.f;
		m_pPerc = cocos2d::CCLabelTTF::create(
			"0%%",
			"黑体",
			XFontData::obyFontSize);
		if(m_pPerc)
		{
			m_pPerc->retain();
			m_pPerc->setColor(ccc3(0x00, 0xFF, 0x00));
			m_pPerc->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pPerc->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
			m_pPerc->setOpacity(0xFF);
		}
		m_pInfo = cocos2d::CCLabelTTF::create(
			_stXStrCnvMainTh.XGB2312_TO_UTF8("资源准备中"),
			"黑体",
			XFontData::obyFontSize);
		if(m_pInfo)
		{
			m_pInfo->retain();
			m_pInfo->setColor(ccc3(0x00, 0xFF, 0x00));
			m_pInfo->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pInfo->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1) - 48));
			m_pInfo->setOpacity(0xFF);
		}
#		endif
	}
	if(m_dwReloadSt)
	{
		float fPerc = 0.f;
		if(XResLoader::_dwReloaded >= XResLoader::_dwReTotal)
			fPerc = 100.f;
		else
		{
			unsigned long dwLoaded = XResLoader::_dwReloaded;
			if(dwLoaded < m_dwReloadSt)
				m_dwReloadSt = dwLoaded;
			fPerc = (float)(dwLoaded - m_dwReloadSt) / (float)(XResLoader::_dwReTotal - m_dwReloadSt) * 100.f;
		}
		m_fPerc = (m_fPerc + fPerc) * 0.5f;
		if(100.f <= m_fPerc)
		{
			m_fPerc = 100.f;
			m_dwReloadSt = 0;
		}
		if(m_pPerc)
		{
			static unsigned long dwLastPerc = 0xFFFFFFFF;
			unsigned long dwPerc = (unsigned long)m_fPerc;
			if(dwLastPerc != dwPerc)
			{
				dwLastPerc = dwPerc;
				char szPerc[32];
				sprintf(szPerc, "%d%%", dwPerc);
				m_pPerc->setString(szPerc);
			}
		}
	}
	else
	{
		if(m_pInfo)
		{
			m_pInfo->release();
			m_pInfo = NULL;
		}
		if(m_pPerc)
		{
			m_pPerc->release();
			m_pPerc = NULL;
		}
		if(m_pLoading)
		{
			m_pLoading->cleanup();
			m_pLoading = NULL;
		}
		if(m_pFrame)
		{
			m_pFrame->cleanup();
			m_pFrame = NULL;
		}
	}
	// XLayer
	return XLayer::OnFrame();
	XFUNC_END_RETURN(unsigned long, res, 0);
}

void HelloWorld::visit()
{
	XFUNC_START();
	if(0xFE == g_byReload)
		g_byReload = 0xFF;

	//// Effect Test
	//if(0xFF == XConfig::m_byScrEffect)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 0, 0);
	//	GLenum __error = glGetError();
	//	if(__error)
	//	{
	//		XConfig::m_byScrEffect = 0x00;
	//		CCLOG("[Screen Effect]Not support[0x%04x].", __error);
	//	}
	//	else
	//	{
	//		XConfig::m_byScrEffect = 0xFE;
	//		CCLOG("[Screen Effect]Enabled.");
	//	}
	//	return;
	//}
	// Draw
	XGameStateManger::OnDraw();
	XLayer::visit();

	static unsigned char byFuckSDK = 0x00;
	if(!byFuckSDK)
	{
		byFuckSDK = 0xFF;
		cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glLineWidth(1.f);
		cocos2d::ccDrawColor4B(0x00, 0x00, 0xFF, 0x20);
		cocos2d::ccDrawLine(ccp(0, 0), ccp(1, 0));
		cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Reload
	if(m_dwReloadSt)
	{
		ccDrawSolidRect(ccp(0.f, 0.f),
			ccp(SCR_W, SCR_H),
			ccc4f(0.f, 0.0f, 0.f, 0.75f));
		m_fRotation += 12.f;
		if(m_fRotation > 360.f)
			m_fRotation -= 360.f;
		if(m_pInfo)
			m_pInfo->visit();
		if(m_pPerc)
			m_pPerc->visit();
		if(m_pLoading)
		{
			m_pLoading->setRotation(m_fRotation);
			m_pLoading->visit();
		}
		if(m_pFrame)
			m_pFrame->visit();
	}

	//unsigned char byDisplace = rand() % 200 + 200;
	//// 闪电，饱和叠加
	//cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//XWeather::Instance().LightningTo(200, 500, 200, 100, byDisplace);
	//// 还原
	//cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	XFUNC_END();
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	bool bRes = XGameStateManger::ccTouchBegan(pTouch, pEvent);
	return bRes;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XGameStateManger::ccTouchMoved(pTouch, pEvent);
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XGameStateManger::ccTouchEnded(pTouch, pEvent);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	XFUNC_START();
	//XEC_TIMER_ST();
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
	}
	m_dwReloadSt = 0;
	m_fPerc = 0.f;
	m_pFrame = NULL;
	m_pLoading = NULL;
	m_pPerc = NULL;
	m_pInfo = NULL;
	m_fRotation = 0.f;
	XPackage::Instance().Start();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCSize visibleSize = XDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = XDirector::sharedDirector()->getVisibleOrigin();
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
#endif

	// 获取设备名
	static std::string strDevice = "";
	if(strDevice.empty())
#	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		strDevice = CCFileUtils::sharedFileUtils()->getDevice();
#	else
		strDevice = "Win or iOS";
#	endif
	char szDevice[2] = { 0, 0 };
	if(strDevice.length() >= 2)
	{
		const char* szDvc = strDevice.c_str();
		if(szDvc)
		{
			szDevice[0] = szDvc[0];
			szDevice[1] = szDvc[1];
		}
	}
	if('M' == szDevice[0] && 'I' == szDevice[1])
		g_byMI = 0xFF;

	bool bRes;

	// register lua engine
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	if(!pEngine)
		return false;
	CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
	CCLuaStack* luaStack = pEngine->getLuaStack();
	if(!luaStack)
		return false;
	lua_State* pMainStack = luaStack->getLuaState();
	if(!pMainStack)
		return false;
	_pfnLuaAlloc = lua_getallocf(pMainStack, NULL);
	lua_setallocf(pMainStack, lua_MyAlloc, NULL);
	lua_checkstack(pMainStack, 4096);
	lua_pushinteger(pMainStack, SCR_W);
	lua_setglobal(pMainStack, "SCR_W");
	lua_pushinteger(pMainStack, SCR_H);
	lua_setglobal(pMainStack, "SCR_H");
	lua_pushinteger(pMainStack, FPS);
	lua_setglobal(pMainStack, "FPS");
	lua_pushstring(pMainStack, strDevice.c_str());
	lua_setglobal(pMainStack, "Device");
	lua_register(pMainStack, "NewName", lua_GetName);
	lua_register(pMainStack, "Utf8ToGB2312", lua_Utf8ToGB2312);
	lua_register(pMainStack, "GB2312ToUtf8", lua_GB2312ToUtf8);
	lua_register(pMainStack, "GB2132ToUniCode", lua_XGB2312_TO_UNICODE);
	lua_register(pMainStack, "UniCodeToGB2312", lua_XUNICODE_TO_GB2312);
	lua_register(pMainStack, "Utf8ToUniCode", lua_XUTF8_TO_UNICODE);
	lua_register(pMainStack, "UniCodeToUtf8", lua_XUNICODE_TO_UTF8);
	lua_register(pMainStack, "DivideExactly", DivideExactly);
	lua_register(pMainStack, "DebugInfo", DebugInfo);
	lua_register(pMainStack, "SetIZStateBits", SetIZStateBits);
	lua_register(pMainStack, "MarkIZStateBit", MarkIZStateBit);
	lua_register(pMainStack, "IZStateBitValue", IZStateBitValue);
	lua_register(pMainStack, "SetShopBits", SetShopBits);
	lua_register(pMainStack, "MarkShopBit", MarkShopBit);
	lua_register(pMainStack, "ShopBitValue", ShopBitValue);
	lua_register(pMainStack, "SetGuildState", SetGuildState);
	lua_register(pMainStack, "MarkGuild", MarkGuild);
	lua_register(pMainStack, "GuildValue", GuildValue);
	lua_register(pMainStack, "ClearGuild", ClearGuild);
	lua_register(pMainStack, "SetGuildMax", SetGuildMax);
	lua_register(pMainStack, "SureExitGame", OnExitGame);
	lua_register(pMainStack, "OpenBloom", OpenBloom);
	lua_register(pMainStack, "XLog", XLog);
	lua_register(pMainStack, "ShowUILine", ShowUILine);
	lua_register(pMainStack, "SendCharge", SendCharge);
	lua_register(pMainStack, "GetSeverListInfo", GetSeverListInfo);
	lua_register(pMainStack, "IsOffLine", IsOffLine);
	lua_register(pMainStack, "GetUnicodeLenth", GetUnicodeLenth);
	lua_register(pMainStack, "ChangeRGBState", ChangeRGBState);
	lua_register(pMainStack, "StartSDK", StartSDK);

	tolua_XTable_open(pMainStack);

	// Init Table
	bRes = XTableFile::InitAllTable();
	if(!bRes)
	{
		CCLOG("XTableFile::InitAllTable failed!");
		return false;
	}
    
	// XSprite内存池必须在XSpriteBatchNode之前创建
	XSprite* pSprT = XSprite::create();
	if(pSprT)
	{
		pSprT->cleanup();
		pSprT = NULL;
	}
	//XEC_OUTRUNSEC();
	// 协议
	XProtocol::Instance().Initialize(XFILE_PROTOCOL);
	//XEC_OUTRUNSEC();
	// 状态机初始化
	XGameStateManger::Init();
	//SHOW_MEM("XGameStateManger::Init()");
	//XEC_OUTRUNSEC();
	// UI
	XWindowType::InitAllType();
	//SHOW_MEM("XWindowType::InitAllType()");
	//XEC_OUTRUNSEC();
	XCamera::Instance().SetViewSize(SCR_W, SCR_H);
	//SHOW_MEM("XCamera::Instance().SetViewSize(SCR_W, SCR_H)");
	// Animation Init
	XTexInitThread::InitAllAnimation();
	//SHOW_MEM("InitAllAnimation()");
	//XEC_OUTRUNSEC();
	XRole::Initialize();
	//SHOW_MEM("XRole::Initialize()");
	//XEC_OUTRUNSEC();
	XEffect::Initialize();
	//SHOW_MEM("XEffect::Initialize()");
	//XEC_OUTRUNSEC();
	// Joystick
	XJSControl::Instance().SetCheckBorder(0x00);
	XJSControl::Instance().SetCtrlRect(CCRectMake(30, 10, 300, (SCR_H >> 1) - 10), ccp(160, 124), 105);
	XJSControl::Instance().SetRadius(96, 128);
	XJSGesture::Instance().SetCtrlRect(CCRectMake((SCR_W >> 1), 0, (SCR_W >> 1), SCR_H), ccp(SCR_W - 166, 133), 124);
	XJSGesture::Instance().SetMoveDis(64);
	//SHOW_MEM("Joystick::Initialize()");
	//SHOW_MEM("XComboEffect::Initialize()");
	//XEC_OUTRUNSEC();
	// Net
	//bool bConn = XNetWork::Instance().ConnectToServer("127.0.0.1", 12002);
	// Effect
	// 关闭复杂特效，在手机上fps比较低
	cocos2d::XScrSprite::SetShaderType(cocos2d::XScrSprite::eShaderNone);
	cocos2d::XScrSprite::UseSimpleMode(true);
	// 禁止使用显存Copy，大部分手机不支持
	XConfig::m_byScrEffect = 0x00;
	// System Info
	m_byCPU = 0;
	m_dwMemPhy = 0;
	m_dwMemVtl = 0;
	m_stCheckThread.Start(this, &HelloWorld::XThreadCheck);

	//XEC_OUTRUNSEC();
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

HelloWorld::~HelloWorld()
{
	m_stCheckThread.End();
}

unsigned long HelloWorld::XThreadCheck(long param, volatile bool& bExit)
{
	XFUNC_START();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	int nPId = getpid();
	const int nBuffLen = 65536;
	char szSys[nBuffLen];
	unsigned char byErrNum = 0;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    float fCpu;
    float fMem;
#endif
	unsigned char byCPU;
	unsigned long dwMemPhy;
	unsigned long dwMemVtl;
	while(!bExit)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		// 获取当前进程系统信息
		//sprintf(szSys, "/system/bin/top -n 1|grep %d", nPId);
		sprintf(szSys, "/system/bin/top -n 1");
		FILE* pfSys = popen(szSys, "r");
		if(!pfSys)
			break;
		int nRead = (int)fread(szSys, sizeof(char), nBuffLen, pfSys);
		unsigned char byTryNum = 0;
		while(!nRead)
		{
			m_stCheckThread.SleepMs(1000);
			nRead = (int)fread(szSys, sizeof(char), nBuffLen, pfSys);
			++byTryNum;
			if(byTryNum >= 4)
				break;
		}
		fclose(pfSys);
		if(!nRead)
		{
			m_stCheckThread.SleepMs(1000);
			++byErrNum;
			if(byErrNum <= 99)
				continue;
			else
			{
				break;
			}
		}
		int nPos = 0;
		int nSt = 0;
		// 查询进程ID
		for(; nPos < nRead; ++nPos)
		{
			if('\r' == szSys[nPos] || '\n' == szSys[nPos] || '\0' == szSys[nPos])
			{
				for(; nSt < nPos; ++nSt)
				{
					if(' ' != szSys[nSt] && '\t' != szSys[nSt])
						break;
				}
				int nId = atoi(&szSys[nSt]);
				if(nId == nPId)
					break;
				nSt = nPos + 1;
			}
		}
		nPos = nSt;
		nSt = -1;
		// 解析CPU
		for(; nPos < nRead; ++nPos)
		{
			if(' ' == szSys[nPos] || '\t' == szSys[nPos])
				nSt = nPos;
			if('%' == szSys[nPos])
				break;
		}
		if(-1 != nSt && nPos != nRead)
			byCPU = (unsigned char)atoi(&szSys[nSt]);
		else
			break;
		++nPos;
		nSt = -1;
		// 解析虚拟内存
		for(; nPos < nRead; ++nPos)
		{
			if(' ' == szSys[nPos] || '\t' == szSys[nPos])
				nSt = nPos;
			if('K' == szSys[nPos])
				break;
		}
		if(-1 != nSt && nPos != nRead)
			dwMemVtl = (unsigned long)atol(&szSys[nSt]);
		else
			break;
		++nPos;
		nSt = -1;
		// 解析物理内存
		for(; nPos < nRead; ++nPos)
		{
			if(' ' == szSys[nPos] || '\t' == szSys[nPos])
				nSt = nPos;
			if('K' == szSys[nPos])
				break;
		}
		if(-1 != nSt && nPos != nRead)
			dwMemPhy = (unsigned long)atol(&szSys[nSt]);
		else
			break;
		m_stCheckLock.Lock();
		m_byCPU = byCPU;
		m_dwMemPhy = dwMemPhy;
		m_dwMemVtl = dwMemVtl;
		m_stCheckLock.Unlock();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCApplication::getSystemInfo(fCpu, fMem);
		m_stCheckLock.Lock();
		m_byCPU = (unsigned char)fCpu;
		m_dwMemPhy = (unsigned long)fMem;
		m_dwMemVtl = 0;
		m_stCheckLock.Unlock();
#else
		static XCPUPercentWin stCPU;
		byCPU = stCPU.GetCPU();
		dwMemPhy = GetMemoryUsed(&dwMemVtl);
		dwMemPhy >>= 10;
		dwMemVtl >>= 10;
		m_stCheckLock.Lock();
		m_byCPU = byCPU;
		m_dwMemPhy = dwMemPhy;
		m_dwMemVtl = dwMemVtl;
		m_stCheckLock.Unlock();
#endif
		m_stCheckThread.SleepMs(4000);
	}
	//m_stCheckLock.Lock();
	m_byCPU = 0;
	m_dwMemPhy = 0;
	m_dwMemVtl = 0;
	//m_stCheckLock.Unlock();
	XFUNC_END_RETURN(unsigned short, res, 0);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		XEventManger::ExecuteEvent(XCommonEvent::EXIT_GAME, 0);
	#endif
}

void HelloWorld::menuDebugCallback(CCObject* pSender)
{
	static unsigned char byShow = cocos2d::XDirector::eShowNone;
	++byShow;
	if(byShow >= cocos2d::XDirector::eShowNum)
		byShow = cocos2d::XDirector::eShowNone;
	cocos2d::XDirector::ShowDebug(byShow);
}

void HelloWorld::menuEffectCallback(CCObject* pSender)
{
	//if(!XConfig::m_byScrEffect)
	//	return;
	//XConfig::m_byScrEffect = 0x00;

	//// 效果优先的全屏特效
	//cocos2d::XScrSprite::UseSimpleMode(false);
	//unsigned char byScrShader = cocos2d::XScrSprite::GetShaderType();
	//++byScrShader;
	//if(byScrShader >= cocos2d::XScrSprite::eShaderNum)
	//	byScrShader = cocos2d::XScrSprite::eShaderNone;
	//cocos2d::XScrSprite::SetShaderType(byScrShader);

	// 依次开启关闭的全屏特效
	unsigned char byScrShader = cocos2d::XScrSprite::GetShaderType();
	//if(cocos2d::XScrSprite::eShaderNone != byScrShader)
	//{
	//	bool bScrSimple = cocos2d::XScrSprite::UseSimpleMode();
	//	if(bScrSimple)
	//	{
	//		cocos2d::XScrSprite::UseSimpleMode(false);
	//		return;
	//	}
	//}
	++byScrShader;
	if(byScrShader >= cocos2d::XScrSprite::eShaderNum)
		byScrShader = cocos2d::XScrSprite::eShaderNone;
	cocos2d::XScrSprite::SetShaderType(byScrShader);
	cocos2d::XScrSprite::UseSimpleMode(true);
}

void HelloWorld::menuCameraCallback(CCObject* pSender)
{
	// 镜头特效
	/*static unsigned char byMax = XTableFile::stTblCamera.GetMaxRowID();
	static unsigned char byNow = 0;
	XCamera::Instance().Effect(byNow++);
	if(byNow >= byMax)
	byNow = 0;*/
	if (XJoystick::m_bDrawLine)
	{
		XJoystick::m_bDrawLine = false;
	}
	else
	{
		XJoystick::m_bDrawLine = true;
	}
}

void HelloWorld::menuWeatherCallback(CCObject* pSender)
{
	// 天气特效
	static unsigned char byNow = XWeather::eSnow;
	XWeather::Instance().SetType(byNow++);
	if(byNow >= XWeather::eTypeNum)
		byNow = XWeather::eNone;
}
