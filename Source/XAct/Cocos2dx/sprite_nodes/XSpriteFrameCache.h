#ifndef __XSPITEFRAMECACHE_H__
#define __XSPITEFRAMECACHE_H__
/*
	使用内存池的精灵帧缓存类
	by 刘潇 @ 2014-04-22
*/

#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
#include "../textures/XTexture2D.h"

NS_CC_BEGIN

#define XSPC_PLIST_MAX		(16)

class XSpriteFrameCache :
	public CCSpriteFrameCache,
	public XSingleton<XSpriteFrameCache>
{
public:
	XSpriteFrameCache() : m_byUsed(0) {}
public:
	virtual void addSpriteFramesWithFile(const char *pszPlist);
	void Reload();
protected:
	XSprFrmInfo		m_astSFInfo[XSPC_PLIST_MAX];
	unsigned char	m_byUsed;
};

NS_CC_END

#endif // __XSPITEFRAMECACHE_H__
