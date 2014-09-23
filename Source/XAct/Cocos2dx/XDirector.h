#ifndef __XDIRECTOR_H__
#define __XDIRECTOR_H__
/*
	祯驱动的导演类，兼容Cocos2dx的类，同时支持扩展的自定义类
	by 刘潇 @ 2013-04-08
*/

#include "cocos2d.h"
#include "../XHelper/XSystem.h"
#include "../XHelper/XSingleton.h"

NS_CC_BEGIN

#define DEBUG_INFO_MAX		(2048)

class XDirector : public CCDirector, public XSingleton<XDirector>
{
public:
    XDirector(void)
	{
		m_bInvalid = false;
		m_fRunTime = 0.f;
		m_fLastFrameTime = 0.f;
		m_dwDrawFrame = 0;
		m_dwFrameNum = 0;
		m_bIsXScene = false;
		m_dwObjRunNum = 0;
		m_pobOpenGLView = NULL;
		XMemLog::m_dwNew += sizeof(XDirector);
	}
    virtual ~XDirector(void)
	{
		XMemLog::m_dwNew -= sizeof(XDirector);
	}
    static XDirector* sharedDirector(void);
public:
    virtual void setAnimationInterval(double dValue);
    virtual void startAnimation(void);
    virtual void stopAnimation();
public:
    virtual bool init(void);
    virtual void mainLoop(void);
    void drawScene(void);
	virtual bool run(const cc_timeval& stNow);
	void end(void);

	void jumpToUpdateScene();
public:
	inline float GetRunTime() {
		return m_fRunTime;
	}
	inline float GetLastFrameTime(){
		return m_fLastFrameTime;
	}
protected:
    void purgeDirector();
	void calculateDeltaTime(void);
    void setNextScene(void);
protected:
	static bool			m_bInited;
    bool				m_bInvalid;
    struct cc_timeval	m_stNow;
	float				m_fRunTime;
	float				m_fLastFrameTime;		// [xuyabo]两帧之间的真实时间
	unsigned long		m_dwDrawFrame;
	unsigned long		m_dwFrameNum;
	bool				m_bIsXScene;
	unsigned long		m_dwObjRunNum;
public:
	static void DebugLog(const char * pszFormat, ...);
	enum {
		eShowNone = 0,
		eShowFps,
		eShowDebug,
		eShowMemory,

		eShowNum
	};
	static void ShowDebug(unsigned char byType);
protected:
	static char				m_szDebugInfo[DEBUG_INFO_MAX];
	static char*			m_szNewInfo;
	static CCLabelTTF*		m_pLabel;
	static unsigned char	m_byDirty;
	static unsigned char	m_byShowDebug;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
public:
	static volatile unsigned char	m_bySDKInit;
#endif
};

NS_CC_END

#endif // __XDIRECTOR_H__
