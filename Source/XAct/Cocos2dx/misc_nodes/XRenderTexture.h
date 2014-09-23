#ifndef __XRENDERTEXTURE_H__
#define __XRENDERTEXTURE_H__
/*
	‰÷»æµΩÃ˘Õº
	by ¡ı‰Ï @ 2013-05-30
*/

#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
#include "../XShader/XShader.h"

NS_CC_BEGIN

class XRenderTexture :
	public CCRenderTexture 
{
public:
    XRenderTexture();
	virtual ~XRenderTexture();
public:
    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
    static XRenderTexture * create(int w ,int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);
    /** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    static XRenderTexture * create(int w, int h, CCTexture2DPixelFormat eFormat);
    /** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
    static XRenderTexture * create(int w, int h);
    /** initializes a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat);
    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
	bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);
};

// end of textures group
/// @}

NS_CC_END

#endif //__XRENDER_TEXTURE_H__
