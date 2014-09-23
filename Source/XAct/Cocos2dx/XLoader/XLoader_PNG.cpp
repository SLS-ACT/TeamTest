#include "XLoader.h"
#include "../textures/XTexture2D.h"

XCPP_DEFINED();

XResType* XResType::InitRes_Png(const char* szPath, ...)
{
	XFUNC_START();
	do 
	{
		XResPNG* pRes = XMP_CREATE(XResPNG);
		if (!pRes)
		{
			CCLOG("XMP_CREATE(XResPNG) failed!");
			break;
		}
		ResetRes_Png(pRes);
		//printf("[new ref:%d]%s\n", pRes->m_pTexture->retainCount(), szPath);
		//CCLOG("InitRes_Png [%s]", szPath);
		return pRes;
	} while (false);
	XFUNC_END_RETURN(XResType*, ptr, NULL);
}

void XResType::ResetRes_Png(XResType* pResInfo)
{
	XFUNC_START();
	do 
	{
		XResPNG* pRes = dynamic_cast<XResPNG*>(pResInfo);
		if (!pRes)
		{
			break;
		}
		pRes->m_pTexture = new cocos2d::XTexture2D();
		if(pRes->m_pTexture)
			pRes->m_pTexture->retain();
	} while (false);
	XFUNC_END();
}

extern unsigned char g_byRGBA4444;
class XImage :
	public cocos2d::CCImage
{
public:
	XImage() {}
	virtual ~XImage() {}
public:
	inline bool InitWithJpeg(cocos2d::CCImage& stJpg, cocos2d::CCImage& stAlpha) {
		XFUNC_START();
		if(g_byRGBA4444)
		{
			// only RGBA4444 supported
			m_nBitsPerComponent	= 4;
			m_nWidth	= stJpg.getWidth();
			m_nHeight	= stJpg.getHeight();
			m_bHasAlpha	= true;
			m_bPreMulti	= true;
			setPixFormat((unsigned char)cocos2d::kCCTexture2DPixelFormat_RGBA4444);
			int nBytesPerComponent = 2;
			int nSize = m_nWidth * m_nHeight * nBytesPerComponent;
			m_pData = new unsigned char[nSize];
			if(!m_pData)
				return false;
			unsigned char* pRGB = stJpg.getData();
			if(!pRGB)
				return false;
			unsigned char* pAlpha = stAlpha.getData();
			unsigned short* pWrite = (unsigned short*)m_pData;
			// Write
			unsigned char byR, byG, byB;
			unsigned short wColor = 0;
			for(unsigned short wPosR = 0; wPosR < m_nHeight; ++wPosR)
			{
				for(unsigned short wPosC = 0; wPosC < m_nWidth; ++wPosC)
				{
					wColor = 0;
					if(pAlpha)
					{
						wColor += *pAlpha++;
						wColor += *pAlpha++;
						wColor += *pAlpha++;
						wColor /= 3;
					}
					else
						wColor = 0xFF;
#					if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
					if(pAlpha)
						pAlpha++;
#					endif
					byR = *pRGB++;
					byG = *pRGB++;
					byB = *pRGB++;
#					if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
					pRGB++;
#					endif
					byR >>= 4;
					byG >>= 4;
					byB >>= 4;
					wColor >>= 4;
					wColor |= (((unsigned short)byR) << 12);
					wColor |= (((unsigned short)byG) << 8);
					wColor |= (((unsigned short)byB) << 4);
					*pWrite++ = wColor;
				}
			}
		}
		else
		{
			// only RGBA8888 supported
			m_nBitsPerComponent	= 8;
			m_nWidth	= stJpg.getWidth();
			m_nHeight	= stJpg.getHeight();
			m_bHasAlpha	= true;
			m_bPreMulti	= true;
			int nBytesPerComponent = 4;
			int nSize = m_nWidth * m_nHeight * nBytesPerComponent;
			m_pData = new unsigned char[nSize];
			if(!m_pData)
				return false;
			unsigned char* pRGB = stJpg.getData();
			if(!pRGB)
				return false;
			unsigned char* pAlpha = stAlpha.getData();
			unsigned char* pWrite = m_pData;
			// Write
			unsigned short wAlpha = 0;
			for(unsigned short wPosR = 0; wPosR < m_nHeight; ++wPosR)
			{
				for(unsigned short wPosC = 0; wPosC < m_nWidth; ++wPosC)
				{
					wAlpha = 0;
					if(pAlpha)
					{
						wAlpha += *pAlpha++;
						wAlpha += *pAlpha++;
						wAlpha += *pAlpha++;
						wAlpha /= 3;
					}
					else
						wAlpha = 0xFF;
#					if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
					if(pAlpha)
						pAlpha++;
#					endif
					*pWrite++ = *pRGB++;
					*pWrite++ = *pRGB++;
					*pWrite++ = *pRGB++;
#					if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
					pRGB++;
#					endif
					*pWrite++ = wAlpha;
				}
			}
		}
		return true;
		XFUNC_END_RETURN(bool, res, false);
	}
};

inline bool initWithJpegFile(cocos2d::CCImage* &pImg, const char* szJpg, int nLen)
{
	XFUNC_START();
	bool bRet = false;
	do 
	{
		char szAlpha[MAX_PATH];
		memset(szAlpha, 0, sizeof(char) * MAX_PATH);
		memcpy(szAlpha, szJpg, nLen);
		if(IsJpg(szJpg, nLen))
		{
			szAlpha[nLen - 4] = 'a';
			szAlpha[nLen - 3] = '.';
			szAlpha[nLen - 2] = 'j';
			szAlpha[nLen - 1] = 'p';
			szAlpha[nLen - 0] = 'g';
		}
		else
		{
			szAlpha[nLen - 5] = 'a';
			szAlpha[nLen - 4] = '.';
			szAlpha[nLen - 3] = 'j';
			szAlpha[nLen - 2] = 'p';
			szAlpha[nLen - 1] = 'e';
			szAlpha[nLen - 0] = 'g';
		}

		//XEC_TIMER_ST();
		cocos2d::CCImage stJpg;
		bRet = stJpg.initWithImageFileInLoadThread(szJpg, cocos2d::CCImage::kFmtJpg);
		if(!bRet)
		{
			bRet = stJpg.initWithImageFileInLoadThread(GetRealPath(szJpg), cocos2d::CCImage::kFmtJpg);
			if(!bRet)
			{
				CCLOG("Load jpg %s failed!", szJpg);
				break;
			}
		}
		cocos2d::CCImage stAlpha;
		bRet = stAlpha.initWithImageFileInLoadThread(szAlpha, cocos2d::CCImage::kFmtJpg);
		if(!bRet)
			bRet = stAlpha.initWithImageFileInLoadThread(GetRealPath(szAlpha), cocos2d::CCImage::kFmtJpg);
		if(bRet)
		{
			if(stJpg.getWidth() != stAlpha.getWidth() ||
				stJpg.getHeight() != stAlpha.getHeight())
			{
				CCLOG("jpg %s 's alpha is not %s!", szJpg, szAlpha);
				break;
			}
		}
		XImage* pXImg = new XImage();
		if(!pXImg)
		{
			CCLOG("new cocos2d::XImage failed!");
			break;
		}
		bRet = pXImg->InitWithJpeg(stJpg, stAlpha);
		if(!bRet)
		{
			CCLOG("InitWithJpeg failed!");
			break;
		}
		pImg = (cocos2d::CCImage*)pXImg;
		//XEC_OUTRUNSEC();
		bRet = true;
	} while (false);
	return bRet;
	XFUNC_END_RETURN(bool, res, false);
}

bool XResType::LoadRes_Png(XResType* pResInfo)
{
	XFUNC_START();
	do 
	{
		XResPNG* pRes = dynamic_cast<XResPNG*>(pResInfo);
		if (!pRes)
		{
			break;
		}

		int nLen = strlen(pRes->m_szPath);
		if(IsJpg(pRes->m_szPath, nLen) || IsJpeg(pRes->m_szPath, nLen))
		{
			if (!initWithJpegFile(pRes->m_pImage, pRes->m_szPath, nLen))
			{
				CC_SAFE_DELETE(pRes->m_pImage);
				CCLOG("can not load %s", pRes->m_szPath);
				break;
			}
			//if(strstr(pRes->m_szPath, "384"))
			//{
			//	pRes->m_pImage->saveToFile("Tex.png", false);
			//}
		}
		else
		{
			pRes->m_pImage = new cocos2d::CCImage();
			if (pRes->m_pImage && !pRes->m_pImage->initWithImageFileInLoadThread(pRes->m_szPath, cocos2d::CCImage::kFmtPng))
			{
				CC_SAFE_DELETE(pRes->m_pImage);
				CCLOG("can not load %s", pRes->m_szPath);
				break;
			}
		}
		//CCLOG("LoadRes_Png [%s]", pRes->m_szPath);
		
		return true;
	} while (false);
	XFUNC_END_RETURN(bool, res, false);
}

void XResPNG::Reload()
{
	XFUNC_START();
	if(m_pTexture)
	{
		if(1 < m_pTexture->retainCount() && !m_pTexture->_byReload)
		{
			++XResLoader::_dwReTotal;
			//CCLOG("[Texture]XResPNG::Reload prepare %s used %d", m_szPath, m_pTexture->getName());
			m_pTexture->retain();
			m_pTexture->_byReload = 0xFF;
			XResLoader::Instance().AddResReload((XEventObj*)m_pTexture, 
				(FUNC_HANDEL)&cocos2d::XTexture2D::ResHandle, GetRealPath(m_szPath), NULL);
		}
	}
	XFUNC_END();
}