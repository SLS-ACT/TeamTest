#ifndef __XLAYER_H__
#define __XLAYER_H__
/*
	每祯调用OnFrame的Layer类，必须使用XScene
	by 刘潇 @ 2013-04-09
*/

#include "cocos2d.h"
#include "../../XBaseObj/XLogicObj.h"
#include "../../XHelper/XHelper.h"

NS_CC_BEGIN

class XLayer :
	public CCLayer,
	public XLinkItem<XLayer>,	// Magaged in XScene
	public XLogicObj			// OnFrame
{
public:
    XLayer();
    virtual ~XLayer();
    virtual bool init();
public:
	virtual unsigned long OnFrame();
public:
	virtual void onEnter();
	virtual void onExit();
};

NS_CC_END

#endif // __XLAYER_H__

