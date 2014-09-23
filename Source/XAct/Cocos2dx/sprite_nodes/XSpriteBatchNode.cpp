#include "../../Common.h"
#include "../XLoader/XLoader.h"
#include "../textures/XTextureCache.h"
//#include "../../XConfig.h"

XCPP_DEFINED();

NS_CC_BEGIN

void XSpriteBatchNode::OnException(const char* szFile, const int nLine, const char* szInfo) {
	CCLOG("[XSpriteBatchNode::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
}

bool XSpriteBatchNode::OnCreate()
{
	XFUNC_START();
	_realOpacity = 0xFF;
	_realColor = cocos2d::ccc3(0xFF, 0xFF, 0xFF);
	_bResLoaded = true;
	_bCleanUpThread = false;
	m_pShaderOnce = NULL;
	_byScale = 0x00;
	_fChildrenScaleY = 1.f;
	_byAntiAlias = 0x00;
	_byChangeColor = 0x00;
	setPosition(1.f,1.f);
	setPosition(0.f,0.f);
	setAnchorPoint(ccp(0.f,0.f));
	setAnchorPoint(ccp(0.5f,0.5f));
	setScale(0.5f);
	setScale(1.f);
	setRotation(1.f);
	setRotation(0.f);
	setOpacity(0x00);
	setOpacity(0xFF);
	retain();
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XSpriteBatchNode::OnRelease()
{
	XFUNC_START();
//#	if defined(X_DUMPFUNC) || defined(ANDROID)
//	XSMLOG("[XSpriteBatchNode::OnRelease] mem[%d] this : %p", XMemLog::m_dwMemPhy, this);
//#	endif
	if (m_pShaderKTX)
	{
//#		if defined(X_DUMPFUNC) || defined(ANDROID)
//		XSMLOG("[XSpriteBatchNode::OnRelease] m_pShaderKTX->Release() : %p", m_pShaderKTX);
//#		endif
		m_pShaderKTX->Release();
		m_pShaderKTX = NULL;
	}
	if (m_pTexKtxAlpha)
	{
//#		if defined(X_DUMPFUNC) || defined(ANDROID)
//		XSMLOG("[XSpriteBatchNode::OnRelease] m_pTexKtxAlpha->Release() : %p", m_pTexKtxAlpha);
//#		endif
		m_pTexKtxAlpha->release();
		m_pTexKtxAlpha = NULL;
	}
	//XSHADER_RELEASE();
	if(m_pobTextureAtlas)
	{
//#		if defined(X_DUMPFUNC) || defined(ANDROID)
//		XSMLOG("[XSpriteBatchNode::OnRelease] CC_SAFE_RELEASE_NULL(m_pobTextureAtlas) : %p", m_pobTextureAtlas);
//#		endif
        m_pobTextureAtlas->removeAllQuads();
		CC_SAFE_RELEASE_NULL(m_pobTextureAtlas);
	}
	m_pobTextureAtlas = NULL;
	if(m_pobDescendants)
	{
//#		if defined(X_DUMPFUNC) || defined(ANDROID)
//		XSMLOG("[XSpriteBatchNode::OnRelease] CC_SAFE_RELEASE_NULL(m_pobDescendants) : %p", m_pobDescendants);
        //#		endif
        m_pobDescendants->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_pobDescendants);
	}
	m_pobDescendants = NULL;
	if(m_pChildren)
	{
		delete m_pChildren;
	}
	m_pChildren = NULL;
	if(m_pobDescendants)
	{
		delete m_pobDescendants;
	}
	m_pobDescendants = NULL;
	XFUNC_END();
}

void XSpriteBatchNode::setOpacity(GLubyte opacity)
{
	XFUNC_START();
	if (opacity != _realOpacity)
	{
		_realOpacity = opacity;
		if(m_pChildren)
		{
			if (m_pChildren->count() > 0)
			{
				int length = m_pChildren->data->num;
				CCNode** x = (CCNode**)m_pChildren->data->arr;

				for(int i = 0; i < length; ++i)
				{
					CCSprite* pChild = (CCSprite*)x[i];
					pChild->setOpacity(opacity);
				}
			}
		}
		
	}
	XFUNC_END();
}

void XSpriteBatchNode::cleanup()
{
	XFUNC_START();
	if(!_bResLoaded)
	{
		_bCleanUpThread = true;
		return;
	}
	_bResLoaded = false;
	CCSpriteBatchNode::cleanup();
	removeAllChildrenWithCleanup(false);
	if(XMemPool<XSpriteBatchNode>::m_pXMPParent)
	{
		XSpriteBatchNode* pSpriteBN = this;
		try
		{
			XMP_RELEASE(XSpriteBatchNode, pSpriteBN);
		}
		catch (...)
		{
		}
	}
	XFUNC_END();
}

/*
* creation with CCTexture2D
*/

XSpriteBatchNode* XSpriteBatchNode::createWithTexture(CCTexture2D* tex, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
	XFUNC_START();
	XSpriteBatchNode *batchNode = XMP_CREATE(XSpriteBatchNode);
	if (batchNode && batchNode->initWithTexture(tex, capacity))
	{
		batchNode->_bResLoaded = true;
		return batchNode;
	}
	XMP_RELEASE(XSpriteBatchNode, batchNode);
	XFUNC_END_RETURN(XSpriteBatchNode*, ptr, NULL);
}

/*
* creation with File Image
*/

XSpriteBatchNode* XSpriteBatchNode::create(const char *fileImage, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
	XFUNC_START();
	XSpriteBatchNode *batchNode = XMP_CREATE(XSpriteBatchNode);
	if (batchNode && batchNode->initWithFile(fileImage, capacity))
	{
		return batchNode;
	}
	XMP_RELEASE(XSpriteBatchNode, batchNode);
	XFUNC_END_RETURN(XSpriteBatchNode*, ptr, NULL);
}

void XSpriteBatchNode::visit(void)
{
	XFUNC_START();
	if(_bResLoaded)
		CCSpriteBatchNode::visit();
	//else
	//{
	//	CCLOG("XSpriteBatchNode::visit not loaded!");
	//}
	XFUNC_END();
}

void XSpriteBatchNode::draw()
{
	XFUNC_START();
	CC_PROFILER_START("XSpriteBatchNode - draw");

	if (m_pobTextureAtlas->getTotalQuads() == 0)
	{
		return;
	}
	do 
	{
		ccGLEnable(m_eGLServerState);
		if (m_pShaderKTX)
		{
			CCGLProgram* pProg = m_pShaderKTX->GetProgram();
			if (pProg)
			{
				pProg->use();
				pProg->setUniformsForBuiltins();
			}
		}
		else
		{
			if (getShaderProgram())
			{
				getShaderProgram()->use();
				getShaderProgram()->setUniformsForBuiltins();
			}			
		}
		if(m_pShaderOnce)
		{
			m_pShaderOnce->use();
			m_pShaderOnce->setUniformsForBuiltins();
			if(m_pShaderOnce->m_pShader)
				m_pShaderOnce->m_pShader->RunBeforeDraw();
			m_pShaderOnce = NULL;
		}
		//else if(m_pobTextureAtlas->getTexture())
		//{
		//	static XShaderBloom stShaderBloom;
		//	stShaderBloom.UseShader();
		//	stShaderBloom.SetTexSize(SCRPIX_W, SCRPIX_H);
		//}
	} while (0);
	arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);
	ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
	if (m_pTexKtxAlpha)
	{
		ccGLBindTexture2DN(1, m_pTexKtxAlpha->getName());
		if (m_pShaderKTX)
		{
			m_pShaderKTX->RunBeforeDraw();
		}	
	}
	m_pobTextureAtlas->drawQuads();
	CC_PROFILER_STOP("XSpriteBatchNode - draw");
	
	XFUNC_END();
}

XSpriteBatchNode::XSpriteBatchNode()
{
	m_pShaderKTX = NULL;
	m_pTexKtxAlpha = NULL;
	OnCreate();
	XMemLog::m_dwNew += sizeof(XSpriteBatchNode);
}

XSpriteBatchNode::~XSpriteBatchNode()
{
	XMemLog::m_dwNew -= sizeof(XSpriteBatchNode);
	m_byIsCreate = 0;
	//CC_SAFE_RELEASE(m_pobTextureAtlas);
	//CC_SAFE_RELEASE(m_pobDescendants);
	m_pobTextureAtlas = NULL;
	m_pobDescendants = NULL;
}

void XSpriteBatchNode::setAntiAliasTexParameters()
{
	XFUNC_START();
	if (m_pobTextureAtlas)
	{
		CCTexture2D* tex = m_pobTextureAtlas->getTexture();
		if (tex)
		{
			tex->setAntiAliasTexParameters();
		}
	}
	
	XFUNC_END();
}

void XSpriteBatchNode::setColor( const ccColor3B& color3 )
{
	XFUNC_START();
	if(!_bResLoaded)
		return;
	if (color3.r != _realColor.r ||
		color3.g != _realColor.b ||
		color3.b != _realColor.b ||
		_byChangeColor)
	{
		_realColor = color3;
		if (m_pChildren->count() > 0)
		{
			int length = m_pChildren->data->num;
			CCNode** x = (CCNode**)m_pChildren->data->arr;
			//insert sort 
			for (int i = 0; i < length; ++i)
			{
				CCSprite* pChild = (CCSprite*)x[i];
				pChild->setColor(color3);
			}			
		}		
	}
	
	XFUNC_END();
}

bool XSpriteBatchNode::initWithFile( const char* fileImage, unsigned int capacity )
{
	XFUNC_START();
	bool bRes;
	do 
	{
		//加入加载队列
		int nLen = strlen(fileImage);
		if (IsPng(fileImage, nLen) || IsJpg(fileImage, nLen) || IsJpeg(fileImage, nLen))
		{
			unsigned short wResId = XResLoader::Instance().AddResLoad(XResType::ePng, (XEventObj*)this, 
				(FUNC_HANDEL)&XSpriteBatchNode::ResHandle, fileImage, NULL);
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
			bRes = initWithTexture(pTex, capacity);
			if (!bRes)
			{
				break;
			}
			//printf("[ref:%d]%s\n", pTex->retainCount(), fileImage);
		}
		else
		{
			break;
			//// 压缩纹理
			//unsigned short wResId = XResLoader::Instance().AddResLoad(XResType::eTexCompressed, (XEventObj*)this, 
			//	(FUNC_HANDEL)&XSpriteBatchNode::ResHandle, fileImage, NULL);
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
			//bRes = initWithTexture(pTex, capacity);
			//if (!bRes)
			//{
			//	break;
			//}
		}

		//static unsigned long dwReq = 0;
		//CCLOG("XSpriteBatchNode::initWithFile [%d] %s", dwReq++, fileImage);

		_bResLoaded = false;
		return true;
		
	} while (false);
	CCLOG("[ERROR]XSpriteBatchNode::initWithFile %s failed!", fileImage);
	XFUNC_END_RETURN(bool, res, false);
}

void XSpriteBatchNode::ResHandle( long lParam )
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
		CCTexture2D* pTex;
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
				bool bRes = XTextureCache::Instance().InitTexture(pRes->m_szPath, pRes->m_pImage, pCTex);
				if (!bRes)
				{
					break;
				}
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
			}
			pTex = pCTex;
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
		if(_byAntiAlias)
			pTex->setAntiAliasTexParameters();
		else
			pTex->setAliasTexParameters();
		// 创建精灵
		XSBNChildInfo* pHead = _stChildren._pHead;
		XSBNChildInfo* pUsed;
		int nPos = 0;
		while(pHead)
		{
			pUsed = pHead;
			pHead = pHead->_pNext;
			XSprite* pSprite = XSprite::createWithTexture(pTex, pUsed->stRect, _byAntiAlias);
			if (!pSprite)
			{
				CCLOG("XSprite::createWithTexture failed!");
				continue;
			}
			if (pUsed->byFlipX)
			{
				pSprite->setFlipX(true);
			}
			pSprite->setAnchorPoint(ccp(0.f, 0.f));
			pSprite->setPosition(ccp(pUsed->fPosX, pUsed->fPosY));
			pSprite->setScaleY(_fChildrenScaleY);
			CCSpriteBatchNode::addChild(pSprite, pSprite->getZOrder(), nPos++);
			XMP_RELEASE(XSBNChildInfo, pUsed);
		}
		_stChildren.Clear();
		_bResLoaded = true;
		if(_bCleanUpThread)
			cleanup();

		//static unsigned long dwRes = 0;
		//CCLOG("XSpriteBatchNode::ResHandle [%d] %s", dwRes++, szPath);

		return;
	} while (0);
	_bResLoaded = false;
	OnRelease();
	CCLOG("[ERROR]XSpriteBatchNode::ResHandle load res failed.");
	XFUNC_END();
}

void XSpriteBatchNode::setPosition( const CCPoint& pos )
{
	XFUNC_START();
	if (m_obPosition.x == pos.x && m_obPosition.y == pos.y)
	{
		return;
	}

	CCSpriteBatchNode::setPosition(pos);
	XFUNC_END();
}

void XSpriteBatchNode::setPosition( float fX, float fY )
{
	XFUNC_START();
	if (m_obPosition.x == fX && m_obPosition.y == fY)
	{
		return;
	}

	CCSpriteBatchNode::setPosition(fX, fY);
	XFUNC_END();
}

NS_CC_END
