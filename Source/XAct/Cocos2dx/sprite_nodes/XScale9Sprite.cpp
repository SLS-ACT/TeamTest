#include "../../Common.h"
#include "XScale9Sprite.h"

XCPP_DEFINED();

NS_CC_BEGIN

XScale9Sprite::XScale9Sprite( void )
{
	XMemLog::m_dwNew += sizeof(XScale9Sprite);
}

XScale9Sprite::~XScale9Sprite( void )
{
	XMemLog::m_dwNew -= sizeof(XScale9Sprite);
}

XScale9Sprite* XScale9Sprite::createWithSpriteFrame( CCSpriteFrame* pSpriteFrame, const CCRect& capInsets )
{
	XFUNC_START();
	if (pSpriteFrame)
	{
		if (fIsZero(capInsets.size.width) || fIsZero(capInsets.size.height))
		{
			return NULL;
		}
		XScale9Sprite* pobSprite = XMP_CREATE(XScale9Sprite);
		while(pobSprite)
		{
			CCTexture2D* pTexture = pSpriteFrame->getTexture();
			if (!pTexture)
			{
				break;
			}
			pobSprite->m_pDrawSprite = XSpriteBatchNode::createWithTexture(pTexture);

			if(!pobSprite->m_pDrawSprite)
				break;
			pobSprite->m_pDrawSprite->setScale(0.5f);
			pobSprite->m_pDrawSprite->setScale(1.f);
			pobSprite->m_pDrawSprite->setPosition(1.f, 1.f);
			pobSprite->m_pDrawSprite->setPosition(0.f, 0.f);
			pobSprite->m_pDrawSprite->setAnchorPoint(ccp(1.f, 1.f));
			pobSprite->m_pDrawSprite->setAnchorPoint(ccp(0.f, 0.f));
			CCRect stRcTex = pSpriteFrame->getRect();

			CCRect stRcChild[eNum];
			stRcChild[eUp_Left		] = CCRectMake(stRcTex.origin.x, stRcTex.origin.y, capInsets.origin.x, capInsets.origin.y);
			stRcChild[eUp_Center	] = CCRectMake(stRcTex.origin.x + capInsets.origin.x, stRcTex.origin.y, capInsets.size.width, capInsets.origin.y);
			stRcChild[eUp_Right		] = CCRectMake(stRcTex.origin.x + capInsets.origin.x + capInsets.size.width, stRcTex.origin.y, stRcTex.size.width - capInsets.origin.x - capInsets.size.width, capInsets.origin.y);

			stRcChild[eMid_Left		] = CCRectMake(stRcTex.origin.x, stRcTex.origin.y + capInsets.origin.y, capInsets.origin.x, capInsets.size.height);
			stRcChild[eMid_Center	] = CCRectMake(stRcTex.origin.x + capInsets.origin.x, stRcTex.origin.y + capInsets.origin.y, capInsets.size.width, capInsets.size.height);
			stRcChild[eMid_Right	] = CCRectMake(stRcTex.origin.x + capInsets.origin.x + capInsets.size.width, stRcTex.origin.y + capInsets.origin.y, stRcTex.size.width - capInsets.origin.x - capInsets.size.width, capInsets.size.height);

			stRcChild[eDown_Left	] = CCRectMake(stRcTex.origin.x, stRcTex.origin.y + capInsets.origin.y + capInsets.size.height, capInsets.origin.x, stRcTex.size.height - capInsets.origin.y - capInsets.size.height);
			stRcChild[eDown_Center	] = CCRectMake(stRcTex.origin.x + capInsets.origin.x, stRcTex.origin.y + capInsets.origin.y + capInsets.size.height, capInsets.size.width, stRcTex.size.height - capInsets.origin.y - capInsets.size.height);
			stRcChild[eDown_Right	] = CCRectMake(stRcTex.origin.x + capInsets.origin.x + capInsets.size.width, stRcTex.origin.y+ capInsets.origin.y + capInsets.size.height, stRcTex.size.width - capInsets.size.width - capInsets.origin.x, stRcTex.size.height - capInsets.origin.y - capInsets.size.height);

			for (int nPos = 0; nPos < eNum; ++nPos)
			{
				XSprite* pSpr = XSprite::createWithTexture(pTexture, stRcChild[nPos], 0x00);
				if(pSpr)
				{
					pSpr->setAnchorPoint(ccp(0.5f,0.5f));
					pSpr->setAnchorPoint(ccp(0.f,0.f));
					pSpr->setOpacity(0x00);
					pSpr->setOpacity(0xFF);
					pobSprite->m_pDrawSprite->addChild(pSpr, pSpr->getZOrder(), nPos);
				}
			}
			pobSprite->m_stRcCenter = capInsets;
			pobSprite->m_stRcTexture = stRcTex;
			pTexture->setAntiAliasTexParameters();
			//pTexture->setAliasTexParameters();
			return pobSprite;
		}
		XMP_RELEASE(XScale9Sprite, pobSprite);
	}
	XFUNC_END_RETURN(XScale9Sprite*, ptr, NULL);	
}

XScale9Sprite* XScale9Sprite::createWithSpriteFrameName( const char* pszSpriteFrameName, const CCRect& capInsets )
{
	XFUNC_START();
	CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
	if(!pFrame)
	{
		CCLOG("%s is not exit!!!!", pszSpriteFrameName);
	}
#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName:%s", pszSpriteFrameName);
	CCAssert(pFrame != NULL, msg);
#else
	if(!pFrame)
		return NULL;
#endif
	return createWithSpriteFrame(pFrame, capInsets);
	XFUNC_END_RETURN(XScale9Sprite*, ptr, NULL);
}

void XScale9Sprite::setSize( float fW, float fH )
{
	XFUNC_START();
	m_fHeight = fH;
	m_fWidth = fW;
	m_bIsdirty = true;
	XFUNC_END();
}

void XScale9Sprite::setPosition( const CCPoint& pos )
{
	XFUNC_START();
	if (m_obPosition.x == pos.x && m_obPosition.y == pos.y)
	{
		return;
	}
	CCSprite::setPosition(pos);
	if (m_pDrawSprite)
	{
		m_pDrawSprite->setPosition(pos);
	}
	XFUNC_END();
}
void XScale9Sprite::setOpacity(unsigned char byApha)
{
	XFUNC_START();
	if (m_pDrawSprite)
	{
		m_pDrawSprite->setOpacity(byApha);
	}
	XFUNC_END();
}
void XScale9Sprite::setScale(float fScale)
{
	XFUNC_START();
	if (m_pDrawSprite)
	{
		m_pDrawSprite->setScale(fScale);
	}
	XFUNC_END();
}
void XScale9Sprite::visit()
{
	XFUNC_START();
	if (m_pDrawSprite)
	{
		if (m_bIsdirty)
		{
			m_bIsdirty = false;
			// 初始化精灵
			CCNode* apNode[eNum];
			for (int nPos = 0; nPos < eNum; ++nPos)
			{
				apNode[nPos] = m_pDrawSprite->getChildByTag(nPos);
				if (!apNode[nPos])
				{
					m_bIsdirty = true;
					break;
				}
			}
			if (!m_bIsdirty)
			{
				float fPosX, fPosY, fScaleX, fScaleY;
				CCNode* pNodeThis;
				fScaleX = (m_fWidth - (m_stRcTexture.size.width - m_stRcCenter.size.width)) / m_stRcCenter.size.width;
				fScaleY = (m_fHeight - (m_stRcTexture.size.height - m_stRcCenter.size.height)) / m_stRcCenter.size.height;
				// 精灵位置及缩放
				pNodeThis = apNode[eUp_Left];
				fPosX = 0.f;
				fPosY = m_fHeight - m_stRcCenter.origin.y;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScale(1.f);

				pNodeThis = apNode[eUp_Center];
				fPosX = m_stRcCenter.origin.x;
				fPosY = m_fHeight - m_stRcCenter.origin.y;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScaleX(fScaleX);
				pNodeThis->setScaleY(1.f);

				pNodeThis = apNode[eUp_Right];
				fPosX = m_fWidth - (m_stRcTexture.size.width - m_stRcCenter.origin.x - m_stRcCenter.size.width);
				fPosY = m_fHeight - m_stRcCenter.origin.y;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScale(1.f);

				pNodeThis = apNode[eMid_Left];
				fPosX = 0.f;
				fPosY = m_stRcTexture.size.height - m_stRcCenter.origin.y - m_stRcCenter.size.height;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScaleX(1.f);
				pNodeThis->setScaleY(fScaleY);

				pNodeThis = apNode[eMid_Center];
				fPosX = m_stRcCenter.origin.x;
				fPosY = m_stRcTexture.size.height - m_stRcCenter.origin.y - m_stRcCenter.size.height;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScaleX(fScaleX);
				pNodeThis->setScaleY(fScaleY);

				pNodeThis = apNode[eMid_Right];
				fPosX = m_fWidth - (m_stRcTexture.size.width - m_stRcCenter.origin.x - m_stRcCenter.size.width);
				fPosY = m_stRcTexture.size.height - m_stRcCenter.origin.y - m_stRcCenter.size.height;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScaleX(1.f);
				pNodeThis->setScaleY(fScaleY);

				pNodeThis = apNode[eDown_Left];
				fPosX = 0.f;
				fPosY = 0.f;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScale(1.f);

				pNodeThis = apNode[eDown_Center];
				fPosX = m_stRcCenter.origin.x;
				fPosY = 0.f;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScaleX(fScaleX);
				pNodeThis->setScaleY(1.f);

				pNodeThis = apNode[eDown_Right];
				fPosX = m_fWidth - (m_stRcTexture.size.width - m_stRcCenter.origin.x - m_stRcCenter.size.width);
				fPosY = 0.f;
				pNodeThis->setPosition(fPosX, fPosY);
				pNodeThis->setScale(1.f);

				//m_pDrawSprite->setScale(1.f);
			}
		}
		m_pDrawSprite->visit();
	}
	XFUNC_END();
}

void XScale9Sprite::OnException( const char* szFile, const int nLine, const char* szInfo )
{
	CCLOG("[XSprite::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
}

bool XScale9Sprite::OnCreate()
{
	XFUNC_START();
	m_pDrawSprite	= NULL;
	m_bIsdirty		= false;
	m_fWidth		= 0.f;
	m_fHeight		= 0.f;
	m_stRcCenter	= CCRectZero;
	m_stRcTexture	= CCRectZero;
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
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XScale9Sprite::OnRelease()
{
	XFUNC_START();
	CC_SAFE_RELEASE_NULL(m_pDrawSprite);
	XFUNC_END();
}

void XScale9Sprite::cleanup()
{
	XFUNC_START();
	if (m_pDrawSprite)
	{
		m_pDrawSprite->cleanup();
		m_pDrawSprite = NULL;
	}
	CCSprite::cleanup();
	if (XMemPool<XScale9Sprite>::m_pXMPParent)
	{
		XScale9Sprite* pSprite = this;
		try
		{
			XMP_RELEASE(XScale9Sprite, pSprite);
		}
		catch (...)
		{
			
		}
	}
	XFUNC_END();
}

NS_CC_END