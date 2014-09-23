#include "../../Common.h"

XCPP_DEFINED();

NS_CC_BEGIN

XRenderTexture::XRenderTexture()
{
	XMemLog::m_dwNew += sizeof(XRenderTexture);
}

XRenderTexture::~XRenderTexture()
{
	XMemLog::m_dwNew -= sizeof(XRenderTexture);
	// Release by mem pool
	m_pSprite = NULL;
}

XRenderTexture * XRenderTexture::create(int w, int h, CCTexture2DPixelFormat eFormat)
{
    XRenderTexture *pRet = new XRenderTexture();
    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat))
    {
        //pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

XRenderTexture * XRenderTexture::create(int w ,int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat)
{
    XRenderTexture *pRet = new XRenderTexture();
    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat, uDepthStencilFormat))
    {
        //pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

XRenderTexture * XRenderTexture::create(int w, int h)
{
    XRenderTexture *pRet = new XRenderTexture();
    if(pRet && pRet->initWithWidthAndHeight(w, h, kCCTexture2DPixelFormat_RGBA8888, 0))
    {
        //pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool XRenderTexture::initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat)
{
    return initWithWidthAndHeight(w, h, eFormat, 0);
}

bool XRenderTexture::initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat)
{
    CCAssert(eFormat != kCCTexture2DPixelFormat_A8, "only RGB and RGBA formats are valid for a render texture");
    bool bRet = false;
    void *data = NULL;
    do 
    {
        w = (int)(w * CC_CONTENT_SCALE_FACTOR());
        h = (int)(h * CC_CONTENT_SCALE_FACTOR());

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);

        // textures must be power of two squared
        unsigned int powW = 0;
        unsigned int powH = 0;

        if (CCConfiguration::sharedConfiguration()->supportsNPOT())
        {
            powW = w;
            powH = h;
        }
        else
        {
            powW = xNextPOT(w);
            powH = xNextPOT(h);
        }

        data = malloc((int)(powW * powH * 4));
        CC_BREAK_IF(! data);

        memset(data, 0, (int)(powW * powH * 4));
        m_ePixelFormat = eFormat;

        m_pTexture = new CCTexture2D();
        if (m_pTexture)
        {
            m_pTexture->initWithData(data, (CCTexture2DPixelFormat)m_ePixelFormat, powW, powH, CCSizeMake((float)w, (float)h));
        }
        else
        {
            break;
        }
        GLint oldRBO;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
        
        if (CCConfiguration::sharedConfiguration()->checkForGLExtension("GL_QCOM"))
        {
            m_pTextureCopy = new CCTexture2D();
            if (m_pTextureCopy)
            {
                m_pTextureCopy->initWithData(data, (CCTexture2DPixelFormat)m_ePixelFormat, powW, powH, CCSizeMake((float)w, (float)h));
            }
            else
            {
                break;
            }
        }

        // generate FBO
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

        // associate texture with FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName(), 0);

        if (uDepthStencilFormat != 0)
        {
            //create and attach depth buffer
            glGenRenderbuffers(1, &m_uDepthRenderBufffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthRenderBufffer);
            glRenderbufferStorage(GL_RENDERBUFFER, uDepthStencilFormat, (GLsizei)powW, (GLsizei)powH);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);

            // if depth format is the one with stencil part, bind same render buffer as stencil attachment
            if (uDepthStencilFormat == GL_DEPTH24_STENCIL8)
            {
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
            }
        }

        // check if it worked (probably worth doing :) )
        CCAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach texture to framebuffer");

        m_pTexture->setAliasTexParameters();

        // retained
        setSprite(XSprite::createWithTexture(m_pTexture));

        m_pTexture->release();
        m_pSprite->setScaleY(-1);

        ccBlendFunc tBlendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        m_pSprite->setBlendFunc(tBlendFunc);

        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
        
        // Diabled by default.
        m_bAutoDraw = false;
        
        // add sprite for backward compatibility
        addChild(m_pSprite);
        
        bRet = true;
    } while (0);
    
    CC_SAFE_FREE(data);
    
    return bRet;
}

NS_CC_END
