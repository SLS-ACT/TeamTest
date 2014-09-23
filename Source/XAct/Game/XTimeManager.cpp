#include "XTimeManager.h"
#include "../XHelper/XAutoClrLua.h"
#include "../Cocos2dx/XDirector.h"

XCPP_DEFINED();

TimerInfo XTimerManger::m_astTimeInfo[TIMERINFO_MAXNUM];

int XTimerManger::RegTime(float fTick, lua_State* pLs, int nFunc, int nParam /* = -1 */, int nCount /* = 1 */)
{
	XFUNC_START();
	if(fTick > 0.f && pLs && nFunc >= 0)
	{
		if(nCount < 0)
			nCount = 1;

		for (int nIndex = 0; nIndex < TIMERINFO_MAXNUM; ++nIndex)
		{
			TimerInfo& stInfo = m_astTimeInfo[nIndex];
			if(fIsZero(stInfo.fEndTick))
			{
				stInfo.fEndTick = cocos2d::XDirector::Instance().GetRunTime() + fTick;
				stInfo.fTick = fTick;
				stInfo.pLs = pLs;
				stInfo.nFunc = nFunc;
				stInfo.nParam = nParam;
				stInfo.nCount = nCount;
				return nIndex;
			}
		}
		CCLOG("XTimerManger::RegTime index is over!!!!!");
	}
	CCLOG("XTimerManger::RegTime is error!!!!!");
	XFUNC_END_RETURN(int, res, -1);
}
void XTimerManger::DelTimeById(int nId)
{
	XFUNC_START();
	if(nId >= 0 && nId < TIMERINFO_MAXNUM)
	{
		m_astTimeInfo[nId].Rest();
	}
	CCLOG("XTimerManger::DelTimeById index[%d] is over!!!!!", nId);
	XFUNC_END();
}

void XTimerManger::OnFrame()
{
	XFUNC_START();
	for (int nIndex = 0; nIndex < TIMERINFO_MAXNUM; ++nIndex)
	{
		TimerInfo& stInfo = m_astTimeInfo[nIndex];
		if(!fIsZero(stInfo.fEndTick))
		{
			float fNowTime = cocos2d::XDirector::Instance().GetRunTime();
			if(fNowTime >= stInfo.fEndTick)
			{
				if(stInfo.pLs)
				{
					// 执行时间
					XAutoClrLua stAutoClear(stInfo.pLs);
					toluafix_get_function_by_refid(stInfo.pLs, stInfo.nFunc);				
					if(lua_isfunction(stInfo.pLs, -1))
					{
						lua_pushinteger(stInfo.pLs, (lua_Integer)stInfo.nParam);

						int nRes = lua_pcall(stInfo.pLs, 1, 0, 0);
						if(nRes)
						{
							CCLOG("[ERROR] XTimerManger::OnFrame callback Info is[%s]!!!",
								lua_tostring(stInfo.pLs, -1));
							lua_pop(stInfo.pLs, 1);	// pop error info
						}
					}
				}
				if(stInfo.nCount > 0)
				{
					if(1 == stInfo.nCount)
					{
						stInfo.Rest();
					}
					else
					{
						--stInfo.nCount;
						float fOffset = fNowTime - stInfo.fEndTick;
						stInfo.fEndTick += (stInfo.fTick - fOffset);
					}
				}
			}
		}
	}
	XFUNC_END();
}

// 注册一次性的Timer
int RegS_Timer(lua_State* tolua_s)
{
	XFUNC_START();
	if(!tolua_s)
		return 0;
	int nTop = lua_gettop(tolua_s);
	if(nTop != 2 && nTop != 3)
	{
		CCLOG("RegS_Timer param must be 2 or 3!!");
		return 0;
	}
	// 时间
	float fTick = (float)lua_tonumber(tolua_s, 1);
	// 回调函数
	int nFunc = toluafix_ref_function(tolua_s, 2, 0);
	// 参数
	int nParam = -1;
	if(3 == nTop)
		nParam = lua_tointeger(tolua_s, 3);

	int nPos = XTimerManger::RegTime(fTick, tolua_s, nFunc, nParam);
	lua_pushinteger(tolua_s, nPos);
	return 1;
	XFUNC_END_RETURN(int, res, 0);
}

// 注册多次性的Timer
int RegM_Timer(lua_State* tolua_s)
{
	XFUNC_START();
	if(!tolua_s)
		return 0;
	int nTop = lua_gettop(tolua_s);
	if(nTop != 4)
	{
		CCLOG("RegM_Timer param must be 4!!");
		return 0;
	}
	// 时间
	float fTick = (float)lua_tonumber(tolua_s, 1);
	// 回调函数
	int nFunc = toluafix_ref_function(tolua_s, 2, 0);
	// 参数
	int nParam = lua_tointeger(tolua_s, 3);
	// 次数
	int nCount = lua_tointeger(tolua_s, 4);
	if(nCount < 0)
		nCount = 1;

	int nPos = XTimerManger::RegTime(fTick, tolua_s, nFunc, nParam, nCount);
	lua_pushinteger(tolua_s, nPos);
	return 1;
	XFUNC_END_RETURN(int, res, 0);
}

int DelTimer(lua_State* tolua_s)
{
	XFUNC_START();
	if(!tolua_s)
		return 0;
	int nTop = lua_gettop(tolua_s);
	if(nTop != 1)
	{
		CCLOG("DelTimer param must be 1!!");
		return 0;
	}
	XTimerManger::DelTimeById(lua_tointeger(tolua_s, 1));
	XFUNC_END_RETURN(int, res, 0);
}


int tolua_XTimerManger_Open(lua_State* tolua_s)
{
	XFUNC_START();
	lua_register(tolua_s, "RegS_Timer", RegS_Timer);
	lua_register(tolua_s, "RegM_Timer", RegM_Timer);
	lua_register(tolua_s, "DelTimer", DelTimer);
	XFUNC_END_RETURN(int, res, 0);
}