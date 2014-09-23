
#ifndef _XTIMERMANGER_H_
#define _XTIMERMANGER_H_

/*
	ʱ�������
	by ���ǲ�
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

#define TIMERINFO_MAXNUM		(32)		// ��ʱ֧�ֵ�ע�ᶨʱ������

// ��ʱ���ṹ��
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

	float fEndTick;				// ��ʱ������ʱ��
	float fTick;				// ÿ�ζ�ʱ����ʱ��
	lua_State* pLs;				// lua�ص�����
	int nFunc;					// ʱ�䵽��Ļص�����
	int nParam;					// �ص������Ĳ���
	int nCount;					// ѭ��������0������ѭ��
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