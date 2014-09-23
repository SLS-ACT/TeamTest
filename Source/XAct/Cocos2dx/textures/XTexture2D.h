#ifndef __XTEXTURE2D_H__
#define __XTEXTURE2D_H__
#include "cocos2d.h"
#include "../../XHelper/XHelper.h"

NS_CC_BEGIN

class XSprFrmInfo :
	public XEventObj
{
public:
	XSprFrmInfo() : _byReload(0x00), pTex(NULL) {}
public:
	void ResHandle(long lParam);
	unsigned char	_byReload;
	CCTexture2D*	pTex;
	char			szTexPath[MAX_PATH];
};

class XTexture2D :
	public XEventObj,
	public CCTexture2D
{
public:
	XTexture2D() {
		_byReload = 0x00;
		XMemLog::m_dwNew += sizeof(XTexture2D);
	}
	virtual ~XTexture2D() {
		XMemLog::m_dwNew -= sizeof(XTexture2D);
	}
public:
	void ResHandle(long lParam);
	unsigned char	_byReload;
public:
	bool initWithETC1File(const char* file);
	bool initWithS3TCFile(const char* file);
};

class XScrTexture :
	public XTexture2D
{
public:
	XScrTexture() {
		XMemLog::m_dwNew += sizeof(XScrTexture);
	}
	virtual ~XScrTexture();
public:
	void InitWithBackBuffer();
	void GetBackBuffer();
};

NS_CC_END

#endif // __XTEXTURE2D_H__
