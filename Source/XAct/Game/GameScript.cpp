#include "GameScript.h"
#include "../XHelper/XAutoClrLua.h"
#include "../XNet/XNetWork.h"
#include "../XEvent/XEventManger.h"
#include "XTimeManager.h"
#include "../XSound/XSoundManager.h"
#include "../Update/XFileAPIC.h"
#include "MaskWord.h"

using namespace cocos2d;

XCPP_DEFINED();

bool GameScriptManger::LoadScript(const char* szPath)
{
	XFUNC_START();
	do 
	{
		if(!m_pLs)
		{
			CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
			if(!pEngine)
				break;
			CCLuaStack* pLuaState = pEngine->getLuaStack();
			if(!pLuaState)
				break;
			m_pLs = pLuaState->getLuaState();

			tolua_GameScript_Open(m_pLs);
			int nTop = lua_gettop(m_pLs);
			if(szPath && szPath[0])
			{
				// 设置自己的Lua table 名词
				cocos2d::CCString* pstrFileContent = cocos2d::CCString::createWithContentsOfFile(szPath);
				if (!pstrFileContent)
				{
					break;
				}

				if(!m_bySetGlobal)
				{
					lua_pushnumber(m_pLs, g_wChannelNum);
					lua_setglobal(m_pLs, "g_wChannelNum");
					lua_pushnumber(m_pLs, g_wCharge);
					lua_setglobal(m_pLs, "g_wCharge");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
					lua_pushnumber(m_pLs, 2);
					lua_setglobal(m_pLs, "g_TargType");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
					lua_pushnumber(m_pLs, 1);
					lua_setglobal(m_pLs, "g_TargType");
#else
					lua_pushnumber(m_pLs, 3);
					lua_setglobal(m_pLs, "g_TargType");
#endif

				}

				int nRes = luaL_dostring(m_pLs, pstrFileContent->getCString());
				if(nRes)
				{
					CCLOG("GameScriptManger::LoadScript luaL_dofile [%s] Faild, Error Info is [%s]",
						szPath, lua_tostring(m_pLs, -1));
					lua_pop(m_pLs,1);	// error info
					break;
				}
			}
		}
		int nEnd = 0;
		const char* sztableName = GetLuaTableName();
		lua_getglobal(m_pLs, "GameScript");
		if(lua_istable(m_pLs, -1))
		{
			// 测试代码
			/*lua_pushnumber(m_pLs, 111.1);
			lua_setfield(m_pLs, -2, "tCtrl");
			lua_pushstring(m_pLs, "I am panel");
			lua_setfield(m_pLs, -2, "tPanel");*/
			//测试结束

			// 调用Onload函数
			lua_getfield(m_pLs, -1, "OnLoad");
			if(lua_isfunction(m_pLs, -1))
			{
				int nRes = lua_pcall(m_pLs, 0, 1, 0);
				if(nRes)
				{
					CCLOG("GameScriptManger::LoadScript call OnLoad Faild, Error info is[%s]!!!",
						lua_tostring(m_pLs, -1));
					lua_pop(m_pLs,1);	// error
					lua_pop(m_pLs,1);	// table
					break;
				}
				// get return value
				if (!lua_isnumber(m_pLs, -1))
				{
					lua_pop(m_pLs, 1);	// error
					lua_pop(m_pLs, 1);	// table
					break;
				}
				nEnd = lua_tointeger(m_pLs, -1);
				lua_pop(m_pLs, 1);
			}
		}
		lua_pop(m_pLs,1);	// table
#ifdef X_SDK_SWITCH
		if(!m_bySetGlobal)
		{
			lua_pushnumber(m_pLs, 1);
			lua_setglobal(m_pLs, "g_IsSDKVersion");
		}
#endif
		m_bySetGlobal = 0xFF;
		if(nEnd)
		{			
			return true;
		}
		return false;
	} while (0);
	CCLOG("GameScriptManger::LoadScript Faild!");
	XFUNC_END_RETURN(bool, res, false);
}
bool GameScriptManger::InitByXml(const char* szXml)
{
	XFUNC_START();
	if(!szXml || !szXml[0])
		return false;

	XFUNC_END_RETURN(bool, res, false);
}
void GameScriptManger::SetLuaGlobalValue()
{
	XFUNC_START();

	XFUNC_END();
}

void GameScriptManger::LuaGC()
{
	XFUNC_START();
	if(m_pLs)
	{
		CCLOG("---------Befor GCCOLLECT lua Mem is %d KB---------", lua_gc(m_pLs, LUA_GCCOUNT, 1));
		lua_gc(m_pLs, LUA_GCCOLLECT, 1);
		CCLOG("---------After GCCOLLECT lua Mem is %d KB---------", lua_gc(m_pLs, LUA_GCCOUNT, 1));
	}
	XFUNC_END();
}

int ConnectToServer(lua_State* tolua_s)
{
	XFUNC_START();
	if(!tolua_s)
		return 0;
	int nTop = lua_gettop(tolua_s);
	if( 2 != nTop)
	{
		CCLOG("RegEvent param must be 2 not is %d!", nTop);
		return 0;
	}
	const char* szIp = lua_tostring(tolua_s, 1);

	// 主机地址 to IP
	PHOSTENT pHostent = NULL;
	SOCKADDR_IN stServerSA;
	unsigned long dwIP = inet_addr(szIp);
	if (INADDR_NONE == dwIP)
	{
		// 获取与主机相关的信息
		pHostent = gethostbyname(szIp);
		if (pHostent)
		{
			memcpy((char*)&(stServerSA.sin_addr),
				pHostent->h_addr,
				pHostent->h_length);
			szIp = inet_ntoa(stServerSA.sin_addr);
		}
		else
		{
			CCLOG("[ConnectToServer]gethostbyname error!");
			XEventManger::ExecuteEvent(XCommonEvent::CONNECTSERVER_FAILD, 0);
		}
	}

	int nPor = lua_tointeger(tolua_s, 2);
	if( !XNetWork::Instance().ConnectToServer(szIp, nPor) )
	{
		XEventManger::ExecuteEvent(XCommonEvent::CONNECTSERVER_FAILD, 0);
	}
	return 0;
	XFUNC_END_RETURN(int,res,0);
}

extern int tolua_XMap_open(lua_State* tolua_s);
extern int tolua_XIZMap_open(lua_State* tolua_s);
extern int tolua_GameState_Open(lua_State* tolua_s);
extern int tolua_XUI_Open(lua_State* tolua_s);
extern int tolua_XEventManger_Open(lua_State* tolua_s);
extern int tolua_XTimerManger_Open(lua_State* tolua_s);
extern int tolua_XSoundManager_open(lua_State* tolua_s);
extern int tolua_MaskWord_open(lua_State* tolua_s);
// role
extern int tolua_XRole_open(lua_State* tolua_S);
int tolua_GameScript_Open(lua_State* tolua_s)
{
	lua_register(tolua_s, "ConnectToServer", ConnectToServer);
	tolua_XMap_open(tolua_s);
	tolua_XIZMap_open(tolua_s);
	tolua_GameState_Open(tolua_s);
	tolua_XRole_open(tolua_s);
	tolua_XUI_Open(tolua_s);
	tolua_XEventManger_Open(tolua_s);
	tolua_XTimerManger_Open(tolua_s);
	tolua_XSoundManager_open(tolua_s);
	tolua_MaskWord_open(tolua_s);
	return 0;
}