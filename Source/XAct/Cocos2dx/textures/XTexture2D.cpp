#include "../../Common.h"
#include "ETC/ETC.h"
#include "DDS/S3TC.h"
#include "../../XConfig.h"
#include "../XLoader/XLoader.h"
XCPP_DEFINED();

NS_CC_BEGIN

void XSprFrmInfo::ResHandle( long lParam )
{
	XFUNC_START();
	do 
	{
		if(!pTex)
			break;
		CppHandle stHandle;
		stHandle.lParam = lParam;
		XResType* pResType = (XResType*)stHandle.pParam;
		if (!pResType)
		{
			break;
		}
		if (XResType::ePngReload == pResType->GetType())
		{
			XResPNG* pRes = dynamic_cast<XResPNG*>(pResType);
			if (!pRes)
			{
				break;
			}
			if (pRes->m_pImage)
			{
				bool bRes = pTex->initWithImage(pRes->m_pImage);
				if (!bRes)
				{
					break;
				}
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
			}
			//CCLOG("Reload %s", pRes->m_szPath);
			++XResLoader::_dwReloaded;
			_byReload = 0x00;
		}

		//static unsigned long dwRes = 0;
		//CCLOG("XSprFrmInfo::ResHandle [%d] %s", dwRes++, szPath);

		return;
	} while (0);
	CCLOG("[ERROR]XSprFrmInfo::ResHandle load res failed.");
	XFUNC_END();
}

void XTexture2D::ResHandle( long lParam )
{
	XFUNC_START();
	do 
	{
		CppHandle stHandle;
		stHandle.lParam = lParam;
		XResType* pResType = (XResType*)stHandle.pParam;
		if (!pResType)
		{
			break;
		}
		if (XResType::ePngReload == pResType->GetType())
		{
			XResPNG* pRes = dynamic_cast<XResPNG*>(pResType);
			if (!pRes)
			{
				break;
			}
			if (pRes->m_pImage)
			{
				bool bRes = initWithImage(pRes->m_pImage);
				if (!bRes)
				{
					CCLOG("【 - ERROR - 】XTexture2D::ResHandle initWithImage failed! %s", pRes->m_szPath);
					break;
				}
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
				//CCLOG("Reload %s", pRes->m_szPath);
			}
			//CCLOG("Reload %s", pRes->m_szPath);
			++XResLoader::_dwReloaded;
			_byReload = 0x00;
			release();
		}
		//static unsigned long dwRes = 0;
		//CCLOG("XTexture2D::ResHandle [%d] %s", dwRes++, szPath);

		return;
	} while (0);
	CCLOG("[ERROR]XTexture2D::ResHandle load res failed.");
	XFUNC_END();
}

bool XTexture2D::initWithETC1File( const char* file )
{
	XFUNC_START();
	XKTX_Info stKtxInfo;
	GLuint	dwTex = 0;
	GLenum	dwTar;
	KTX_dimensions	stDimen;
	GLboolean bIsMipMap;
	GLenum	dwGLErr;
	unsigned int	nKvdLen = 0;
	unsigned char*	pKvdData = NULL;
	KTX_error_code nErr = KTX_SUCCESS;

	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(file, "rb", &nSize);
	if (pBuffer != NULL && nSize > 0)
	{
		nErr = ktxLoadTextureM(pBuffer, nSize, &dwTex, &dwTar, &stDimen, &bIsMipMap, &dwGLErr, &nKvdLen, &pKvdData, &stKtxInfo);
		if (KTX_SUCCESS == nErr)
		{
			glTexParameteri(dwTar, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(dwTar, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(dwTar, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(dwTar, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			m_uName = dwTex;
			m_fMaxS = 1.f;
			m_fMaxT = 1.f;
			m_uPixelsWide = stKtxInfo.width;
			m_uPixelsHigh = stKtxInfo.height;
			m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
			m_bHasPremultipliedAlpha = false;
			m_ePixelFormat = kCCTexture2DPixelFormat_RGB888;
			m_bHasMipmaps = bIsMipMap ? true : false;
		}
		CCFileUtils::sharedFileUtils()->delFileData(pBuffer);
		return (KTX_SUCCESS == nErr);		
	}
	
	CCLOG("cocos2d: Couldn't load ETC1 image [%d]%s", nErr, file);
	XFUNC_END_RETURN(bool, res, false);
}

bool XTexture2D::initWithS3TCFile( const char* file )
{
	XFUNC_START();
	bool bRet = false;

	XS3TC::XDDS_Info stDdsInfo;
	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(file, "rb", &nSize);
	if (pBuffer != NULL && nSize > 0)
	{
		bRet = XS3TC::s3tcLoadTexture(pBuffer, nSize, stDdsInfo);
		if (bRet)
		{
			m_uName = stDdsInfo.dwTexture;
			m_fMaxS = 1.f;
			m_fMaxT = 1.f;

			m_uPixelsWide = stDdsInfo.stDDsc.dwWidth;
			m_uPixelsHigh = stDdsInfo.stDDsc.dwHeight;
			m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
			m_bHasPremultipliedAlpha = false;
			m_ePixelFormat = kCCTexture2DPixelFormat_RGB888;
			m_bHasMipmaps = (stDdsInfo.stDDsc.dwMipMapCount > 1);
		}
		CCFileUtils::sharedFileUtils()->delFileData(pBuffer);
		return bRet;		
	}

	CCLOG("cocos2d: Couldn't load DDS image %s", file);
	XFUNC_END_RETURN(bool, res, false);
}

XScrTexture::~XScrTexture()
{
	XMemLog::m_dwNew -= sizeof(XScrTexture);
}

void XScrTexture::InitWithBackBuffer()
{
	XFUNC_START();
	unsigned int powW = 0;
	unsigned int powH = 0;
	if (CCConfiguration::sharedConfiguration()->supportsNPOT())
	{
		powW = SCRPIX_W;
		powH = SCRPIX_H;
	}
	else
	{
		powW = xNextPOT(SCRPIX_W);
		powH = xNextPOT(SCRPIX_H);
	}
	char* szBuf = new char[(powW * powH) << 2];
	memset(szBuf, 0x00000000, sizeof(char) * ((powW * powH) << 2));
	bool bRet = initWithData(szBuf, kCCTexture2DPixelFormat_RGBA8888, powW, powH, CCSizeMake((float)SCRPIX_W, (float)SCRPIX_H));
	if(szBuf)
	{
		delete[] szBuf;
		szBuf = NULL;
	}
	if(!bRet)
	{
		CCLOG("cocos2d: InitWithBackBuffer failed");
		return;
	}
	setAliasTexParameters();
	XFUNC_END();
}

void XScrTexture::GetBackBuffer()
{
	glBindTexture(GL_TEXTURE_2D, m_uName);
	//CHECK_GL_ERROR_DEBUG();
	//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, SCRPIX_W, SCRPIX_H, 0);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SCRPIX_W, SCRPIX_H);
	static bool bFirst = true;
	if(bFirst)
	{
		bFirst = false;
		GLenum __error = glGetError();
		if(__error)
		{
			XConfig::m_byScrEffect = 0x00;
			CCLOG("[Screen Effect]Not support[0x%04x].", __error);
			cocos2d::XScrSprite::SetShaderType(cocos2d::XScrSprite::eShaderNone);
		}
	}
	//CHECK_GL_ERROR_DEBUG();
	glBindTexture(GL_TEXTURE_2D, 0);
	//CHECK_GL_ERROR_DEBUG();
}

NS_CC_END
