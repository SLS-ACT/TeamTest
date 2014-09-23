#ifndef __XSPRITEBATCHNODE_H__
#define __XSPRITEBATCHNODE_H__
/*
	使用内存池的SpriteBatchNode类
	by 刘潇 @ 2013-04-11
*/

#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
#include "../../XEvent/XEvent.h"
#include "../XShader/XShader.h"

NS_CC_BEGIN

class XSBNChildInfo :
	public XMemPool<XSBNChildInfo>		// Memory Pool
{
public:
	unsigned char	byFlipX;
	float			fPosX;
	float			fPosY;
	CCRect			stRect;
	XSBNChildInfo*	_pNext;
public:	// Memory Pool
	static const unsigned short _XMP_Alloc_Num = 1024;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo) {
		CCLOG("[XSBNChildInfo::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}
	virtual bool OnCreate() {
		byFlipX = 0;
		fPosY = 0.f;
		fPosX = 0.f;
		stRect = CCRectMake(0,0,0,0);
		_pNext = NULL;
		return true;
	}
	virtual void OnRelease() {
	}
};

#define xDefaultSpriteBatchCapacity   (32)

class XTexture2D;
class XSpriteBatchNode :
	public XEventObj,
	public CCSpriteBatchNode,
	public XMemPool<XSpriteBatchNode>		// Memory Pool
{
public:
	static const unsigned short _XMP_Alloc_Num = 256;
public:
	XSpriteBatchNode();
    virtual ~XSpriteBatchNode();
    /** creates a CCSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
    static XSpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity);
    static XSpriteBatchNode* createWithTexture(CCTexture2D* tex) {
        return XSpriteBatchNode::createWithTexture(tex, xDefaultSpriteBatchCapacity);
    }
    /** creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */
    static XSpriteBatchNode* create(const char* fileImage, unsigned int capacity);
    static XSpriteBatchNode* create(const char* fileImage) {
        return XSpriteBatchNode::create(fileImage, xDefaultSpriteBatchCapacity);
	}

	bool initWithFile(const char* fileImage, unsigned int capacity);
	void ResHandle(long lParam);
public:
	virtual void setPosition(const CCPoint& pos);
	virtual void setPosition(float fX, float fY);
	virtual void visit(void);
	virtual void draw(void);
	inline bool Loaded() {
		return _bResLoaded;
	}
	inline void SetShaderOnce(XGLProgram* pGLP) {
		m_pShaderOnce = pGLP;
	}
protected:
	typedef XSinglyLink<XSBNChildInfo> XChildrenInfo;
	XChildrenInfo	m_lstCInfo;
	unsigned char	m_byIsCreate;
	XGLProgram*		m_pShaderOnce;
public:		// Func
	virtual void setOpacity(GLubyte opacity);
	virtual void setColor(const ccColor3B& color3);
	virtual void setAntiAliasTexParameters();
	virtual void cleanup();
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo);
	virtual bool OnCreate();
	virtual void OnRelease();
public:
	inline void EndbledScale(unsigned char byScale) {
		_byScale = byScale;
	}
	inline virtual void setScale(float fScale) {
		if(_byScale)
			fScale *= (100.f / _byScale);
		if(fScale != m_fScaleX || fScale != m_fScaleY)
			CCSpriteBatchNode::setScale(fScale);
	}
	inline virtual void setScaleX(float fScaleX) {
		if(_byScale)
			fScaleX *= (100.f / _byScale);
		if(fScaleX != m_fScaleX)
			CCSpriteBatchNode::setScaleX(fScaleX);
	}
	inline virtual void setScaleY(float fScaleY) {
		if(_byScale)
			fScaleY *= (100.f / _byScale);
		if(fScaleY != m_fScaleY)
			CCSpriteBatchNode::setScaleY(fScaleY);
	}
	inline virtual void setChildrenScaleY(float fScaleY) {
		_fChildrenScaleY = fScaleY;
	}
	inline void SetChangeColor(unsigned char byChange)
	{
		_byChangeColor = byChange;
	}
	inline void EnabledAntiAlias() {
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		_byAntiAlias = 0xFF;
#		else
		_byAntiAlias = 0xFF;
#		endif
	}
protected:
	XShader*		m_pShaderKTX;
	XTexture2D*		m_pTexKtxAlpha;
	GLubyte			_realOpacity;
	ccColor3B		_realColor;
	bool			_bResLoaded;
	bool			_bCleanUpThread;
	unsigned char	_byScale;
	float			_fChildrenScaleY;
	unsigned char	_byAntiAlias;	// 是否开启抗锯齿
	unsigned char	_byChangeColor;
public:	// child info
	inline void AddChildInfo(bool bFlipX, float fPosX, float fPosY, CCRect& stRc)
	{
		XSBNChildInfo* pInfo = XMP_CREATE(XSBNChildInfo);
		if (!pInfo)
		{
			return;
		}
		pInfo->byFlipX = bFlipX ? 0xFF : 0x00;
		pInfo->fPosX = fPosX;
		pInfo->fPosY = fPosY;
		pInfo->stRect= stRc;
		_stChildren.AddObj(pInfo);
	}
protected:
	XSinglyLink<XSBNChildInfo>	_stChildren;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __XSPRITEBATCHNODE_H__
