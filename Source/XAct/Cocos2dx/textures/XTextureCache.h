#ifndef __XTEXTURECACHE_H__
#define __XTEXTURECACHE_H__
#include "cocos2d.h"
#include "../../XHelper/XHelper.h"

NS_CC_BEGIN

class XTexture2D;
class XTextureCache :
	public CCTextureCache,
	public XSingleton<XTextureCache>		// Memory Pool
{
public:
	XTextureCache() {
		XMemLog::m_dwNew += sizeof(XTextureCache);
	}
	virtual ~XTextureCache() {
		XMemLog::m_dwNew -= sizeof(XTextureCache);
	}

	/** Returns a Texture2D object given an file image
    * If the file image was not previously loaded, it will create a new CCTexture2D
    *  object and it will return it. It will use the filename as a key.
    * Otherwise it will return a reference of a previously loaded image.
    * Supported image extensions: .png, .bmp, .tiff, .jpeg, .pvr, .gif
    */
    virtual CCTexture2D* addImage(const char* fileimage);
    
	CCTexture2D*	addETC1Image(const char* fileimage);

	CCTexture2D*	addS3TCImage(const char* fileimage);

	bool InitTexture(const char* fileName, CCImage* pImg, CCTexture2D* pTex);
	bool InitTexture(const char* fileName, XTexture2D* pTex, XTexture2D* &pTexKtxAlpha);

	void dumpCachedTextureInfo();
};


// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __XTEXTURECACHE_H__
