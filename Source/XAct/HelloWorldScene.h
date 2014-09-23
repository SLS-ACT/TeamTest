#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "common.h"

class HelloWorld : public cocos2d::XLayer
{
public:
	virtual ~HelloWorld();
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual unsigned long OnFrame();
	virtual void visit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::XScene* scene();
    
    // a selector callback
	void menuCloseCallback(CCObject* pSender);

	// a selector callback
	void menuDebugCallback(CCObject* pSender);

	// a selector callback
	void menuEffectCallback(CCObject* pSender);

	// a selector callback
	void menuCameraCallback(CCObject* pSender);

	// a selector callback
	void menuWeatherCallback(CCObject* pSender);
    
	// implement the "static node()" method manually
	static HelloWorld *create(void);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void keyBackClicked(void);
	virtual void keyMenuClicked(void);
#endif

	//unsigned long XThreadProc(long param, volatile bool& bExit)
	//{
	//	while(true)
	//	{
	//		//Sleep(10);
	//		m_xthread.SleepMs(100);
	//		if(bExit)
	//			break;
	//	}
	//	return 0;
	//}

	//OGDP::XThread<HelloWorld>	m_xthread;

	unsigned long			m_dwReloadSt;
	float					m_fPerc;
	static cocos2d::XSprite*		m_pFrame;
	static cocos2d::XSprite*		m_pLoading;
	cocos2d::CCLabelTTF*	m_pPerc;
	cocos2d::CCLabelTTF*	m_pInfo;
	float					m_fRotation;
protected:	// Thread for system info
	OGDP::XThread<HelloWorld>		m_stCheckThread;
	OGDP::XThreadLock				m_stCheckLock;
	unsigned long XThreadCheck(long param, volatile bool& bExit);
public:
	volatile unsigned char			m_byCPU;		// 程序占用的CPU百分比
	volatile unsigned long			m_dwMemPhy;		// 程序占用的物理内存
	volatile unsigned long			m_dwMemVtl;		// 程序占用的虚拟内存
};

#endif // __HELLOWORLD_SCENE_H__
