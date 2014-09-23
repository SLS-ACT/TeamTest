
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "../Common.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
	const char* LuaDump(lua_State* pLS);
}
#include "CCLuaEngine.h"
#include "tolua_fix.h"
#include "../XHelper/XSingleton.h"

/*
	游戏脚本管理器
	by 徐亚博
*/
class GameScriptManger :
	public XSingleton<GameScriptManger>
{
public:
	GameScriptManger() :m_pLs(NULL), m_bySetGlobal(0x00) {
		XMemLog::m_dwNew += sizeof(GameScriptManger);
	}
	~GameScriptManger() {
		XMemLog::m_dwNew -= sizeof(GameScriptManger);
	}

	inline lua_State* GetLuaState(){
		return m_pLs;
	}
	const char* GetLuaTableName()
	{
		return "GameScript";
	}
	bool LoadScript(const char* szPath);
	bool InitByXml(const char* szXml);
	void SetLuaGlobalValue();
	void LuaGC();
private:
	lua_State* m_pLs;
	unsigned char m_bySetGlobal;
};

int tolua_GameScript_Open(lua_State* tolua_s);

#endif