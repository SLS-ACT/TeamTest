#ifndef __XSCALE9SRPITE_H__
#define __XSCALE9SRPITE_H__
// 九宫格精灵类
#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
NS_CC_BEGIN

class XSprite;
class XSpriteBatchNode;
class XScale9Sprite :
		public CCSprite,
		public XMemPool<XScale9Sprite> // 内存池
{
public:
	static const unsigned short _XMP_Alloc_Num = 32;
public:
	XScale9Sprite(void);
	virtual ~XScale9Sprite(void);
public:
	static XScale9Sprite* createWithSpriteFrame(CCSpriteFrame* pSpriteFrame, const CCRect& capInsets);
	static XScale9Sprite* createWithSpriteFrameName(const char* pszSpriteFrameName, const CCRect& capInsets);
public:
	virtual void setSize(float fW, float fH);
	virtual void setPosition(const CCPoint& pos);
	virtual void setOpacity(unsigned char byApha);
	virtual void setScale(float fScale);
	virtual void visit();
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo);
	virtual bool OnCreate();
	virtual void OnRelease();
	virtual void cleanup();
protected:
	XSpriteBatchNode* m_pDrawSprite;
	enum{
		eUp_Left = 0,
		eUp_Center,
		eUp_Right,
		eMid_Left,
		eMid_Center,
		eMid_Right,
		eDown_Left,
		eDown_Center,
		eDown_Right,
		eNum
	};
	CCRect	m_stRcCenter;
	CCRect	m_stRcTexture;
	bool	m_bIsdirty;
	float	m_fWidth;
	float	m_fHeight;
};
NS_CC_END
#endif /* __XSCALE9SRPITE_H__ */