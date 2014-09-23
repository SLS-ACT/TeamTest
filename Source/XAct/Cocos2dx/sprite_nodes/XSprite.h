#ifndef __XSPITE_H__
#define __XSPITE_H__
/*
	使用内存池的精灵类
	by 刘潇 @ 2013-04-08
*/

#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
#include "../../XEvent/XEvent.h"
#include "../../XBaseObj/XLogicObj.h"
#include "../XShader/XShader.h"
#include "../textures/XTexture2D.h"

class XSprCallback;

NS_CC_BEGIN

class XSprite :
	public XEventObj,
	public CCSprite,
	public XMemPool<XSprite>		// Memory Pool
{
public:
    /// @{
    /// @name Creators
    static XSprite* create();
	static XSprite* create(const char *pszFileName, bool bThread = false);
	static XSprite* createWithXA8(const char *szXA8File, unsigned short wFrame);
    static XSprite* create(const char *pszFileName, const CCRect& rect);
    static XSprite* createWithTexture(CCTexture2D *pTexture);
    static XSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect, unsigned char byAntiAlias/* = 0x00*/);	// 是否开启抗锯齿
    static XSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static XSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    /// @}  end of creators group
    virtual bool initWithFile(const char *pszFilename);
	virtual bool initWithFile(const char *pszFilename, const CCRect& rect);
	virtual bool initWithTexture(CCTexture2D *pTex, CCTexture2D *pTexA, CCTexture2D *pTexP);
    /// @{
    /// @name Initializers
    XSprite(void);
    virtual ~XSprite(void);
	/// @} end of initializers
public:
	virtual void setPosition(const CCPoint& pos);
	virtual void draw();
	inline void EndbledScale(unsigned char byScale) {
		_byScale = byScale;
	}
	inline virtual void setScale(float fScale) {
		if(100 != _byScale)
			fScale *= (_byScale * 0.01f);
		if(fScale != m_fScaleX || fScale != m_fScaleY)
			CCSprite::setScale(fScale);
	}
	inline virtual void setScaleX(float fScaleX) {
		if(100 != _byScale)
			fScaleX *= (_byScale * 0.01f);
		if(fScaleX != m_fScaleX)
			CCSprite::setScaleX(fScaleX);
	}
	inline virtual void setScaleY(float fScaleY) {
		if(100 != _byScale)
			fScaleY *= (_byScale * 0.01f);
		if(fScaleY != m_fScaleY)
			CCSprite::setScaleY(fScaleY);
	}
	void setFlipX(bool bFlipX);
	void setFlipY(bool bFlipY);
	inline void SetShaderOnce(XGLProgram* pGLP) {
		m_pShaderOnce = pGLP;
	}
	inline void EnabledAntiAlias() {
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		_byAntiAlias = 0xFF;
#		else
		_byAntiAlias = 0xFF;
#		endif
	}
	virtual void setOpacity(GLubyte opacity);
	virtual void setColor(const ccColor3B& color3);
protected:
	virtual void setTextureCoords(CCRect rect);
	virtual void updateTransform(void);
protected:
	XGLProgram*		m_pShaderOnce;
	unsigned char	_byScale;
	unsigned char	_byAntiAlias;	// 是否开启抗锯齿
	XSprCallback*	_pFileReload;
public:	// XA8
	enum {
		eTexColor	= 0,
		eTexAlpha	= 1,
		eTexPal		= 2,
	};
	unsigned short	m_wXA8Frame;
	CCTexture2D*	m_pTexPal;
	CCTexture2D*	m_pTexAlpha;
	short			m_nOffsetX;
	short			m_nOffsetY;
protected:	// Thread
	bool			_bResLoaded;
	bool			_bCleanUpThread;
public:
	inline bool Loaded() {
		return _bResLoaded;
	}
	void ResHandle(long lParam);
	bool initWithFileThread(const char* fileImage);
public:	// Memory Pool
	static const unsigned short _XMP_Alloc_Num = 64;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo);
	virtual bool OnCreate();
	virtual void OnRelease();
	virtual void cleanup();
};

class XScrSprite :
	public XSprite
{
public:
	enum{
		eShaderNone = 0,
		eShaderBloom,			// 全屏柔光
		eShaderHeat,			// 热扭曲

		eShaderNum
	};
	static unsigned char GetShaderType() {
		return m_byShaderType;
	}
	static void SetShaderType(unsigned char byType) {
		m_byShaderType = byType;
	}
	static void SetShaderParam(unsigned char byParam) {
		m_byShaderParam = byParam;
	}
	// 使用简易全屏特效【一次渲染|高效】|【多次渲染|华丽】
	static bool UseSimpleMode() {
		return (0 != m_bySimpleMode);
	}
	static void UseSimpleMode(bool bUsed) {
		m_bySimpleMode = bUsed ? 0xFF : 0x00;
	}
	// 全屏缩放
	static void ScrScale(float fScale) {
		m_fScrScale = fScale;
	}
	static void Reload() {
		m_byReloadID = m_byScrSprNum;
	}
protected:
	static unsigned char	m_byShaderType;
	static unsigned char	m_byShaderParam;
	static unsigned char	m_bySimpleMode;
	static float			m_fScrScale;
	static unsigned char	m_byScrSprNum;
	static unsigned char	m_byReloadID;
public:
	XScrSprite(void);
	virtual ~XScrSprite(void);
public:
	void BeginRTT();
	void GetScreen();
	void OnDraw();
protected:
	virtual void draw();
protected:
	XScrTexture		m_stScrTex;
	CCRenderTexture	m_stRTT;
	unsigned char	m_byRTTInited;
	unsigned char	m_byID;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __XSPITE_H__
