
#ifndef _XTIMERMANGER_H_
#define _XTIMERMANGER_H_

/*
	时间管理类
	by 徐亚博
*/

#include "../Common.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "CCLuaEngine.h"
#include "tolua_fix.h"
#include "../XHelper/XSingleton.h"

#define TIMERINFO_MAXNUM		(32)		// 暂时支持的注册定时器数量

// 定时器结构体
struct TimerInfo
{
	TimerInfo()
	{
		Rest();
	}

	inline void Rest()
	{
		fEndTick = 0.f;
		fTick = 0.f;
		pLs = NULL;
		nFunc = -1;
		nParam = -1;
		nCount = 1;
	}

	float fEndTick;				// 定时器结束时间
	float fTick;				// 每次定时器的时间
	lua_State* pLs;				// lua回调函数
	int nFunc;					// 时间到后的回调函数
	int nParam;					// 回调函数的参数
	int nCount;					// 循环次数，0是无限循环
};

class XTimerManger
{
public:
	XTimerManger(){
		XMemLog::m_dwUI += sizeof(XTimerManger);
	}
	~XTimerManger(){
		XMemLog::m_dwUI -= sizeof(XTimerManger);
	}
public:
	static void OnFrame();
	static int RegTime(float fTick, lua_State* pLs, int nFunc, int nParam = -1, int nCount = 1);
	static void DelTimeById(int nId);

private:
	static TimerInfo m_astTimeInfo[TIMERINFO_MAXNUM];
};

int tolua_XTimerManger_Open(lua_State* tolua_s);

#endif