#include "XGameState.h"
#include "../XUI/XUIManger.h"
#include "../XUI/Control/XProssBar.h"
#include "../XUI/Control/XLabel.h"
#include "../XUI/XFont/XFont.h"
#include "../XEvent/XEventManger.h"
#include "../Role/XRole.h"
#include "../XControl/XJSCtrl.h"
#include "../XControl/XJSGesture.h"
#include "../XConfig.h"
#include "../XCamera/XCamera.h"
#include "../XSound/XSoundManager.h"
#include "../Cocos2dx/XWeather.h"
#include "XTimeManager.h"
#include "../XNet/XProtocol.h"
#include "../Game/GameScript.h"
#include "../CommonFile.h"
#include "../Role/XRole.h"
#include "../HelloWorldScene.h"
#include "../SDK/XSdkManager.h"
#include "../XNumEffect/XNumEffect.h"
#include "../XNumEffect/XComboEffect.h"
#include "../XNet/XNetWork.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "CCLuaEngine.h"
#include "tolua_fix.h"

XCPP_DEFINED();
static unsigned char byCityEvent = 0x00;
static unsigned char byIZEvent = 0x00;
static unsigned char bySelRole = 0x00;

XBaseWindow* g_pLoading = NULL;
XProssBar*	g_pBar = NULL;
XLabel* g_pLabel = NULL;
unsigned char g_byLoadPerc = 0xFF;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	HelloWorld* XGameStateManger::m_pHelloWorld = NULL;
#endif
#ifdef X_SDK_SWITCH
volatile unsigned char XGameStateManger::m_bySDKLogin = 0x00;
volatile unsigned char XGameStateManger::m_bySDKSurface = 0xFF;
unsigned long XGameStateManger::m_dwUID = 0;
char	XGameStateManger::m_szACID[SDK_STR_NUM] = {0};
char	XGameStateManger::m_szToken[SDK_STR_NUM] = {0};
#endif /* X_SDK_SWITCH */

// 登陆
bool XLoginState::OnInit()
{
	XFUNC_START();	
#ifdef X_SDK_SWITCH
	CCLog("lihaofeng OnInit  %d", XGameStateManger::m_bySDKSurface);
	if (!XGameStateManger::m_bySDKLogin && 2 != g_wChannelNum)
	{
		return true;
	}
#endif /* X_SDK_SWITCH */
	m_byPlayMusic = 0x00;
	if(!m_pSprLogo)
	{
		m_pSprLogo = cocos2d::XSprite::create("Core/logo.jpg");
		if(m_pSprLogo)
		{
			const cocos2d::CCSize stSz = m_pSprLogo->getContentSize();
			float fSX = SCR_W / stSz.width;
			float fSY = SCR_H / stSz.height;
			m_pSprLogo->setScaleX(fSX);
			m_pSprLogo->setScaleY(fSY);
			m_pSprLogo->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pSprLogo->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
			m_pSprLogo->setOpacity(0x00);
		}
	}
	if(!m_pStrVer)
	{
		XConfigT* pCT = (XConfigT*)XTableFile::stConfig.GetRowData(200, sizeof(XConfigT));
		if(pCT)
		{
			char szVer[64];
			sprintf(szVer, "Version %d.%d.%d", pCT->valParam1, pCT->valParam2, pCT->valParam3);
			m_pStrVer = XFontManager::Instance().CreateRenderString(_stXStrCnvMainTh.XGB2312_TO_UNICODE(szVer));
		}
	}
	return true;
	XFUNC_END_RETURN(bool, res, false);
}
unsigned char XLoginState::OnFrame()
{
	XFUNC_START();
	unsigned char byState = enState_Login;
#ifdef X_SDK_SWITCH
	if (!XGameStateManger::m_bySDKLogin && 2 != g_wChannelNum)
	{
		return byState;
	}
	else
	{
		m_byPlayMusic = 0x00;
		if(!m_pSprLogo)
		{
			m_pSprLogo = cocos2d::XSprite::create("Core/logo.jpg");
			if(m_pSprLogo)
			{
				const cocos2d::CCSize stSz = m_pSprLogo->getContentSize();
				float fSX = SCR_W / stSz.width;
				float fSY = SCR_H / stSz.height;
				m_pSprLogo->setScaleX(fSX);
				m_pSprLogo->setScaleY(fSY);
				m_pSprLogo->setAnchorPoint(ccp(0.5f, 0.5f));
				m_pSprLogo->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
				m_pSprLogo->setOpacity(0x00);
			}
		}
	}
#endif /* X_SDK_SWITCH */
	if(0x01 == m_byPlayMusic)
	{
		XSoundManager::PlaySoundByID(0);
		m_byPlayMusic = 0xFF;
	}
	if(0x01 == m_byLoadScript)
	{
		unsigned char byLoaded = XProtocol::Instance()._byLoaded;
		if(0xFF == byLoaded)
			m_byLoadScript = 0xFE;
	}
	if(0xFE == m_byLoadScript)
	{
		bool bEnd = false;
		// 脚本
		if(2 == g_wChannelNum)
		{
			bEnd = GameScriptManger::Instance().LoadScript(XFILE_GAMESCRIPT_KAKAO);
		}
		else
		{
			bEnd = GameScriptManger::Instance().LoadScript(XFILE_GAMESCRIPT);
		}
		if(bEnd)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			if(XGameStateManger::m_pHelloWorld)
				XGameStateManger::m_pHelloWorld->setKeypadEnabled(true);
#endif
			XNumEffect::InitByImagePath("System/NumImage.png");
			XSupplyNum::InitByImagePath("System/Supplynumber.png");
			XNumEffect::InitCritByImagePath("System/CritNum.png", "System/Crit.png"/*, "System/CritEffect.png"*/);
			XComboEffect::Instance().InitByImagePath("System/ComboBack.png",
				"", "System/ComboNum.png");
			XRole::InitializeShodow("shadow.png");
			XJSControl::Instance().SetPointSpr("111_6.png", "111_7.png", "111_5.png");			
			m_byLoadScript = 0xFD;

			// 初始化loding相关
			if(!g_pLoading)
				g_pLoading = XUIManger::GetWindow("loading");
			if (g_pLoading)
			{
				if(!g_pBar)
				{
					g_pBar = XProssBar::ToProssBar(g_pLoading->GetChildByName("XProssBar"));
					g_pBar->SetEffectPosType(XBaseWindow::eEffectPos_Right);
					g_pBar->SetLoopEffect(26066);
				}
				if(!g_pLabel)
					g_pLabel = XLabel::ToLabel(g_pLoading->GetChildByName("XLabel"));
			}
		}
		else
			return byState;
	}

	XUIManger::OnFrame();

	return byState;

	XFUNC_END_RETURN(unsigned char, res, enState_MaxNum);
}
void XLoginState::OnDraw()
{
	XFUNC_START();
#ifdef X_SDK_SWITCH
	unsigned char byState = enState_Login;
	if(XGameStateManger::m_bySDKSurface && 2 != g_wChannelNum)
	{
		if(!m_pSprLogo)
		{
			m_pSprLogo = cocos2d::XSprite::create("Core/logo.jpg");
			if(m_pSprLogo)
			{
				const cocos2d::CCSize stSz = m_pSprLogo->getContentSize();
				float fSX = SCR_W / stSz.width;
				float fSY = SCR_H / stSz.height;
				m_pSprLogo->setScaleX(fSX);
				m_pSprLogo->setScaleY(fSY);
				m_pSprLogo->setAnchorPoint(ccp(0.5f, 0.5f));
				m_pSprLogo->setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
				m_pSprLogo->setOpacity(0x00);
			}
		}
		if(m_pSprLogo)
		{
			m_pSprLogo->visit();
		}
		XSdkManager::StartSDKLogin();
		XGameStateManger::m_bySDKSurface = 0x00;
	}
	if (!XGameStateManger::m_bySDKLogin && 2 != g_wChannelNum)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
	if (m_byLoadScript == 0xFF && !XGameStateManger::m_byLoginFlag)
	{
		if (XGameStateManger::m_bySDKLogin)
		{
			XEventManger::ExecuteEvent(XCommonEvent::EVENT_SDKLOGIN_SUC, XGameStateManger::m_szACID, XGameStateManger::m_szToken, NULL);
			//CCLog("XEventManger::ExecuteEvent m_dwUID = [%ld], line = [%d]", XGameStateManger::m_dwUID, __LINE__);
		}
		XGameStateManger::m_byLoginFlag = 0xFF;
	}
#endif /* X_SDK_SWITCH */
	if(0x00 == m_byPlayMusic)
		m_byPlayMusic = 0x01;
	unsigned char byAlpha = 0xFF;
	unsigned char byAAdd = 8;
	if(0x00 == m_byLoadScript)
	{
		if(m_pSprLogo)
		{
			byAlpha = m_pSprLogo->getOpacity();
			if(0xFF == byAlpha)
			{
				m_byLoadScript = 0x01;
			}
			if(byAlpha < 0xFF - byAAdd)
				byAlpha += byAAdd;
			else
				byAlpha = 0xFF;
			m_pSprLogo->setOpacity(byAlpha);
			m_pSprLogo->visit();
		}
		else
		{
			m_byLoadScript = 0x01;
			char szLoading[] = "Loading...";
			CCLabelTTF stLabel;
			stLabel.initWithString(szLoading, "Arial", 24);
			stLabel.setAnchorPoint(ccp(0.5f, 0.5f));
			stLabel.setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
			stLabel.setColor(ccc3(0xFF, 0xFF, 0x99));
			stLabel.visit();
		}
#	ifndef X_SINGLE
		if(m_pStrVer)
			m_pStrVer->OnDraw(40.f, SCR_H - 40.f);
#	endif
		return;
	}

	XUIManger::OnDraw(0.f, 0.f, 1.f, 0xFF);

	if(0xFE == m_byLoadScript || 0xFD == m_byLoadScript)
	{
		if(m_pSprLogo)
		{
			byAlpha = m_pSprLogo->getOpacity();
			if(0xFD == m_byLoadScript)
			{
				if(0x00 == byAlpha) 
				{
					m_byLoadScript = 0xFF;
				}
				else
				{
					if(byAlpha > byAAdd)
						byAlpha -= byAAdd;
					else
					{
						byAlpha = 0x00;
					}
				}
			}
			m_pSprLogo->setOpacity(byAlpha);
			m_pSprLogo->visit();
		}
		else
		{
			static unsigned char byPt = 0;
			static CCLabelTTF stLabel;
			if(!byPt)
			{
				byPt = 1;
				char szLoading[] = "Loading";
				stLabel.initWithString(szLoading, "Arial", 24);
				stLabel.setAnchorPoint(ccp(0.5f, 0.5f));
				stLabel.setPosition(ccp((SCR_W >> 1), (SCR_H >> 1)));
				stLabel.setColor(ccc3(0xFF, 0xFF, 0x99));
			}
			if(byPt > 45)
			{
				byPt = 1;
			}
			if(1 == byPt)
			{
				char szLoading[] = "Loading.";
				stLabel.setString(szLoading);
			}
			else if(15 == byPt)
			{
				char szLoading[] = "Loading..";
				stLabel.setString(szLoading);
			}
			else if(30 == byPt)
			{
				char szLoading[] = "Loading...";
				stLabel.setString(szLoading);
			}
			stLabel.visit();
			++byPt;
			if(byPt == 45 && 0xFD == m_byLoadScript)
				m_byLoadScript = 0xFF;
		}
	}
#	ifndef X_SINGLE
	if(m_pStrVer)
		m_pStrVer->OnDraw(40.f, SCR_H - 40.f);
#	endif
	XFUNC_END();
}
void XLoginState::OnRealse()
{
	XFUNC_START();
	if(0xFF == m_byPlayMusic)
	{
		XSoundManager::StopAllSound();
		m_byPlayMusic = 0x00;
	}
	if(m_pSprLogo)
	{
		m_pSprLogo->cleanup();
		m_pSprLogo = NULL;
	}
	XFUNC_END();
}
bool XLoginState::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	bool bRes = XUIManger::ccTouchBegan(pTouch, pEvent);
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}
void XLoginState::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	XUIManger::ccTouchMoved(pTouch, pEvent);
	XFUNC_END();
}
void XLoginState::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	XUIManger::ccTouchEnded(pTouch, pEvent);
	XFUNC_END();
}
// 选角色
bool XSelRoleState::OnInit()
{
	XFUNC_START();
	// loading相关
	g_byLoadPerc = 0x00;
	bySelRole = 0x00;
	if(!m_pRoleMap)
	{
		//初始化地图
		m_pRoleMap = XCreateRoleMap::CreateByID();
		if(!m_pRoleMap)
		{
			CCLOG("CreateMap Faild!");
			return false;
		}		
	}
	m_pRoleMap->Reset();
	for (unsigned char byCnt = 0; byCnt < XMAP_PLAYER_MAXNUM; ++byCnt)
	{
		XRole* pRole = XRole::Create();
		if (pRole)
		{
			pRole->SetRoleID(byCnt);
			pRole->SetTypeA(XMapObj::eRole);
			pRole->SetTypeB(XMapObj::ePlayer);
			pRole->SetCamp(XCamp::eCampFriend);
			m_pRoleMap->AddRole(pRole);
		}
	}
	g_pLoading->ShowWindow(true);
	return true;
	XFUNC_END_RETURN(bool, res, false);
}
unsigned char XSelRoleState::OnFrame()
{
	XFUNC_START();
	unsigned char byState = enState_SelectRole;

	if(m_pRoleMap)
	{
		if(0x00 == bySelRole && 0xFF == g_byLoadPerc)
		{
			bySelRole = 0xFF;
			XSoundManager::PlaySoundByID(5);
		}
		if(0xFF != g_byLoadPerc)
		{
			if(0x64 > g_byLoadPerc)
				g_byLoadPerc = m_pRoleMap->GetLoadPerc();
			else
				g_byLoadPerc = 0xFF;
			if(g_pBar)
				g_pBar->OnFrame();
		}
		m_pRoleMap->OnFrame();
	}
	XUIManger::OnFrame();

	return byState;
	XFUNC_END_RETURN(unsigned char, res, enState_MaxNum);
}
void XSelRoleState::OnDraw()
{
	XFUNC_START();
	if(0xFF != g_byLoadPerc)
	{
		static unsigned char byCurrLoadPerc = 0xFF;
		if (g_pLoading)
		{
			if (g_byLoadPerc != byCurrLoadPerc && g_pLabel && g_pBar)
			{
				byCurrLoadPerc = g_byLoadPerc;
				g_pBar->CalPross(g_byLoadPerc);
				static char szPerc[16];
				unsigned char byPerc = g_byLoadPerc;
				if(byPerc > 100)
				{
					byPerc = 100;
				}
				sprintf(szPerc, "%d%%", byPerc);
				g_pLabel->setText(szPerc);
			}
			g_pLoading->OnDraw(0.f, 0.f, 1.f, 0xFF);
			if (byCurrLoadPerc == 100)
			{
				byCurrLoadPerc = 0xFF;
				g_pLoading->ShowWindow(false);
			}
		}
		return;
	}
	XUIManger::OnDraw(0.f, 0.f, 1.f, 0xFF);
	if(m_pRoleMap)
		m_pRoleMap->visit();
	XFUNC_END();
}
void XSelRoleState::OnRealse()
{
	XFUNC_START();
	if(m_pRoleMap)
	{
		XSoundManager::StopAllSound();
		m_pRoleMap->ClearMapObj();
		m_pRoleMap->Reset();
	}
	XFUNC_END();
}
bool XSelRoleState::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return false;
	bool bRes = XUIManger::ccTouchBegan(pTouch, pEvent);
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}
void XSelRoleState::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchMoved(pTouch, pEvent);
	XFUNC_END();
}
void XSelRoleState::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchEnded(pTouch, pEvent);
	XFUNC_END();
}
// 城市
bool XCityState::OnInit()
{
	XFUNC_START();
	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	if(m_wCurrMapId == (unsigned short)-1)
		return false;

	// loading相关
	g_byLoadPerc = 0x00;
	byCityEvent = 0x00;

	if(!m_pCityMap)
	{
		//初始化地图
		m_pCityMap = XMap::CreateByID(m_wCurrMapId);
		if(!m_pCityMap)
		{
			CCLOG("CreateMap Faild!");
			return false;
		}
		//SHOW_MEM("XMap::CreateByID(m_wCurrMapId)");
	}
	else
	{
		m_pCityMap->InitByID(m_wCurrMapId);
		if(!m_pCityMap)
		{
			CCLOG("CreateMap Faild!");
			return false;
		}
	}


	if(m_pCityMap)
	{
		// 从副本中拿出玩家,放到City
		XGameSateBase* pState = XGameStateManger::GetGameState(enState_InstZone);
		if(pState)
		{
			XInstZoneState* pIZState = (XInstZoneState*)pState;
			if(pIZState)
			{
				XIZMap* pMap = pIZState->GetMap();
				if(pMap)
				{
					float fCityPosX, fCityPosY, fCityPosH;
					pIZState->GetRoleCityPos(fCityPosX, fCityPosY, fCityPosH);
					XRole* pRole = pMap->GetControl();
					if(pRole)
					{
						pRole->Wait(true);
						pMap->DelMapObj(pRole);
						pRole->Release();
						m_pCityMap->AddMapObj(pRole);
						m_pCityMap->SetControl(pRole);
						pRole->SetHp(pRole->GetMaxHp());
						pRole->SetMp(pRole->GetMaxMp());
						pRole->SetFlipX(false);
						pRole->ResetPAI(0x00);
						pRole->SetPosition(fCityPosX, fCityPosY, fCityPosH);
						m_fPosX = fCityPosX;
						m_fPosY = fCityPosY;
						XRole* pElfAtt = pRole->m_pElfAtt;
						if (pElfAtt)
						{
							pElfAtt->Wait(true);
							pMap->DelMapObj(pElfAtt);
							//pElfAtt->Release();
							m_pCityMap->AddMapObj(pElfAtt);
							pElfAtt->SetPosition(fCityPosX, fCityPosY, fCityPosH);
							pElfAtt->SetFlipX(false);
						}
						XRole* pElfDef = pRole->m_pElfDef;
						if (pElfDef)
						{
							pElfDef->Wait(true);
							pMap->DelMapObj(pElfDef);
							//pElfDef->Release();
							m_pCityMap->AddMapObj(pElfDef);
							pElfDef->SetPosition(fCityPosX, fCityPosY, fCityPosH);
							pElfDef->SetFlipX(false);
						}
					}
				}
			}
		}
	}
	else
	{
		m_fPosX = 0.f;
		m_fPosY = 0.f;
	}
	XUIManger::SetTouchBegin(0x00);
	XMap::SetMapState(0x00);

	g_pLoading->ShowWindow(true);
	unsigned char byWeatherID = m_pCityMap->GetWeatherID();
	XWeather::Instance().SetType(byWeatherID);
	XMapT* pMT = m_pCityMap->GetMapT();
	if(pMT)
		XWeather::Instance().Lightning(pMT->valLightning);
	//cocos2d::XTextureCache::Instance().dumpCachedTextureInfo();
	//XDrawTex::DumpUsedInfo();
	return true;
	XFUNC_END_RETURN(bool, res, false);
}
unsigned char XCityState::OnFrame()
{
	XFUNC_START();
	unsigned char byState = enState_City;
	if(m_pCityMap)
	{
		if(0x00 == byCityEvent && 0xFF == g_byLoadPerc)
		{
			XEventManger::ExecuteEvent(XCommonEvent::CITYENTER, 0);
			byCityEvent = 0xFF;
			// 回到主城，回满HP,MP
			XRole* pRole = XJoystick::m_opCtrlRole;
			if (pRole)
			{
				pRole->SetHp(pRole->GetMaxHp());
				pRole->SetMp(pRole->GetMaxMp());
			}
			unsigned short wSoundID = m_pCityMap->GetMapSoundID();
			if (wSoundID != (unsigned short)-1)
			{
				XSoundManager::PlaySoundByID(wSoundID);
			}
		}
		if(0x64 > g_byLoadPerc)
			g_byLoadPerc = m_pCityMap->GetLoadPerc();
		else
			g_byLoadPerc = 0xFF;
		if(g_pBar)
			g_pBar->OnFrame();

		m_pCityMap->OnFrame();

		unsigned long dwTick = XConfig::Instance().GetTickCount();

		// 每2秒发送一次位置给服务器
		if(!g_byOffLine)
		{
			static unsigned long _dwPosTick = 0;
			if(_dwPosTick + 2000 < dwTick)
			{
				if(m_pCityMap)
				{
					XRole* pRole = m_pCityMap->GetControl();
					if(pRole)
					{
						float fPosX, fPosY, fPosH;
						pRole->GetPosition(fPosX, fPosY, fPosH);
						if(m_fPosX != fPosX || m_fPosY != fPosY)
						{
							m_fPosX = fPosX;
							m_fPosY = fPosY;
							// 位置是否改变，改变才发
							XEventManger::ExecuteEvent(XCommonEvent::CITYPOS_REQ, 0);
						}
					}
				}
				_dwPosTick = dwTick;
			}
		}
	}
	XUIManger::OnFrame();
	return byState;
	XFUNC_END_RETURN(unsigned char, res, enState_MaxNum);
}
void XCityState::OnDraw()
{
	XFUNC_START();
	if(0xFF != g_byLoadPerc)
	{
		static unsigned char byCurrLoadPerc = 0xFF;
		if (g_pLoading)
		{
			if (g_byLoadPerc != byCurrLoadPerc && g_pLabel && g_pBar)
			{
				byCurrLoadPerc = g_byLoadPerc;
				g_pBar->CalPross(g_byLoadPerc);
				static char szPerc[16];
				unsigned char byPerc = g_byLoadPerc;
				if(byPerc > 100)
				{
					byPerc = 100;
				}
				sprintf(szPerc, "%d%%", byPerc);
				g_pLabel->setText(szPerc);
			}
			g_pLoading->OnDraw(0.f, 0.f, 1.f, 0xFF);
			if (byCurrLoadPerc == 100)
			{
				byCurrLoadPerc = 0xFF;
				g_pLoading->ShowWindow(false);
			}
		}
		return;
	}
	if(m_pCityMap)
	{
		m_pCityMap->visit();
	}
	XUIManger::OnDraw(0.f, 0.f, 1.f, 0xFF);
	XFUNC_END();
}
void XCityState::OnRealse()
{
	XFUNC_START();
	if(m_pCityMap)
	{
		unsigned short wSoundID = m_pCityMap->GetMapSoundID();
		if (wSoundID != (unsigned short)-1)
		{
			XSoundManager::StopAllSound();
		}
		m_pCityMap->ClearMapObj();
		m_pCityMap->Reset();
	}
	XFUNC_END();
}
bool XCityState::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return false;
	bool bRes = XUIManger::ccTouchBegan(pTouch, pEvent);
	if(!bRes)
	{
		if(m_pCityMap)
			bRes = m_pCityMap->ccTouchBegan(pTouch, pEvent);
	}
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}
void XCityState::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchMoved(pTouch, pEvent);
	if(m_pCityMap)
		m_pCityMap->ccTouchMoved(pTouch, pEvent);
	XFUNC_END();
}
void XCityState::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchEnded(pTouch, pEvent);
	if(m_pCityMap)
		m_pCityMap->ccTouchEnded(pTouch, pEvent);
	XFUNC_END();
}
// 副本
bool XInstZoneState::OnInit()
{
	XFUNC_START();
	XInstZonesT* pIzT = (XInstZonesT*)XTableFile::stInstZones.GetRowData(m_wCurrIZId, sizeof(XInstZonesT));
	if(!pIzT)
	{
		CCLOG("IZ id [%d] not exit!", m_wCurrIZId);
		return false;
	}

	// loading 相关
	g_byLoadPerc = 0x00;
	byIZEvent = 0x00;

	if(!m_pIZMap)
	{
		//初始化地图
		m_pIZMap = XIZMap::CreateByID(pIzT->valMapID, m_wCurrIZId);
		if(!m_pIZMap)
		{
			CCLOG("XIZMap::CreateByID Faild!");
			return false;
		}
		SHOW_MEM("m_pIZMap::CreateByID(0)");
	}
	else
	{
		m_pIZMap->SetIZId(m_wCurrIZId);
		m_pIZMap->InitByID(pIzT->valMapID);
	}
	if(m_pIZMap)
	{
		// 把玩家放到副本中
		XGameSateBase* pState = XGameStateManger::GetGameState(enState_City);
		if(pState)
		{
			XCityState* pCityState = (XCityState*)pState;
			if(pCityState)
			{
				XMap* pMap = pCityState->GetMap();
				if(pMap)
				{
					XRole* pRole = pMap->GetControl();
					if(pRole)
					{
						pRole->Wait(true);
						XJoystick::SetCtrlRole(pRole);
						pMap->DelMapObj(pRole);
						pRole->Release();
						pRole->SetHp(pRole->GetMaxHp());
						pRole->SetMp(pRole->GetMaxMp());
						m_pIZMap->AddMapObj(pRole);
						pRole->GetPosition(m_fCityPosX, m_fCityPosY, m_fCityPosH);
						pRole->SetPosition((float)pIzT->valBornX, (float)pIzT->valBornY, 0.f);
						m_pIZMap->SetControl(pRole);
						pRole->SetFlipX(false);

						XRole* pElfAtt = pRole->m_pElfAtt;
						if (pElfAtt)
						{
							pElfAtt->Wait(true);
							pMap->DelMapObj(pElfAtt);
							m_pIZMap->AddMapObj(pElfAtt);
							pElfAtt->SetPosition((float)pIzT->valBornX + 100, (float)pIzT->valBornY, 0.f);
							pElfAtt->SetFlipX(false);
						}
						XRole* pElfDef = pRole->m_pElfDef;
						if (pElfDef)
						{
							pElfDef->Wait(true);
							pMap->DelMapObj(pElfDef);
							m_pIZMap->AddMapObj(pElfDef);
							pElfDef->SetPosition((float)pIzT->valBornX - 100, (float)pIzT->valBornY, 0.f);
							pElfDef->SetFlipX(false);
						}
					}
				}
			}
		}
		if(3 == pIzT->valType)
		{
			//m_pIZMap->m_bySpecial = 0xFF;
			m_pIZMap->m_byArena = 0xFF;
		}
		if (4 == pIzT->valType)
		{
			m_pIZMap->m_bySpecial = 0xFF;
			m_pIZMap->m_byActivity = 0xFF;
		}
		if (5 == pIzT->valType)
		{
			m_pIZMap->m_byActivity = 0xFF;
		}
	}
	XUIManger::SetTouchBegin(0x00);
	XMap::SetMapState(0x00);
	g_pLoading->ShowWindow(true);
	unsigned char byWeatherID = m_pIZMap->GetWeatherID();
	XWeather::Instance().SetType(byWeatherID);
	XMapT* pMT = m_pIZMap->GetMapT();
	if(pMT)
		XWeather::Instance().Lightning(pMT->valLightning);
	return true;
	XFUNC_END_RETURN(bool, res, false);
}
unsigned char XInstZoneState::OnFrame()
{
	XFUNC_START();
	unsigned char byState = enState_InstZone;
	if(m_pIZMap)
	{
		if(0x00 == byIZEvent && 0xFF == g_byLoadPerc)
		{
			XEventManger::ExecuteEvent(XCommonEvent::CITYIZMAP, 0);
			byIZEvent = 0xFF;
			m_pIZMap->SetIZMapBegin();
			unsigned short wSoundID = m_pIZMap->GetMapSoundID();
			if (wSoundID != (unsigned short)-1)
			{
				XSoundManager::PlaySoundByID(wSoundID);
			}
		}

		if(0x64 > g_byLoadPerc)
			g_byLoadPerc = m_pIZMap->GetLoadPerc();
		else if(0xFF != g_byLoadPerc)
		{
			// 加载第二屏
			m_pIZMap->LoadNextStageRes();
			g_byLoadPerc = 0xFF;
		}
		if(g_pBar)
			g_pBar->OnFrame();

		unsigned long nIZState = m_pIZMap->OnFrame();
		if(nIZState <= enState_MaxNum && byState != nIZState)
		{
			byState = (unsigned char)nIZState;
		}
	}

	XUIManger::OnFrame();
	if (XJoystick::m_byKeyCache != 0xFF)
	{
		XJSGesture::Instance().UpdateRole(false, 0xFF);
	}
	return byState;
	XFUNC_END_RETURN(unsigned char, res, enState_MaxNum);
}
void XInstZoneState::OnDraw()
{
	XFUNC_START();
	if(0xFF != g_byLoadPerc)
	{
		static unsigned char byCurrLoadPerc = 0xFF;
		if (g_pLoading)
		{
			if (g_byLoadPerc != byCurrLoadPerc && g_pLabel && g_pBar)
			{
				byCurrLoadPerc = g_byLoadPerc;
				g_pBar->CalPross(g_byLoadPerc);
				static char szPerc[16];
				unsigned char byPerc = g_byLoadPerc;
				if(byPerc > 100)
				{
					byPerc = 100;
				}
				sprintf(szPerc, "%d%%", byPerc);
				g_pLabel->setText(szPerc);
			}
			g_pLoading->OnDraw(0.f, 0.f, 1.f, 0xFF);
			if (byCurrLoadPerc == 100)
			{
				byCurrLoadPerc = 0xFF;
				g_pLoading->ShowWindow(false);
			}
		}
		return;
	}
	if(m_pIZMap)
		m_pIZMap->visit();
	if(!XMapObj::m_bySlow || 0xFF == XMap::m_byIZMapState)	// 后面那个判断只在新手副本中用
		XUIManger::OnDraw(0.f, 0.f, 1.f, 0xFF);
	XFUNC_END();
}
void XInstZoneState::OnRealse()
{
	XFUNC_START();
	if(m_pIZMap)
	{
		unsigned short wSoundID = m_pIZMap->GetMapSoundID();
		if (wSoundID != (unsigned short)-1)
		{
			XSoundManager::StopAllSound();
		}
		m_pIZMap->ClearMapObj();
		m_pIZMap->Reset();
	}
	XFUNC_END();
}
bool XInstZoneState::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return false;
	bool bRes = XUIManger::ccTouchBegan(pTouch, pEvent);
	if(!bRes)
	{
		bRes |= XJSGesture::Instance().ccTouchBegan(pTouch, pEvent);
		if(m_pIZMap)
			bRes |= m_pIZMap->ccTouchBegan(pTouch, pEvent);
	}
	bRes |= XJSControl::Instance().ccTouchBegan(pTouch, pEvent);
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}
void XInstZoneState::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchMoved(pTouch, pEvent);
	XJSControl::Instance().ccTouchMoved(pTouch, pEvent);
	XJSGesture::Instance().ccTouchMoved(pTouch, pEvent);
	if(m_pIZMap)
		m_pIZMap->ccTouchMoved(pTouch, pEvent);
	XFUNC_END();
}
void XInstZoneState::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if(0x64 > g_byLoadPerc)
		return;
	XUIManger::ccTouchEnded(pTouch, pEvent);
	XJSControl::Instance().ccTouchEnded(pTouch, pEvent);
	XJSGesture::Instance().ccTouchEnded(pTouch, pEvent);
	if(m_pIZMap)
		m_pIZMap->ccTouchEnded(pTouch, pEvent);
	XFUNC_END();
}
#ifdef X_SINGLE
	// 单机
	bool XSingleState::OnInit()
	{
		g_byLoadPerc = 0x00;
		byIZEvent = 0x00;
		if(!m_pIZMap)
		{
			//初始化地图
			m_pIZMap = XIZMap::CreateByID(m_nMapID, m_nMapID);
			if(!m_pIZMap)
			{
				CCLOG("CreateMap Faild!");
				return false;
			}
		}
		else
		{
			m_pIZMap->SetIZId(m_nMapID);
			m_pIZMap->InitByID(m_nMapID);
		}
		if(m_pIZMap)
		{
			if(!m_pRole)
			{
				m_pRole = XRole::Create();
				if(m_pRole)
				{
					m_pRole->SetRoleID(m_nRoleID);
					m_pIZMap->AddMapObj(m_pRole);
				}
			}
			if(m_pRole)
			{
				m_pRole->Wait(true);
				XJoystick::SetCtrlRole(m_pRole);
				m_pRole->SetPosition(100.f, 100.f, 0.f);
				m_pIZMap->SetControl(m_pRole);
				m_pRole->SetTypeA(XMapObj::eRole);
				m_pRole->SetTypeB(XMapObj::eHero);
				m_pRole->SetMaxHp(999999999);
				m_pRole->SetHp(999999999);
				m_pRole->SetMaxMp(2000);
				m_pRole->SetMp(2000);
				m_pRole->SetFlipX(false);
			}
		}
		unsigned char byWeatherID = m_pIZMap->GetWeatherID();
		XWeather::Instance().SetType(byWeatherID);
		XMapT* pMT = m_pIZMap->GetMapT();
		if(pMT)
			XWeather::Instance().Lightning(pMT->valLightning);
		// 初始化loding相关
		if(g_pLoading)
		{
			g_pLoading->ShowWindow(true);
		}
		return true;
	}
	unsigned char XSingleState::OnFrame()
	{
		XFUNC_START();
		unsigned char byState = enState_InstZone;
		if(m_pIZMap)
		{
			if(0x00 == byIZEvent && 0xFF == g_byLoadPerc)
			{
				XEventManger::ExecuteEvent(XCommonEvent::CITYIZMAP, 0);
				byIZEvent = 0xFF;
				m_pIZMap->SetIZMapBegin();
				unsigned short wSoundID = m_pIZMap->GetMapSoundID();
				if (wSoundID != (unsigned short)-1)
				{
					XSoundManager::PlaySoundByID(wSoundID);
				}
			}

			if(0x64 > g_byLoadPerc)
				g_byLoadPerc = m_pIZMap->GetLoadPerc();
			else if(0xFF != g_byLoadPerc)
			{
				// 加载第二屏
				m_pIZMap->LoadNextStageRes();
				g_byLoadPerc = 0xFF;
			}
			if(g_pBar)
				g_pBar->OnFrame();

			m_pIZMap->OnFrame();
		}
		XUIManger::OnFrame();
		return byState;
		XFUNC_END_RETURN(unsigned char, res, 0xFF);
	}
	void XSingleState::OnDraw()
	{
		XFUNC_START();
		if(0xFF != g_byLoadPerc)
		{
			static unsigned char byCurrLoadPerc = 0xFF;
			if (g_pLoading)
			{
				if (g_byLoadPerc != byCurrLoadPerc && g_pLabel && g_pBar)
				{
					byCurrLoadPerc = g_byLoadPerc;
					g_pBar->CalPross(g_byLoadPerc);
					static char szPerc[16];
					unsigned char byPerc = g_byLoadPerc;
					if(byPerc > 100)
					{
						byPerc = 100;
					}
					sprintf(szPerc, "%d%%", byPerc);
					g_pLabel->setText(szPerc);
				}
				g_pLoading->OnDraw(0.f, 0.f, 1.f, 0xFF);
				if (byCurrLoadPerc == 100)
				{
					byCurrLoadPerc = 0xFF;
					g_pLoading->ShowWindow(false);
				}
			}
			return;
		}
		if(m_pIZMap)
			m_pIZMap->visit();
		XUIManger::OnDraw(0.f, 0.f, 1.f, 0xFF);
		XFUNC_END();
	}
	void XSingleState::OnRealse()
	{
		XFUNC_START();
		if(m_pIZMap)
		{
			unsigned short wSoundID = m_pIZMap->GetMapSoundID();
			if (wSoundID != (unsigned short)-1)
			{
				XSoundManager::StopAllSound();
			}
			m_pIZMap->ClearMapObj();
			m_pIZMap->Reset();
		}
		XFUNC_END();
	}
	bool XSingleState::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		XFUNC_START();
		bool bRes = XUIManger::ccTouchBegan(pTouch, pEvent);
		if (!bRes)
		{
			bRes |= XJSGesture::Instance().ccTouchBegan(pTouch, pEvent);
			if(m_pIZMap)
				bRes |= m_pIZMap->ccTouchBegan(pTouch, pEvent);
		}
		bRes |= XJSControl::Instance().ccTouchBegan(pTouch, pEvent);
		return bRes;
		XFUNC_END_RETURN(bool, res, false);
	}
	void XSingleState::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		XFUNC_START();
		XUIManger::ccTouchMoved(pTouch, pEvent);
		XJSControl::Instance().ccTouchMoved(pTouch, pEvent);
		XJSGesture::Instance().ccTouchMoved(pTouch, pEvent);
		if(m_pIZMap)
			m_pIZMap->ccTouchMoved(pTouch, pEvent);
		XFUNC_END();
	}
	void XSingleState::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		XFUNC_START();
		XUIManger::ccTouchEnded(pTouch, pEvent);
		XJSControl::Instance().ccTouchEnded(pTouch, pEvent);
		XJSGesture::Instance().ccTouchEnded(pTouch, pEvent);
		if(m_pIZMap)
			m_pIZMap->ccTouchEnded(pTouch, pEvent);
		XFUNC_END();
	}
#endif

// 状态机管理器
unsigned char XGameStateManger::m_byNextFrameExitGame = 0x00;
unsigned char XGameStateManger::m_byState = enState_Login;
unsigned char XGameStateManger::m_byHEARTBEAT_REQ = 0x00;
unsigned char XGameStateManger::m_byCheckWaiting = 0x00;
unsigned char XGameStateManger::m_byEnterBackground = 0x00;
//unsigned char XGameStateManger::m_byEnterForeground = 0x00;
unsigned long XGameStateManger::m_dwWaitingTick = 0;
unsigned long XGameStateManger::m_dwWaitingTotalTick = 0;
unsigned long XGameStateManger::m_dwNetCloseTick = 0;
#ifdef X_SDK_SWITCH
	unsigned char XGameStateManger::m_byLoginFlag = 0x00;
#endif
volatile unsigned char XGameStateManger::m_byChageFaild = 0x00;
volatile unsigned char XGameStateManger::m_byChageID = 0xFF;

XGameSateBase* XGameStateManger::m_GameState[enState_MaxNum];

XGameStateManger::XGameStateManger()
{
	XMemLog::m_dwNew += sizeof(XGameStateManger);
}
XGameStateManger::~XGameStateManger()
{
	XMemLog::m_dwNew -= sizeof(XGameStateManger);
}
void XGameStateManger::Init()
{
	XFUNC_START();
	for ( unsigned char byState = enState_Login; byState < enState_MaxNum; ++byState )
	{
		Create(byState);
	}
	m_GameState[m_byState]->OnInit();
	XFUNC_END();
}
void XGameStateManger::Create(unsigned char byState)
{
	XFUNC_START();
	if ( byState >= enState_MaxNum )
	{
		return;
	}

	if ( m_GameState[byState] )
	{
		return;
	}

	switch(byState)
	{
	case enState_Login:
		{
			XLoginState* pLogin = new XLoginState;
			m_GameState[byState] = (XGameSateBase*)pLogin;
		}
		break;
	case enState_SelectRole:
		{
			XSelRoleState* pSelRole = new XSelRoleState;
			m_GameState[byState] = (XGameSateBase*)pSelRole;
		}
		break;
	case enState_City:
		{
			XCityState* pCity = new XCityState;
			m_GameState[byState] = (XGameSateBase*)pCity;
		}
		break;
	case enState_InstZone:
		{
			XInstZoneState* pIZState = new XInstZoneState;
			m_GameState[byState] = (XGameSateBase*)pIZState;
		}
		break;
#ifdef X_SINGLE
	case enState_Single:
		{
			XSingleState* pSingle = new XSingleState;
			m_GameState[byState] = (XGameSateBase*)pSingle;
		}
		break;
#endif
	}
	XFUNC_END();
}

void XGameStateManger::ChangeState( unsigned char byState )
{
	XFUNC_START();
	if ( /*m_byState == byState || */byState >= enState_MaxNum )
	{
		return;
	}
	if(m_byState < enState_MaxNum)
	{
		if ( m_GameState[m_byState] )
		{
			// 释放当前状态
			m_GameState[m_byState]->OnRealse();
		}
	}
	XUIManger::SetTouchBegin(0x00);
	XUIManger::ResetGuild();
	m_byState = byState;
	m_GameState[m_byState]->OnInit();
	GameScriptManger::Instance().LuaGC();
	cocos2d::CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	XFUNC_END();
}

extern unsigned char g_byShowLine;
void XGameStateManger::OnFrame()
{
	XFUNC_START();
	//if(m_byEnterBackground)
	//{
	//	--m_byEnterBackground;
	//	return;
	//}
	if(0xFF == m_byNextFrameExitGame)
	{
		ExitGame();
		return;
	}
	unsigned char byState = enState_MaxNum;
	if ( m_byState < enState_MaxNum )
	{
		if ( m_GameState[m_byState] )
		{
			// Key test in windows
			if(XJoystick::m_opCtrlRole)
			{
#if defined(WIN32) && defined(_WINDOWS)
				XRoleCmd stCmd;
				stCmd.byTypeA = XRole::eCmdA_Move;
				stCmd.byTypeB = 0xFF;
				stCmd.nParam1 = (short)(0.f * 1000.f);
				stCmd.nParam2 = (short)(0.f * 1000.f);
				static unsigned char byKeyStates = 0;
				if( byKeyStates )
				{
					if ( (byKeyStates == 1 && KEYUP('W')) || (byKeyStates == 2 && KEYUP('S')) || (byKeyStates == 3 && KEYUP('D')) || (byKeyStates == 4 && KEYUP('A')))
					{
						stCmd.byTypeA = XRole::eCmdA_Wait;
						XJoystick::m_opCtrlRole->SendCmd(stCmd);
						byKeyStates = false;
					}
				}
				if(KEYDOWN('W'))
				{
					stCmd.nParam2 = (short)(1.f * 1000.f);
					XJoystick::m_opCtrlRole->SendCmd(stCmd);
					byKeyStates = 1;
				}
				if(KEYDOWN('S'))
				{
					stCmd.nParam2 = (short)(-1.f * 1000.f);
					XJoystick::m_opCtrlRole->SendCmd(stCmd);
					byKeyStates = 2;
				}
				if(KEYDOWN('D'))
				{
					stCmd.nParam1 = (short)(1.f * 1000.f);
					XJoystick::m_opCtrlRole->SendCmd(stCmd);
					byKeyStates = 3;
				}
				if(KEYDOWN('A'))
				{
					stCmd.nParam1 = (short)(-1.f * 1000.f);
					XJoystick::m_opCtrlRole->SendCmd(stCmd);
					byKeyStates = 4;
				}
				if(KEYDOWN('O'))
					g_ShowUILine = 0x00;
				if(KEYDOWN('P'))
					g_ShowUILine = 0xFF;
				if(KEYDOWN('U'))
					g_byShowLine = 0x00;
				if(KEYDOWN('I'))
					g_byShowLine = 0xFF;
				if(KEYDOWN('J'))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Skill, XRole::eSkl_Normal, 0, 0);
				if(KEYDOWN(VK_SPACE))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Dodge, XRole::eCmdB_None, 0, 0);
				if(KEYDOWN(VK_UP))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Skill, XRole::eSkl_Up, 0, 0);
				if(KEYDOWN(VK_DOWN))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Skill, XRole::eSkl_Down, 0, 0);
				if(KEYDOWN(VK_RIGHT))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Skill, XRole::eSkl_Front, 0, 0);
				if(KEYDOWN(VK_LEFT))
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Skill, XRole::eSkl_Back, 0, 0);
				if (KEYDOWN(VK_ESCAPE))
				{
					if (XJoystick::m_bDrawLine)
					{
						XJoystick::m_bDrawLine = false;
					}
					else
					{
						XJoystick::m_bDrawLine = true;
					}
				}
				if(KEYDOWN('C'))
				{
					static unsigned char byMax = XTableFile::stTblCamera.GetMaxRowID();
					static unsigned char byNow = 0;
					XCamera::Instance().Effect(byNow++);
					if(byNow >= byMax)
						byNow = 0;
				}
				//if(KEYDOWN('I'))
				//{
				//	if(!XJoystick::m_opCtrlRole->IsUsePAI())
				//		XJoystick::m_opCtrlRole->UsePlayerAI();
				//}
#endif /* defined(WIN32) && defined(_WINDOWS) */
				if (XJoystick::m_byJump)
				{
					XJoystick::m_opCtrlRole->SendCmd(XRole::eCmdA_Dodge, XRole::eCmdB_None, 0, 0);
					XJoystick::m_byJump--;
				}
			}
			byState = m_GameState[m_byState]->OnFrame();
		}
		XTimerManger::OnFrame();

		if(m_byHEARTBEAT_REQ)
		{
			unsigned long dwTick = XConfig::Instance().GetTickCount();
			// 每30秒发送一次心跳给服务器
			static unsigned long _dwHBTick = 0;
			if(_dwHBTick + 30000 < dwTick)
			{
				XEventManger::ExecuteEvent(XCommonEvent::HEARTBEAT_REQ, 0);
				_dwHBTick = dwTick;
			}
		}
		// 检查网路等待
		if(m_byCheckWaiting && !g_byOffLine)
		{
			unsigned long dwTick = XConfig::Instance().GetTickCount();			
			if(m_dwWaitingTick + 1500 < dwTick)
			{				
				m_dwWaitingTick = dwTick;
				XEventManger::ExecuteEvent(XCommonEvent::NET_WAITING_CHECK, 0);
			}
			// 超过10秒,停止检查,清空所有网路等待
			if(m_dwWaitingTotalTick + 10000 < dwTick)
			{
				XUIManger::SetWaiting(0x00);
				m_byCheckWaiting = 0x00;
				XEventManger::ExecuteEvent(XCommonEvent::NET_WAITING_CLEAR, 0);
			}
		}
		if(m_dwNetCloseTick)
		{
			unsigned long dwTick = XConfig::Instance().GetTickCount();
			// 超过3秒直接断线
			if(m_dwNetCloseTick + 3000 < dwTick)
			{
				g_byOffLine = 0xFF;
				m_byHEARTBEAT_REQ = 0x00;
				m_dwNetCloseTick = 0;
				XEventManger::ExecuteEvent(XCommonEvent::DISCONNECT_EVENT, 0);
			}
		}
		//if(m_byEnterForeground)
		//{
		//	if(m_dwNetCloseTick)
		//	{
		//		unsigned long dwTick = XConfig::Instance().GetTickCount();
		//		// 小于10秒，断线重连
		//		if(m_dwNetCloseTick + 10000 > dwTick)
		//		{
		//			g_byOffLine = 0xFE;
		//			m_byHEARTBEAT_REQ = 0x00;
		//			XEventManger::ExecuteEvent(XCommonEvent::RECONNECT_EVENT, 0);
		//		}
		//	}
		//	m_byEnterForeground = 0x00;
		//}
		if(XGameStateManger::m_byChageFaild)
		{
			XGameStateManger::m_byChageFaild = 0x00;
			XEventManger::ExecuteEvent(XCommonEvent::CHARGE_EVENT, 1);
		}
		if(0xFF != XGameStateManger::m_byChageID)
		{
			XChargeT* pCharge = (XChargeT*)XTableFile::stCharge.GetRowData(XGameStateManger::m_byChageID, sizeof(XChargeT));
			if(pCharge) {
				XGameStateManger::m_byChageID = 0xFF;
				XEventManger::ExecuteEvent(XCommonEvent::CHARGE_EVENT, pCharge->valGetCount);
			}
		}
	}
	XFUNC_END();
}

void XGameStateManger::OnDraw()
{
	XFUNC_START();
	if ( m_byState < enState_MaxNum )
	{
		m_GameState[m_byState]->OnDraw();
	}
	XFUNC_END();
}

bool XGameStateManger::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if ( m_byState < enState_MaxNum )
	{
		return m_GameState[m_byState]->ccTouchBegan(pTouch, pEvent);
	}
	XFUNC_END_RETURN(bool, res, false);
}
void XGameStateManger::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if ( m_byState < enState_MaxNum )
	{
		m_GameState[m_byState]->ccTouchMoved(pTouch, pEvent);
	}
	XFUNC_END();
}
void XGameStateManger::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	XFUNC_START();
	if ( m_byState < enState_MaxNum )
	{
		m_GameState[m_byState]->ccTouchEnded(pTouch, pEvent);
	}
	XFUNC_END();
}
XGameSateBase* XGameStateManger::GetGameState(unsigned char byState)
{
	XFUNC_START();
	if(byState < enState_MaxNum)
	{
		return m_GameState[byState];
	}

	return NULL;
	XFUNC_END_RETURN(XGameSateBase*, res, NULL);
}

void XGameStateManger::OnJump()
{
	XFUNC_START();
	if(XJoystick::m_opCtrlRole)
	{
		XJoystick::m_byJump = 2;
	}
	XFUNC_END();
}

void XGameStateManger::SetCityTouch(unsigned char byTouch)
{
	XFUNC_START();
	if(m_GameState && m_GameState[enState_City])
	{
		XCityState* pCityState = dynamic_cast<XCityState*>(m_GameState[enState_City]);
		if(pCityState)
		{
			XMap* pMap = pCityState->GetMap();
			if(pMap)
				pMap->SetTouchMap(byTouch);
		}
	}
	XFUNC_END();
}

void XGameStateManger::ExitGame()
{
	XFUNC_START();
	XSoundManager::PauseAllSound();
	XDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
	XFUNC_END();
}

void XGameStateManger::SetShowJoystick(unsigned char byType, bool bIsShow)
{
	XFUNC_START();
	if(0x00 == byType)
	{
		XJSControl::Instance().ShowJoystick(bIsShow);
	}
	else
	{
		XJSGesture::Instance().ShowJoystick(bIsShow);
	}
	XFUNC_END();
}

void XGameStateManger::SetJoyStickStates(unsigned char byType, bool bActive)
{
	XFUNC_START();
	if (0x00 == byType)
	{
		XJSControl::Instance().SetActive(bActive);
		XRole* pRole = XJoystick::m_opCtrlRole;
		if (pRole)
		{
			pRole->ForceWait();
		}
	}
	else
	{
		XJSGesture::Instance().SetActive(bActive);
	}
	XFUNC_END();
}

void XGameStateManger::UseSkillByDir( unsigned char byDir )
{
	XFUNC_START();
	if (byDir >= 1 && byDir <= 5)
	{
		XJSGesture::Instance().UpdateRole(true, byDir);
	}
	XFUNC_END();
}

unsigned short XGameStateManger::GetActivityRoleID( const char* szMapInfoPath, unsigned char byMark )
{
	XFUNC_START();
	do 
	{
		if(!szMapInfoPath || !szMapInfoPath[0])
		{
			break;
		}
		XTable stTable;
		if(!stTable.LoadTable(szMapInfoPath))
			break;
		unsigned char byAddNpcNum = stTable.GetMaxRowID();
		
		for (int nIndex = 0; nIndex < byAddNpcNum; ++nIndex)
		{
			if (!XBIT_VALUE(&byMark, nIndex))
			{
				continue;
			}
			XMapInfoT* pMapInfo = (XMapInfoT*)stTable.GetRowData(nIndex, sizeof(XMapInfoT));
			if(pMapInfo)
			{
				return pMapInfo->valID;
			}
		}
	} while (0);
	XFUNC_END_RETURN(unsigned short, res, 0xFFFF);
}
void XGameStateManger::SetCheckWaiting(unsigned char byCheck)
{
	XFUNC_START();
	m_byCheckWaiting = byCheck;
	if(byCheck)
	{
		m_dwWaitingTick = XConfig::Instance().GetTickCount();
		if(!m_dwWaitingTotalTick)
		{
			m_dwWaitingTotalTick = m_dwWaitingTick;
		}
	}
	else
	{
		m_dwWaitingTotalTick = 0;
	}
	XFUNC_END();
}
void XGameStateManger::OnReLoadGame()
{
	XFUNC_START();
#ifdef _WIN32
	OnResetLoadInfo(false);
#else

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(XGameStateManger::m_pHelloWorld)
		XGameStateManger::m_pHelloWorld->setKeypadEnabled(false);
#endif

	OnResetLoadInfo(true);
	XSdkManager::OnReLoadGame();
#endif
	XFUNC_END();
}

void XGameStateManger::OnResetLoadInfo(bool bSDKSurface)
{
	XFUNC_START();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	XSoundManager::PauseAllSound();
	// 把自己和自己的精灵从地图中删除
	XMap* pMap = GetCurrMap();
	if(pMap)
	{
		pMap->ClearAllMapobj();
	}
	XUIManger::ReLoadScript();
	m_byHEARTBEAT_REQ = 0x00;
	m_dwWaitingTick = 0;
	m_dwWaitingTotalTick = 0;
	g_byOffLine = 0xFF;
	XNetWork::Instance().Close();	
	m_dwNetCloseTick = 0;
	m_byEnterBackground = 0x00;
	//m_byEnterForeground = 0x00;
#ifdef X_SDK_SWITCH
	XGameStateManger::m_byLoginFlag = 0x00;
	XGameStateManger::m_bySDKLogin = 0x00;
	/*if (bSDKSurface)
	{
		XGameStateManger::m_bySDKSurface = 0x00;
	}*/
	XGameStateManger::m_bySDKSurface = 0x00;
	CCLog("lihaofeng XGameStateManger::m_bySDKSurface =  %d", XGameStateManger::m_bySDKSurface);
#endif /* X_SDK_SWITCH */
	// 各个状态机重置
	m_GameState[enState_City]->OnRealse();
	m_GameState[enState_InstZone]->OnRealse();

	XEventManger::ExecuteEvent(XCommonEvent::RELOADGAME_CHANGESTATE, 0);

	XFUNC_END();
}
void XGameStateManger::NetClose()
{
	XFUNC_START();
	if(m_byHEARTBEAT_REQ)
	{
//#ifdef X_SDK_SWITCH
//		m_bySDKLogin = 0x00;
//#endif /* X_SDK_SWITCH */
		g_byOffLine = 0xEE;
		XNetWork::Instance().Close();
	}
	XFUNC_END();
}
void XGameStateManger::OnNetClose()
{
	XFUNC_START();
	if(m_byHEARTBEAT_REQ && !g_byOffLine && !m_dwNetCloseTick)
	{
		g_byOffLine = 0xFE;
		m_byHEARTBEAT_REQ = 0x00;
		m_dwNetCloseTick = XConfig::Instance().GetTickCount();
		XEventManger::ExecuteEvent(XCommonEvent::RECONNECT_EVENT, 0);
	}
	XFUNC_END();
}
void XGameStateManger::OnEnterBackground()
{
	XFUNC_START();
	//if(m_byHEARTBEAT_REQ)
	//{
	//	m_dwNetCloseTick = XConfig::Instance().GetTickCount();
	//}
	//m_byEnterBackground = 0x01;
	//m_byEnterForeground = 0x00;
	XEventManger::ExecuteEvent(XCommonEvent::HEARTBEAT_REQ, 0);
	XFUNC_END();
}
void XGameStateManger::OnEnterForeground()
{
	XFUNC_START();
	//m_byEnterForeground = 0xFF;
	XEventManger::ExecuteEvent(XCommonEvent::HEARTBEAT_REQ, 0);
	XFUNC_END();
}
void XGameStateManger::OnNetReLoadSucc()
{
	XFUNC_START();
	m_dwNetCloseTick = 0;
	g_byOffLine = 0x00;
	m_byHEARTBEAT_REQ = 0xFF;
	XFUNC_END();
}
void XGameStateManger::OnNetReLoadFaild()
{
	XFUNC_START();
	g_byOffLine = 0xFF;
	m_byHEARTBEAT_REQ = 0x00;
	m_dwNetCloseTick = 0;
	XEventManger::ExecuteEvent(XCommonEvent::DISCONNECT_EVENT, 0);
	XFUNC_END();
}
XMap* XGameStateManger::GetCurrMap()
{
	XFUNC_START();
	if(enState_City == m_byState)
	{
		 XCityState* pState = dynamic_cast<XCityState*>(m_GameState[m_byState]);
		 if(pState)
		 {
			 return pState->GetMap();
		 }
	}
	if(enState_InstZone == m_byState)
	{
		XInstZoneState* pState = dynamic_cast<XInstZoneState*>(m_GameState[m_byState]);
		if(pState)
		{
			XIZMap* pMap = pState->GetMap();
			if(pMap)
			{
				return dynamic_cast<XMap*>(pMap);
			}
		}
	}
	XFUNC_END_RETURN(XMap*, res, NULL);
}

#ifdef X_SINGLE
	int GS_Single(lua_State* tolua_s)
	{
		XFUNC_START();
		if(!tolua_s)
			return 0;
		if(2 != lua_gettop(tolua_s))
		{
			CCLOG("[GS_Single] lua param must be 2 !");
			return 0;
		}
		unsigned int nMapID = (unsigned int)lua_tointeger(tolua_s, 1);
		unsigned int nRoleID = (unsigned int)lua_tointeger(tolua_s, 2);
		XSingleState* pState = (XSingleState*)XGameStateManger::GetGameState(enState_Single);
		if(pState)
		{
			pState->m_nRoleID = nRoleID;
			pState->m_nMapID = nMapID;
			XGameStateManger::ChangeState(enState_Single);
			tolua_pushusertype(tolua_s,(void*)pState->GetSelf(),"XRole");
			return 1;
		}
		XFUNC_END_RETURN(int, res, 0);
	}
#endif

extern int tolua_XGameStateBase_open(lua_State* tolua_S);
extern int tolua_XCityState_open(lua_State* tolua_S);
extern int tolua_XInstZoneState_open(lua_State* tolua_S);
extern int tolua_XGameStateManger_open(lua_State* tolua_S);
extern int tolua_XSelRoleState_open(lua_State* tolua_S);

int tolua_GameState_Open(lua_State* tolua_s)
{
	lua_pushinteger(tolua_s, enState_Login);
	lua_setglobal(tolua_s, "GS_LOGIN");
	lua_pushinteger(tolua_s, enState_SelectRole);
	lua_setglobal(tolua_s, "GS_SELECTROLE");
	lua_pushinteger(tolua_s, enState_City);
	lua_setglobal(tolua_s, "GS_CITY");
	lua_pushinteger(tolua_s, enState_InstZone);
	lua_setglobal(tolua_s, "GS_INSTZONE");
#ifdef X_SINGLE
	lua_register(tolua_s, "GS_Single", GS_Single);
#endif
	tolua_XGameStateBase_open(tolua_s);
	tolua_XCityState_open(tolua_s);
	tolua_XInstZoneState_open(tolua_s);
	tolua_XSelRoleState_open(tolua_s);
	tolua_XGameStateManger_open(tolua_s);

	return 0;
}
