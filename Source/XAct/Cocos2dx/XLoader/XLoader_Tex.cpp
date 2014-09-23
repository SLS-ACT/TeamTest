#include "XLoader.h"

XCPP_DEFINED();

XResType* XResType::InitRes_XA8(const char* szPath, ...)
{
	XFUNC_START();
	do 
	{
		XResXA8* pRes = XMP_CREATE(XResXA8);
		if (!pRes)
		{
			CCLOG("XMP_CREATE(XResXA8) failed!");
			break;
		}
		ResetRes_XA8(pRes);
		return pRes;
	} while (false);
	XFUNC_END_RETURN(XResType*, ptr, NULL);
}

void XResType::ResetRes_XA8(XResType* pResInfo)
{
	XFUNC_START();
	do 
	{
		//XResXA8* pRes = dynamic_cast<XResXA8*>(pResInfo);
		//if (!pRes)
		//{
		//	break;
		//}
	} while (false);
	XFUNC_END();
}

bool XResType::LoadRes_XA8(XResType* pResInfo)
{
	XFUNC_START();
	do 
	{
		XResXA8* pRes = dynamic_cast<XResXA8*>(pResInfo);
		if (!pRes)
			break;
		int nLen = strlen(pRes->m_szPath);
		if(!IsXA8(pRes->m_szPath, nLen))
			break;
		bool bRet = false;
		unsigned long nSize = 0;
#		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		unsigned char *pBuffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileDataThread(pRes->m_szPath, "rb", &nSize, 0x00);
#		else
		unsigned char *pBuffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(pRes->m_szPath, "rb", &nSize);
#		endif
		if (pBuffer != NULL && nSize > 0)
		{
			//XEC_TIMER_ST();
			pRes->m_pImage = new XA8Reader;
			if(pRes->m_pImage)
			{
				bRet = pRes->m_pImage->Read(pBuffer, nSize);
				if(!bRet)
				{
					CCLOG("Open %s failed : %s", pRes->m_szPath, pRes->m_pImage->szErr);
				}
			}
			cocos2d::CCFileUtils::sharedFileUtils()->delFileData(pBuffer);
			//XEC_OUTRUNSEC();
		}
		return bRet;
	} while (false);
	XFUNC_END_RETURN(bool, res, false);
}

void XResXA8::Reload()
{
	XFUNC_START();
	XFUNC_END();
}
