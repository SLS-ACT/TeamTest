#include "../../Common.h"
#include "../../XConfig.h"
#include "../XLoader/XLoader.h"

XCPP_DEFINED();

NS_CC_BEGIN

void XSprite::OnException(const char* szFile, const int nLine, const char* szInfo) {
	CCLOG("[XSprite::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
}

bool XSprite::OnCreate()
{
	XFUNC_START();
	m_pShaderOnce = NULL;
	_byScale = 100;
	_byAntiAlias = 0x00;
	_pFileReload = NULL;
	_bResLoaded = true;
	_bCleanUpThread = false;
	m_wXA8Frame = 0xFFFF;
	m_pTexPal = NULL;
	m_pTexAlpha = NULL;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_obPosition.x = 1.f;
	m_obPosition.y = 1.f;
	setPosition(ccp(1.f,1.f));
	setPosition(ccp(0.f,0.f));
	setAnchorPoint(ccp(0.f,0.f));
	setAnchorPoint(ccp(0.5f,0.5f));
	setScale(0.5f);
	setScale(1.f);
	setRotation(1.f);
	setRotation(0.f);
	m_pobBatchNode = NULL;
	m_pobTextureAtlas = NULL;
	setOpacity(0x00);
	setOpacity(0xFF);
	m_obUnflippedOffsetPositionFromCenter = CCPointZero;
	m_obOffsetPosition = CCPointZero;
	//setFlipX(true);
	//setFlipY(true);
	memset(&m_sQuad, 0, sizeof(m_sQuad));
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XSprite::OnRelease()
{
	XFUNC_START();
	CC_SAFE_RELEASE(m_pobTexture);
	m_pobTexture = NULL;
	CC_SAFE_RELEASE(m_pTexPal);
	m_pTexPal = NULL;
	CC_SAFE_RELEASE(m_pTexAlpha);
	m_pTexAlpha = NULL;
	setPosition(ccp(1.f,1.f));
	setPosition(ccp(0.f,0.f));
	XFUNC_END();
}

void XSprite::cleanup()
{
	XFUNC_START();
	if(!_bResLoaded)
	{
		_bCleanUpThread = true;
		return;
	}
	if(_pFileReload)
	{
		XResLoader::Instance().DelSpr(_pFileReload);
		_pFileReload = NULL;
	}
	_bResLoaded = false;
	CCSprite::cleanup();
	if(XMemPool<XSprite>::m_pXMPParent)
	{
		XSprite* pSprite = this;
		try
		{
			XMP_RELEASE(XSprite, pSprite);
		}
		catch (...)
		{
		}
	}
	XFUNC_END();
}

XSprite* XSprite::createWithTexture(CCTexture2D *pTexture)
{
	XFUNC_START();
    XSprite *pobSprite = XMP_CREATE(XSprite);
    if (pobSprite && pobSprite->CCSprite::initWithTexture(pTexture))
    {
        return pobSprite;
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect, unsigned char byAntiAlias/* = 0x00*/)
{
	XFUNC_START();
	XSprite *pobSprite = XMP_CREATE(XSprite);
	if(pobSprite)
	{
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		pobSprite->_byAntiAlias = byAntiAlias;
#		else
		pobSprite->_byAntiAlias = byAntiAlias;
#		endif
		if(pobSprite->CCSprite::initWithTexture(pTexture, rect))
		{
			return pobSprite;
		}
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::create(const char *pszFileName, bool bThread/* = false*/)
{
	XFUNC_START();
	XSprite *pobSprite = XMP_CREATE(XSprite);
	if(bThread)
	{
		if (pobSprite && pobSprite->initWithFileThread(pszFileName))
		{
			return pobSprite;
		}
	}
	else
	{
		if (pobSprite && pobSprite->initWithFile(pszFileName))
		{
			pobSprite->getTexture()->setAntiAliasTexParameters();
			return pobSprite;
		}
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::createWithXA8(const char *szXA8File, unsigned short wFrame)
{
	XFUNC_START();
	XSprite *pobSprite = XMP_CREATE(XSprite);
	if (pobSprite)
	{
		pobSprite->m_wXA8Frame = wFrame;
		if(pobSprite->initWithFileThread(szXA8File))
			return pobSprite;
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::create(const char *pszFileName, const CCRect& rect)
{
	XFUNC_START();
	XSprite *pobSprite = XMP_CREATE(XSprite);
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
	{
        return pobSprite;
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
	XFUNC_START();
	XSprite *pobSprite = XMP_CREATE(XSprite);
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        return pobSprite;
	}
	XMP_RELEASE(XSprite, pobSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

XSprite* XSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	XFUNC_START();
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    if(!pFrame)
	{
		CCLOG("%s is not exit!!!!", pszSpriteFrameName);
	}
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#else
	 if(!pFrame)
		return NULL;
#endif

	return createWithSpriteFrame(pFrame);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

bool XSprite::initWithFile(const char *pszFilename)
{
	XFUNC_START();
	bool bRes = CCSprite::initWithFile(pszFilename);
	if(bRes)
	{
		_pFileReload = XResLoader::Instance().RegSpr(pszFilename, m_pobTexture);
		if(_pFileReload)
		{
			if(m_pobTexture != _pFileReload->pTex)
			{
				CCLOG("【 - ERROR - 】file %s reload!", pszFilename);
				setTexture(_pFileReload->pTex);
			}
		}
		else
			CCLOG("【 - ERROR - 】file %s reg failed!", pszFilename);
	}
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}

bool XSprite::initWithFile(const char *pszFilename, const CCRect& rect)
{
	XFUNC_START();
	bool bRes = CCSprite::initWithFile(pszFilename, rect);
	if(bRes)
	{
		_pFileReload = XResLoader::Instance().RegSpr(pszFilename, m_pobTexture);
		if(_pFileReload)
		{
			if(m_pobTexture != _pFileReload->pTex)
			{
				CCLOG("【 - ERROR - 】file %s reload!", pszFilename);
				setTexture(_pFileReload->pTex);
			}
		}
		else
			CCLOG("【 - ERROR - 】file %s reg failed!", pszFilename);
	}
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}

bool XSprite::initWithTexture(CCTexture2D *pTex, CCTexture2D *pTexA, CCTexture2D *pTexP)
{
	XFUNC_START();
	bool bRes = CCSprite::initWithTexture(pTex);
	if(bRes)
	{
		m_pTexAlpha = pTexA;
		if(m_pTexAlpha)
			m_pTexAlpha->retain();
		m_pTexPal = pTexP;
		if(m_pTexPal)
			m_pTexPal->retain();
	}
	return bRes;
	XFUNC_END_RETURN(bool, res, false);
}

XSprite* XSprite::create()
{
	XFUNC_START();
	XSprite *pSprite = XMP_CREATE(XSprite);
    if (pSprite && pSprite->init())
    {
        return pSprite;
	}
	XMP_RELEASE(XSprite, pSprite);
	XFUNC_END_RETURN(XSprite*, ptr, NULL);
}

void XSprite::setTextureCoords(CCRect rect)
{
	XFUNC_START();
	if(m_pobBatchNode && !m_pobTextureAtlas)
	{
		CCLOG("【 - ERROR - 】无效的m_pobTextureAtlas");
		return;
	}
	rect = CC_RECT_POINTS_TO_PIXELS(rect);

	CCTexture2D *tex = m_pobBatchNode ? m_pobTextureAtlas->getTexture() : m_pobTexture;
	if (! tex)
	{
		return;
	}

	float atlasWidth = (float)tex->getPixelsWide();
	float atlasHeight = (float)tex->getPixelsHigh();
	if(fIsZero(atlasWidth))
	{
		//CCLOG("【 - ERROR - 】无效的贴图宽度");
		atlasWidth = 1.f; 
	}
	if(fIsZero(atlasHeight))
	{
		//CCLOG("【 - ERROR - 】无效的贴图高度");
		atlasHeight = 1.f; 
	}

	float left, right, top, bottom;
#	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	float fOffset = 1.0f;
#	else
	float fOffset = 1.0f;
#	endif
	if(m_pTexPal)
		_byAntiAlias = 0x00;
	if (m_bRectRotated)
	{
		if(_byAntiAlias)
		{
			left    = rect.origin.x/atlasWidth;
			right    = (rect.origin.x+rect.size.height) / atlasWidth;
			top        = (2*rect.origin.y+fOffset)/(2*atlasHeight);
			bottom    = top+(rect.size.width*2-fOffset*2)/(2*atlasHeight);
		}
		else
		{
			left    = rect.origin.x/atlasWidth;
			right    = (rect.origin.x+rect.size.height) / atlasWidth;
			top        = rect.origin.y/atlasHeight;
			bottom    = (rect.origin.y+rect.size.width) / atlasHeight;
		}

		if (m_bFlipX)
		{
			CC_SWAP(top, bottom, float);
		}

		if (m_bFlipY)
		{
			CC_SWAP(left, right, float);
		}

		m_sQuad.bl.texCoords.u = left;
		m_sQuad.bl.texCoords.v = top;
		m_sQuad.br.texCoords.u = left;
		m_sQuad.br.texCoords.v = bottom;
		m_sQuad.tl.texCoords.u = right;
		m_sQuad.tl.texCoords.v = top;
		m_sQuad.tr.texCoords.u = right;
		m_sQuad.tr.texCoords.v = bottom;
	}
	else
	{
		if(_byAntiAlias)
		{
			left    = rect.origin.x/atlasWidth;
			right    = (rect.origin.x + rect.size.width) / atlasWidth;
			top        = (2*rect.origin.y+fOffset)/(2*atlasHeight);
			bottom    = top + (rect.size.height*2-fOffset*2)/(2*atlasHeight);
		}
		else
		{
			left    = rect.origin.x/atlasWidth;
			right    = (rect.origin.x + rect.size.width) / atlasWidth;
			top        = rect.origin.y/atlasHeight;
			bottom    = (rect.origin.y + rect.size.height) / atlasHeight;
		}

		if(m_bFlipX)
		{
			CC_SWAP(left,right,float);
		}

		if(m_bFlipY)
		{
			CC_SWAP(top,bottom,float);
		}

		m_sQuad.bl.texCoords.u = left;
		m_sQuad.bl.texCoords.v = bottom;
		m_sQuad.br.texCoords.u = right;
		m_sQuad.br.texCoords.v = bottom;
		m_sQuad.tl.texCoords.u = left;
		m_sQuad.tl.texCoords.v = top;
		m_sQuad.tr.texCoords.u = right;
		m_sQuad.tr.texCoords.v = top;
	}
	XFUNC_END();
}

void XSprite::updateTransform(void)
{
	XFUNC_START();
    CCAssert(m_pobBatchNode, "updateTransform is only valid when CCSprite is being rendered using an CCSpriteBatchNode");

    // recalculate matrix only if it is dirty
    if( isDirty() ) {

        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !m_bVisible || ( m_pParent && m_pParent != m_pobBatchNode && ((XSprite*)m_pParent)->m_bShouldBeHidden) )
        {
            m_sQuad.br.vertices = m_sQuad.tl.vertices = m_sQuad.tr.vertices = m_sQuad.bl.vertices = vertex3(0,0,0);
            m_bShouldBeHidden = true;
        }
        else 
        {
            m_bShouldBeHidden = false;

            if( ! m_pParent || m_pParent == m_pobBatchNode )
            {
                m_transformToBatch = nodeToParentTransform();
            }
            else 
            {
                CCAssert( dynamic_cast<CCSprite*>(m_pParent), "Logic error in CCSprite. Parent must be a CCSprite");
                m_transformToBatch = CCAffineTransformConcat( nodeToParentTransform() , ((XSprite*)m_pParent)->m_transformToBatch );
            }

            //
            // calculate the Quad based on the Affine Matrix
            //

            CCSize size = m_obRect.size;

            float x1 = m_obOffsetPosition.x;
            float y1 = m_obOffsetPosition.y;

            float x2 = x1 + size.width;
            float y2 = y1 + size.height;
            float x = m_transformToBatch.tx;
            float y = m_transformToBatch.ty;

            float cr = m_transformToBatch.a;
            float sr = m_transformToBatch.b;
            float cr2 = m_transformToBatch.d;
            float sr2 = -m_transformToBatch.c;
            float ax = x1 * cr - y1 * sr2 + x;
            float ay = x1 * sr + y1 * cr2 + y;

            float bx = x2 * cr - y1 * sr2 + x;
            float by = x2 * sr + y1 * cr2 + y;

            float cx = x2 * cr - y2 * sr2 + x;
            float cy = x2 * sr + y2 * cr2 + y;

            float dx = x1 * cr - y2 * sr2 + x;
            float dy = x1 * sr + y2 * cr2 + y;

			if(_byAntiAlias)
			{
				m_sQuad.bl.vertices = vertex3( ceil(ax), ceil(ay), m_fVertexZ );
				m_sQuad.br.vertices = vertex3( ceil(bx), ceil(by), m_fVertexZ );
				m_sQuad.tl.vertices = vertex3( ceil(dx), ceil(dy), m_fVertexZ );
				m_sQuad.tr.vertices = vertex3( ceil(cx), ceil(cy), m_fVertexZ );
			}
			else
			{
				m_sQuad.bl.vertices = vertex3(ax, ay, m_fVertexZ );
				m_sQuad.br.vertices = vertex3(bx, by, m_fVertexZ );
				m_sQuad.tl.vertices = vertex3(dx, dy, m_fVertexZ );
				m_sQuad.tr.vertices = vertex3(cx, cy, m_fVertexZ );
			}
			//if(KEYDOWN('1'))
			//{
			//	CCLOG("【 Pos 】 %s | %f, %f | %f, %f", m_bIgnoreAnchorPointForPosition ? "true" : "false", m_obPosition.x, m_obPosition.y, m_obAnchorPointInPoints.x, m_obAnchorPointInPoints.y);
			//	CCLOG("【 Pos 】 %f, %f | %f, %f | %f, %f", m_fRotationX, m_fRotationY, m_fSkewX, m_fSkewY, m_fScaleX, m_fScaleY);
			//	CCLOG("【 Pos 】 %f, %f | %f, %f | %f, %f", m_transformToBatch.tx, m_transformToBatch.ty, m_transformToBatch.a, m_transformToBatch.b, m_transformToBatch.c, m_transformToBatch.d);
			//	CCLOG("【 Pos 】 bl : %f, %f | %f, %f", m_sQuad.bl.texCoords.u, m_sQuad.bl.texCoords.v, m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y);
			//	CCLOG("【 Pos 】 br : %f, %f | %f, %f", m_sQuad.br.texCoords.u, m_sQuad.br.texCoords.v, m_sQuad.br.vertices.x, m_sQuad.br.vertices.y);
			//	CCLOG("【 Pos 】 tl : %f, %f | %f, %f", m_sQuad.tl.texCoords.u, m_sQuad.tl.texCoords.v, m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y);
			//	CCLOG("【 Pos 】 tr : %f, %f | %f, %f", m_sQuad.tr.texCoords.u, m_sQuad.tr.texCoords.v, m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y);
			//}
        }

        // MARMALADE CHANGE: ADDED CHECK FOR NULL, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
        if (m_pobTextureAtlas)
		{
            m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
		
        m_bRecursiveDirty = false;
        setDirty(false);
    }

    // MARMALADE CHANGED
    // recursively iterate over children
/*    if( m_bHasChildren ) 
    {
        // MARMALADE: CHANGED TO USE CCNode*
        // NOTE THAT WE HAVE ALSO DEFINED virtual CCNode::updateTransform()
        arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);
    }*/
    CCNode::updateTransform();

#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint vertices[4] = {
        ccp( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),
        ccp( m_sQuad.br.vertices.x, m_sQuad.br.vertices.y ),
        ccp( m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y ),
        ccp( m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
	XFUNC_END();
}

bool XSprite::initWithFileThread( const char* fileImage )
{
	XFUNC_START();
	do 
	{
		//加入加载队列
		int nLen = strlen(fileImage);
		if (IsPng(fileImage, nLen) || IsJpg(fileImage, nLen) || IsJpeg(fileImage, nLen))
		{
			unsigned short wResId = XResLoader::Instance().AddResLoad(XResType::ePng, (XEventObj*)this, 
				(FUNC_HANDEL)&XSprite::ResHandle, fileImage, NULL);
			if (wResId == 0xFFFF)
			{
				break;
			}
			XResType* pResType = XResLoader::Instance().GetResByID(wResId);
			if (!pResType)
			{
				break;
			}
			XResPNG* pRes = dynamic_cast<XResPNG*>(pResType);
			if (!pRes)
			{
				break;
			}			
			XTexture2D* &pTex = pRes->m_pTexture;
			if (!pTex)
			{
				break;
			}
			//printf("[ref:%d]%s\n", pTex->retainCount(), fileImage);
		}
		else if(IsXA8(fileImage, nLen))
		{
			unsigned short wResId = XResLoader::Instance().AddResLoad(XResType::eXA8, (XEventObj*)this, 
				(FUNC_HANDEL)&XSprite::ResHandle, fileImage, NULL);
			if (wResId == 0xFFFF)
			{
				break;
			}
		}
		else
		{
			break;
			//// 压缩纹理
			//unsigned short wResId = XResLoader::Instance().AddResLoad(XResType::eTexCompressed, (XEventObj*)this, 
			//	(FUNC_HANDEL)&XSpriteBatchNode::ResHandle, GetRealPath(fileImage), NULL);
			//if (wResId == 0xFFFF)
			//{
			//	break;
			//}
			//XResType* pResType = XResLoader::Instance().GetResByID(wResId);
			//if (!pResType)
			//{
			//	break;
			//}
			//XResCompressed* pRes = dynamic_cast<XResCompressed*>(pResType);
			//if (!pRes)
			//{
			//	break;
			//}			
			//XTexture2D* &pTex = pRes->m_pTexture;
			//if (!pTex)
			//{
			//	break;
			//}
		}

		//static unsigned long dwReq = 0;
		//CCLOG("XSpriteBatchNode::initWithFile [%d] %s", dwReq++, fileImage);

		_bResLoaded = false;
		return true;

	} while (false);
	CCLOG("[ERROR]XSprite::initWithFile %s failed!", fileImage);
	XFUNC_END_RETURN(bool, res, false);
}

void XSprite::ResHandle( long lParam )
{
	XFUNC_START();
	do 
	{
		const char* szPath = NULL;
		XResType* pResType = XResLoader::Instance().GetResByID(lParam);
		if (!pResType)
		{
			break;
		}
		CCTexture2D* pTex, *pTexA = NULL, *pTexP = NULL;
		if (XResType::ePng == pResType->GetType())
		{
			XResPNG* pRes = dynamic_cast<XResPNG*>(pResType);
			if (!pRes)
			{
				break;
			}
			XTexture2D* &pCTex = pRes->m_pTexture;
			if (!pCTex)
			{
				break;
			}
			if (pRes->m_pImage)
			{
				//XEC_TIMER_ST();
				bool bRes = XTextureCache::Instance().InitTexture(pRes->m_szPath, pRes->m_pImage, pCTex);
				if (!bRes)
				{
					break;
				}
				//static unsigned long dwImg = 0;
				//char szImg[MAX_PATH];
				//sprintf(szImg, "%d.png", dwImg++);
				//pRes->m_pImage->saveToFile(szImg, false);
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
				//XEC_OUTRUNSEC();
			}
			pTex = pCTex;
			szPath = pRes->m_szPath;
			if(_byAntiAlias)
				pTex->setAntiAliasTexParameters();
			else
				pTex->setAliasTexParameters();
		}
		else if (XResType::eXA8 == pResType->GetType())
		{
			XResXA8* pRes = dynamic_cast<XResXA8*>(pResType);
			if (!pRes)
			{
				break;
			}
			if(pRes->m_pImage)
			{
				pRes->m_wFrameNum = pRes->m_pImage->stScrDesc.wFrameNum;
				if(m_wXA8Frame >= pRes->m_wFrameNum)
					break;
				unsigned int powW = 0;
				unsigned int powH = 0;
				short nWHalf = (pRes->m_pImage->stScrDesc.wWidth >> 1);
				short nWHalfFrm;
				for(int nPos = 0; nPos < pRes->m_wFrameNum; ++nPos)
				{
					XA8Frame& stFrm = pRes->m_pImage->astFrames[nPos];
					// 偏移修改为左下角(0,0)，以裁剪图底边中心点为热点
					nWHalfFrm = (short)(stFrm.wWidth >> 1);
					pRes->m_aOffset[nPos].nX = (short)stFrm.wLeft + nWHalfFrm - nWHalf;
					pRes->m_aOffset[nPos].nY = (short)pRes->m_pImage->stScrDesc.wHeight - (short)stFrm.wTop - (short)stFrm.wHeight;
					if (CCConfiguration::sharedConfiguration()->supportsNPOT())
					{
						powW = stFrm.wWidth;
						powH = stFrm.wHeight;
					}
					else
					{
						powW = xNextPOT(stFrm.wWidth);
						powH = xNextPOT(stFrm.wHeight);
					}
					//XEC_TIMER_ST();
					pRes->m_pTexture[nPos] = new cocos2d::XTexture2D();
					if(pRes->m_pTexture[nPos])
					{
						pRes->m_pTexture[nPos]->retain();
						if(stFrm.abyData)
						{
							pRes->m_pTexture[nPos]->initWithData(stFrm.abyData, kCCTexture2DPixelFormat_A8, powW, powH, CCSizeMake((float)stFrm.wWidth, (float)stFrm.wHeight));
							pRes->m_pTexture[nPos]->setAliasTexParameters();
						}
					}
					//XEC_OUTRUNSEC();
					pRes->m_pTexAlpha[nPos] = new cocos2d::XTexture2D();
					if(pRes->m_pTexAlpha[nPos])
					{
						pRes->m_pTexAlpha[nPos]->retain();
						if(stFrm.abyDataA)
						{
							pRes->m_pTexAlpha[nPos]->initWithData(stFrm.abyDataA, kCCTexture2DPixelFormat_A8, powW, powH, CCSizeMake((float)stFrm.wWidth, (float)stFrm.wHeight));
							pRes->m_pTexAlpha[nPos]->setAntiAliasTexParameters();
						}
					}
					//XEC_OUTRUNSEC();
				}
				//XEC_TIMER_ST();
				pRes->m_pTexPal = new cocos2d::XTexture2D();
				if(pRes->m_pTexPal)
				{
					pRes->m_pTexPal->retain();
					pRes->m_pTexPal->initWithData(pRes->m_pImage->stScrDesc.abyPal, kCCTexture2DPixelFormat_RGB888, 256, 1, CCSizeMake(256.f, 1.f));
					pRes->m_pTexPal->setAliasTexParameters();
				}
				//XEC_OUTRUNSEC();
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
			}
			m_nOffsetX = pRes->m_aOffset[m_wXA8Frame].nX;
			m_nOffsetY = pRes->m_aOffset[m_wXA8Frame].nY;
			pTex = pRes->m_pTexture[m_wXA8Frame];
			pTexA = pRes->m_pTexAlpha[m_wXA8Frame];
			pTexP = pRes->m_pTexPal;
			szPath = pRes->m_szPath;
		}
		else
		{
			break;
			//// 压缩纹理
			//XResCompressed* pRes = dynamic_cast<XResCompressed*>(pResType);
			//if (!pRes)
			//{
			//	break;
			//}
			//XTexture2D* &pXTex = pRes->m_pTexture;
			//if (!pXTex)
			//{
			//	break;
			//}
			//if (!pRes->m_byTexInited)
			//{
			//	bool bRes = XTextureCache::Instance().InitTexture(pRes->m_szPath, pXTex, pRes->m_pTexAlpha);
			//	if (!bRes)
			//	{
			//		break;
			//	}
			//	pRes->m_byTexInited = 0xFF;
			//}
			//if (pRes->m_pTexAlpha)
			//{
			//	m_pTexKtxAlpha = pRes->m_pTexAlpha;
			//	m_pTexKtxAlpha->retain();
			//	m_pShaderKTX = XShader::GetShader(this, XShader::eETC);
			//}			
			//pTex = pXTex;
			//szPath = pRes->m_szPath;
		}
		if(pTex)
		{
			CCPoint stPt = m_obPosition;
			CCPoint stAPt = m_obAnchorPoint;
			bool bRes = initWithTexture(pTex, pTexA, pTexP);
			if (!bRes)
			{
				break;
			}
			//if(pTexA)
			//	CCLOG("[%s]%d : CRef %d, ARef %d, PRef %d", pResType->m_szPath, m_wXA8Frame, pTex->retainCount(), pTexA->retainCount(), pTexP->retainCount());
			_bResLoaded = true;
			setAnchorPoint(stAPt);
			setPosition(stPt);
		}
		if(_bCleanUpThread)
			cleanup();

		//static unsigned long dwRes = 0;
		//CCLOG("XSpriteBatchNode::ResHandle [%d] %s", dwRes++, szPath);

		return;
	} while (0);
	_bResLoaded = false;
	OnRelease();
	CCLOG("[ERROR]XSprite::ResHandle load res failed.");
	XFUNC_END();
}

XSprite::XSprite(void)
{
	_byScale = 100;
	XMemLog::m_dwNew += sizeof(XSprite);
}

XSprite::~XSprite(void)
{
	XMemLog::m_dwNew -= sizeof(XSprite);
}

void XSprite::setFlipX(bool bFlipX)
{
	XFUNC_START();
	if(!_bResLoaded)
		return;
	if (m_bFlipX != bFlipX)
	{
		m_bFlipX = bFlipX;
		setTextureRect(m_obRect, m_bRectRotated, m_obContentSize);
	}
	XFUNC_END();
}

void XSprite::setFlipY(bool bFlipY)
{
	XFUNC_START();
	if(!_bResLoaded)
		return;
	if (m_bFlipY != bFlipY)
	{
		m_bFlipY = bFlipY;
		setTextureRect(m_obRect, m_bRectRotated, m_obContentSize);
	}
	XFUNC_END();
}

void XSprite::setOpacity(GLubyte opacity)
{
	if(!_bResLoaded)
		return;
	if(_realOpacity == opacity)
		return;
	CCSprite::setOpacity(opacity);;
}

void XSprite::setColor(const ccColor3B& color3)
{
	if(!_bResLoaded)
		return;
	if(_realColor.r == color3.r && _realColor.g == color3.g && _realColor.b == color3.b)
		return;
	CCSprite::setColor(color3);
}

void XSprite::setPosition( const CCPoint& pos )
{
	XFUNC_START();
	if(!_bResLoaded)
		return;
	CCPoint stPosOffset = pos;
	stPosOffset.x += (short)(m_nOffsetX * m_fScaleX);
	stPosOffset.y += (short)(m_nOffsetY * m_fScaleY);
	if (m_obPosition.x == stPosOffset.x && m_obPosition.y == stPosOffset.y)
		return;
	CCSprite::setPosition(stPosOffset);
	XFUNC_END();
}

void XSprite::draw()
{
	XFUNC_START();
	if(!_bResLoaded)
		return;
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "XSprite - draw");
	CCAssert(!m_pobBatchNode, "If XSprite is being rendered by CCSpriteBatchNode, XSprite#draw SHOULD NOT be called");
	do{
		ccGLEnable(m_eGLServerState);
		if(m_pTexAlpha && m_pTexPal)
		{
			static XShaderPal stShaderPal;
			stShaderPal.UseShader();
			stShaderPal.RunBeforeDraw();
		}
		else
		{
			CCAssert(getShaderProgram(), "No shader program set for this node");
			{
				getShaderProgram()->use();
				getShaderProgram()->setUniformsForBuiltins();
			}
			if(m_pShaderOnce)
			{
				m_pShaderOnce->use();
				m_pShaderOnce->setUniformsForBuiltins();
				if(m_pShaderOnce->m_pShader)
					m_pShaderOnce->m_pShader->RunBeforeDraw();
				m_pShaderOnce = NULL;
			}
			//else if(m_pobTexture)
			//{
			//	static XShaderBloom stShaderBloom;
			//	stShaderBloom.UseShader();
			//	stShaderBloom.SetTexSize(SCRPIX_W, SCRPIX_H);
			//}
		}
	} while(0);
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	if (m_pobTexture)
		ccGLBindTexture2DN(eTexColor, m_pobTexture->getName());
	else
		ccGLBindTexture2DN(eTexColor, 0);
	if(m_pTexAlpha)
		ccGLBindTexture2DN(eTexAlpha, m_pTexAlpha->getName());
	else
		ccGLBindTexture2DN(eTexAlpha, 0);
	if(m_pTexPal)
		ccGLBindTexture2DN(eTexPal, m_pTexPal->getName());
	else
		ccGLBindTexture2DN(eTexPal, 0);
    //
    // Attributes
    //
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_GL_ERROR_DEBUG();
	XMemLog::m_dwUpdate += (sizeof(ccV3F_C4B_T2F) << 2);
#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    CCPoint vertices[4]={
        ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
        ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
        ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
        ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    CCSize s = this->getTextureRect().size;
    CCPoint offsetPix = this->getOffsetPosition();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    CC_INCREMENT_GL_DRAWS(1);
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "XSprite - draw");
	XFUNC_END();
}

unsigned char XScrSprite::m_byShaderType = XScrSprite::eShaderNone;
unsigned char XScrSprite::m_byShaderParam = 0x00;
unsigned char XScrSprite::m_bySimpleMode = 0xFF;
float XScrSprite::m_fScrScale = 1.f;
unsigned char XScrSprite::m_byScrSprNum = 0x00;
unsigned char XScrSprite::m_byReloadID = 0x00;
float fScrScale = 1.f;

XScrSprite::XScrSprite(void)
{
	m_stScrTex.InitWithBackBuffer();
	m_stScrTex.setAntiAliasTexParameters();
	CCSprite::initWithTexture(&m_stScrTex, CCRectMake(0, 0, SCRPIX_W, SCRPIX_H));
	_bResLoaded = true;
	setAnchorPoint(ccp(0.5f, 0.5f));
	setPosition(ccp(SCR_W>>1, SCR_H>>1));
	fScrScale = 1.f / SCR_SCALE;
	setScaleX(fScrScale);
	setScaleY(-fScrScale);
	m_byRTTInited = 0x00;
	m_byID = ++m_byScrSprNum;
}

XScrSprite::~XScrSprite(void)
{
	--m_byScrSprNum;
}

void XScrSprite::BeginRTT()
{
	XFUNC_START();
	if(XConfig::m_byScrEffect)
		return;
	if(m_byReloadID && m_byReloadID == m_byID)
	{
		if(m_byRTTInited)
		{
			CCSprite::cleanup();
			OnRelease();
			m_stScrTex.InitWithBackBuffer();
			m_stScrTex.setAntiAliasTexParameters();
			CCSprite::initWithTexture(&m_stScrTex, CCRectMake(0, 0, SCRPIX_W, SCRPIX_H));
			_bResLoaded = true;
			setAnchorPoint(ccp(0.5f, 0.5f));
			setPosition(ccp(SCR_W>>1, SCR_H>>1));
			fScrScale = 1.f / SCR_SCALE;
			setScaleX(fScrScale);
			setScaleY(-fScrScale);
			m_byRTTInited = 0x00;
			m_stRTT.reset();
			m_byRTTInited = 0x00;
		}
		--m_byReloadID;
	}
	if(!m_byRTTInited)
	{
		m_byRTTInited = 0xFF;
		unsigned short wW = SCR_W;//xNextPOT(SCR_W);
		unsigned short wH = SCR_H;//xNextPOT(SCR_H);
		//if(wW > wH)
		//	wH = wW;
		//else
		//	wW = wH;
		bool bRes = m_stRTT.initWithWidthAndHeight(wW, wH, kCCTexture2DPixelFormat_RGBA8888);
		if(bRes)
		{
			cocos2d::CCTexture2D* pTex = m_stRTT.GetTexture();
			if(pTex)
				pTex->setAntiAliasTexParameters();
			m_stRTT.setAnchorPoint(ccp(0.5f, 0.5f));
			m_stRTT.setPosition(ccp(SCR_W>>1, SCR_H>>1));
			bRes = CCSprite::initWithTexture(m_stRTT.GetTexture());
			if(!bRes)
				m_byRTTInited = 0xFE;
		}
		else
			m_byRTTInited = 0xFE;
	}
	if(0xFE == m_byRTTInited)
		return;
	//m_stRTT.beginWithClear(0.f, 0.f, 0.f, 0.f);
	m_stRTT.begin();
	XFUNC_END();
}

void XScrSprite::GetScreen()
{
	XFUNC_START();
	if(XConfig::m_byScrEffect)
	{
		if(fIsZero(m_fScrScale - 1.f) && eShaderNone == m_byShaderType)
			return;
		m_stScrTex.GetBackBuffer();
		return;
	}
	if(0xFE == m_byRTTInited)
		return;
	m_stRTT.end();
	XFUNC_END();
}

void XScrSprite::OnDraw()
{
	XFUNC_START();
	if(XConfig::m_byScrEffect)
	{
		if(fIsZero(m_fScrScale - 1.f) && eShaderNone == m_byShaderType)
			return;
		float fScale = m_fScrScale * fScrScale;
		setScaleX(fScale);
		setScaleY(-fScale);
	}
	else
	{
		if(0xFE == m_byRTTInited)
			return;
		setScaleX(m_fScrScale);
		setScaleY(-m_fScrScale);
	}
	visit();
	XFUNC_END();
}

void XScrSprite::draw()
{
	XFUNC_START();
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "XScrSprite - draw");
	CCAssert(!m_pobBatchNode, "If XScrSprite is being rendered by CCSpriteBatchNode, XScrSprite#draw SHOULD NOT be called");
	do{
		ccGLEnable(m_eGLServerState);
		if (m_pobTexture)
		{
			if(eShaderBloom == m_byShaderType)
			{
				static XShaderBloom stShaderBloom;
				stShaderBloom.UseShader();
				if(m_byShaderParam > 100)
					m_byShaderParam = 100;
				float fParam = 0.5f + 0.25f * (float)m_byShaderParam * 0.01f;
				stShaderBloom.SetTexSize(SCRPIX_W, SCRPIX_H, fParam);
			}
			else if(eShaderHeat == m_byShaderType)
			{
				static XShaderHeat stShaderHeat;
				stShaderHeat.UseShader();
				stShaderHeat.RunBeforeDraw();
			}
			else
			{
				CCAssert(getShaderProgram(), "No shader program set for this node");
				{
					getShaderProgram()->use();
					getShaderProgram()->setUniformsForBuiltins();
				}
				if(m_pShaderOnce)
				{
					m_pShaderOnce->use();
					m_pShaderOnce->setUniformsForBuiltins();
					if(m_pShaderOnce->m_pShader)
						m_pShaderOnce->m_pShader->RunBeforeDraw();
					m_pShaderOnce = NULL;
				}
			}
		}
	} while(0);
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	if (m_pobTexture != NULL)
		ccGLBindTexture2D( m_pobTexture->getName() );
	else
		ccGLBindTexture2D(0);
	ccGLBindTexture2DN(1, m_pobTexture->getName());
	//
	// Attributes
	//
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;
	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CHECK_GL_ERROR_DEBUG();
	XMemLog::m_dwUpdate += (sizeof(ccV3F_C4B_T2F) << 2);
#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4]={
		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
	CC_INCREMENT_GL_DRAWS(1);
	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "XScrSprite - draw");
	XFUNC_END();
}

NS_CC_END
