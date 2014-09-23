#ifndef __XSCENE_H__
#define __XSCENE_H__
/*
	ÿ������OnFrame�ĳ����࣬����ʹ��XDirector
	by ���� @ 2013-04-08
*/

#include "cocos2d.h"
#include "../../XBaseObj/XLogicObj.h"

NS_CC_BEGIN

class XLayer;

class XScene :
	public CCScene,
	public XLogicObjMgr<XLayer>		// Manager for XLayer
{
public:
    XScene();
    virtual ~XScene();
	static XScene *create(void);
	XLayer* getRunLayer();
public:
	virtual unsigned long OnFrame();
public:
    bool init();
};

NS_CC_END

#endif // __XSCENE_H__
