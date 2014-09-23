
#ifndef _XGAMESTATE_H_
#define _XGAMESTATE_H_

#include "../XMap/XIZMap.h"
#include "../XMap/XCreateRoleMap.h"
#include "../Common.h"
/*
	游戏状态机类
	by 徐亚博
*/

enum
{
	enState_Login = 0,		// 登陆
	enState_SelectRole,		// 选角色
	enState_City,			// 城市
	enState_InstZone,		// 副本
#ifdef X_SINGLE
	enState_Single,			// 单机
#endif

	enState_MaxNum
};

class HelloWorld;

class XRenderString;

#define MAX_LOADEFFECT_NUM		(32)
#define SDK_STR_NUM				(128)

// 状态机基类
class XGameSateBase
{
public:
	XGameSateBase(){}
	virtual ~XGameSateBase(){};
	virtual bool OnInit() = 0;
	virtual unsigned char OnFrame() = 0;
	virtual void OnDraw() = 0;
	virtual void OnRealse() = 0;
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) = 0;
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) = 0;
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) = 0;
};

// 登陆
class XLoginState : public XGameSateBase
{
public:
	XLoginState(){
		m_byLoadScript = 0x00;
		m_pSprLogo = NULL;
		m_pStrVer = NULL;
		XMemLog::m_dwNew += sizeof(XLoginState);
	}
	virtual ~XLoginState(){
		XMemLog::m_dwNew -= sizeof(XLoginState);
	}
	virtual bool OnInit();
	virtual unsigned char OnFrame();
	virtual void OnDraw();
	virtual void OnRealse();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
	unsigned char m_byPlayMusic;
	unsigned char m_byLoadScript;
	cocos2d::XSprite*	m_pSprLogo;
	XRenderString*	m_pStrVer;
	// 登陆对象的指针
};

// 选角色
class XCreateRoleMap;
class XSelRoleState : public XGameSateBase
{
public:
	XSelRoleState(): m_pRoleMap(NULL){
		XMemLog::m_dwNew += sizeof(XSelRoleState);
	}
	virtual ~XSelRoleState(){
		XMemLog::m_dwNew -= sizeof(XSelRoleState);
	}
	static XSelRoleState* ToSelRoleState(XGameSateBase* pState)
	{
		XSelRoleState* pSelRole = dynamic_cast<XSelRoleState*>(pState);
		return pSelRole;
	}
	virtual bool OnInit();
	virtual unsigned char OnFrame();
	virtual void OnDraw();
	virtual void OnRealse();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	inline XCreateRoleMap* GetMap()
	{
		return m_pRoleMap;
	}
	inline XRole* GetRoleByID(unsigned char byRoleID)
	{
		if (m_pRoleMap)
		{
			return m_pRoleMap->GetRoleByID(byRoleID);
		}
		return NULL;
	}
	inline void AddRole(XRole* pRole)
	{
		if (m_pRoleMap)
		{
			m_pRoleMap->AddRole(pRole);
		}
	}
private:
	// 登陆对象的指针
	XCreateRoleMap*	m_pRoleMap;
};

// 城市
class XCityState : public XGameSateBase
{
public:
	XCityState() :m_wCurrMapId((unsigned short)-1), m_pCityMap(NULL) {
		XMemLog::m_dwNew += sizeof(XCityState);
	}
	virtual ~XCityState(){
		XMemLog::m_dwNew -= sizeof(XCityState);
	}
	static XCityState* ToCityState(XGameSateBase* pState)
	{
		XCityState* pCityState = dynamic_cast<XCityState*>(pState);
		return pCityState;
	}
	virtual bool OnInit();
	virtual unsigned char OnFrame();
	virtual void OnDraw();
	virtual void OnRealse();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	inline XMap* GetMap()
	{
		return m_pCityMap;
	}
	inline void SetMapId(unsigned short wMapId)
	{
		m_wCurrMapId = wMapId;
	}
	inline unsigned short GetMapId()
	{
		return m_wCurrMapId;
	}

private:
	float m_fPosX;
	float m_fPosY;
	unsigned short m_wCurrMapId;
	// 城市对象的指针
	XMap*	m_pCityMap;
};

// 副本
class XInstZoneState : public XGameSateBase
{
public:
	XInstZoneState() : m_wCurrIZId((unsigned short)-1), m_pIZMap(NULL), m_fCityPosX(600.f), m_fCityPosY(120.f), m_fCityPosH(100.f){
		XMemLog::m_dwNew += sizeof(XInstZoneState);
	}
	virtual ~XInstZoneState(){
		XMemLog::m_dwNew -= sizeof(XInstZoneState);
	}
	static XInstZoneState* ToIZState(XGameSateBase* pState)
	{
		XInstZoneState* pIZState = dynamic_cast<XInstZoneState*>(pState);
		return pIZState;
	}
	virtual bool OnInit();
	virtual unsigned char OnFrame();
	virtual void OnDraw();
	virtual void OnRealse();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	inline XIZMap* GetMap()
	{
		return m_pIZMap;
	}
	inline void SetIZId(unsigned short wIZId)
	{
		m_wCurrIZId = wIZId;
	}
	inline unsigned short GetIZId()
	{
		return m_wCurrIZId;
	}
	inline void GetRoleCityPos(float& fCityPosX, float& fCityPosY, float& fCityPosH)
	{
		fCityPosX = m_fCityPosX;
		fCityPosY = m_fCityPosY;
		fCityPosH = m_fCityPosH;
	}
private:
	unsigned short m_wCurrIZId;
	// 副本对象的指针
	XIZMap*			m_pIZMap;
	float			m_fCityPosX;
	float			m_fCityPosY;
	float			m_fCityPosH;
};
#ifdef X_SINGLE
	// 单机
	class XSingleState : public XGameSateBase
	{
	public:
		XSingleState() : m_pIZMap(NULL), m_nMapID(1), m_nRoleID(0), m_pRole(NULL){
			XMemLog::m_dwNew += sizeof(XSingleState);
		}
		virtual ~XSingleState(){
			XMemLog::m_dwNew -= sizeof(XSingleState);
		}
		static XSingleState* ToSingleState(XGameSateBase* pState)
		{
			XSingleState* pSingleState = dynamic_cast<XSingleState*>(pState);
			return pSingleState;
		}
		virtual bool OnInit();
		virtual unsigned char OnFrame();
		virtual void OnDraw();
		virtual void OnRealse();
		virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

		inline XIZMap* GetMap()
		{
			return m_pIZMap;
		}
		inline XRole* GetSelf()
		{
			return m_pRole;
		}
	public:
		unsigned int	m_nMapID;
		unsigned int	m_nRoleID;
	private:
		// 副本对象的指针
		XIZMap* m_pIZMap;
		XRole*	m_pRole;
	};
#endif

struct lua_State;
// 状态机管理器
class XGameStateManger
{
public:
	XGameStateManger();
	virtual ~XGameStateManger();
	static void Init();
	static void Create(unsigned char byState);
	static void ChangeState( unsigned char byState );
	static void OnFrame();
	static void OnDraw();
	static bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static XGameSateBase* GetGameState(unsigned char byState);
	static void OnJump();
	static unsigned char GetCurrState()
	{
		return m_byState;
	}
	static void SetShowJoystick(unsigned char byType, bool bIsShow);
	static void SetJoyStickStates(unsigned char byType, bool bActive);
	static void UseSkillByDir(unsigned char byDir);
	static void SetCityTouch(unsigned char byTouch);
	static unsigned short GetActivityRoleID(const char* szMapInfoPath, unsigned char byMark);
	static void SetNextFrameExitGame()
	{
		m_byNextFrameExitGame = 0xFF;
	}
	static void ExitGame();
	static void SetHEARTBEAT_REQ(unsigned char byHEARTBEAT_REQ)
	{
		m_byHEARTBEAT_REQ = byHEARTBEAT_REQ;
	}
	static unsigned char GetHEARTBEAT_REQ()
	{
		return m_byHEARTBEAT_REQ;
	}
	static void SetCheckWaiting(unsigned char byCheck);
	static unsigned char GetCheckWaiting()
	{
		return m_byCheckWaiting;
	}
	static void OnReLoadGame();
	static void OnResetLoadInfo(bool bSDKSurface);
	static XMap* GetCurrMap();
	static void NetClose();
	static void OnNetClose();
	static void OnEnterBackground();	// 切到后台
	static void OnEnterForeground();	// 切到前台
	static void OnNetReLoadSucc();		// 断线重连成功
	static void OnNetReLoadFaild();		// 断线重连失败
private:
	static unsigned char m_byNextFrameExitGame;
	static unsigned char m_byState;
	static unsigned char m_byHEARTBEAT_REQ;
	static unsigned char m_byCheckWaiting;	
	static unsigned long m_dwWaitingTick;
	static unsigned long m_dwWaitingTotalTick;
	static unsigned char m_byEnterBackground;
	//static unsigned char m_byEnterForeground;
	static XGameSateBase* m_GameState[enState_MaxNum];
	static unsigned long  m_dwNetCloseTick;			// 断线重连时间缓冲
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
public:
	static HelloWorld* m_pHelloWorld;
#endif
#ifdef X_SDK_SWITCH
public:
	static volatile unsigned char	m_bySDKLogin;
	static volatile unsigned char	m_bySDKSurface;
	static unsigned long			m_dwUID;
	static char						m_szACID[SDK_STR_NUM];
	static char						m_szToken[SDK_STR_NUM];
	static unsigned char m_byLoginFlag;
#endif /* X_SDK_SWITCH */
public:
	static volatile unsigned char	m_byChageFaild;
	static volatile unsigned char	m_byChageID;
};

int tolua_GameState_Open(lua_State* tolua_s);

#endif