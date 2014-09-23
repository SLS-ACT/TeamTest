#include "XLoader.h"
#include <stdio.h>

XCPP_DEFINED();

XResType::XResInfo XResType::m_afnType[XResType::eNum];
unsigned long XResLoader::_dwLoaded = 1;
unsigned long XResLoader::_dwTotal = 1;
unsigned long XResLoader::_dwReloaded = 1;
unsigned long XResLoader::_dwReTotal = 1;

void XResLoader::Reload()
{
	XFUNC_START();
	// 重加载资源
	for(unsigned short wPos = 0; wPos < m_wResIdUsed; ++wPos)
	{
		if(m_aResInfo[wPos])
			m_aResInfo[wPos]->Reload();
	}
	// For UI
	XSprCallback* pSpr = m_stHashSprFile.Next(true);
	while(pSpr)
	{
		XSprCallback& stSInfo = *pSpr;
		if(stSInfo.pTex && !stSInfo._byReload)
		{
			++XResLoader::_dwReTotal;
			//CCLOG("[Texture]XResLoader::Reload prepare %s used %d", stSInfo.szTexPath, stSInfo.pTex->getName());
			stSInfo.pTex->retain();
			stSInfo._byReload = 0xFF;
			XResLoader::Instance().AddResReload((XEventObj*)pSpr, 
				(FUNC_HANDEL)&XSprCallback::ResHandle, GetRealPath(stSInfo.szTexPath), NULL);
		}
		pSpr = m_stHashSprFile.Next(false);
	}
	XFUNC_END();
}

unsigned short XResLoader::AddResLoad(unsigned char byType, XEventObj* pObj, FUNC_HANDEL pfnHandle, const char* szHashName, ...)
{
	XFUNC_START();
	if (!szHashName)
	{
		CCLOG("AddResLoaded no name");
		return false;
	}

	if(XResType::ePngReload == byType)
	{
		CCLOG("XResLoader::AddResLoad XMP_CREATE(XResPNG) failed!");
		return 0xFFFF;
	}

	do 
	{
		if (byType >= XResType::eNum || !pObj || !pfnHandle)
		{
			CCLOG("AddResLoad error 1");
			break;
		}
		if (!XResType::m_afnType[byType].pfnInit || !XResType::m_afnType[byType].pfnLoad)
		{
			CCLOG("AddResLoad error 2");
			break;
		}
		XResType* pResType = NULL;
		// search hash
		pResType = m_stHashRes.Get(szHashName, false);
		if (pResType)
		{
			// 已存在，注册回调
			if (XResType::eLoadFailed != pResType->m_byStates)
			{
				pResType->AddHandle(pObj, pfnHandle);
			}
			else
			{
				CCLOG("AddResLoad error 3");
				break;
			}
			// 已回调，重新回调
			if(XResType::eCallback == pResType->m_byStates)
			{
				pResType->m_byStates = XResType::eLoadSucc;
				m_stLstCallback.AddObj(pResType);
				++_dwTotal;
			}
			// 需要重新加载
			if(XResType::eClean == pResType->m_byStates)
			{
				pResType->m_byStates = XResType::eWait;
				(*XResType::m_afnType[byType].pfnReset)(pResType);
				//printf("[ref:%d]%s\n", pRes->m_pTexture->retainCount(), szPath);
				//CCLOG("[XResLoader::AddResLoad]%s", szHashName);
				// 加入Add列表
				m_stLockAdd.Lock();
				m_stLstAdd.AddObj(pResType);
				++_dwTotal;
				m_stLockAdd.Unlock();
			}
		}
		else
		{
			const int nMaxParam = 4;
			const char* szParam[nMaxParam] = {0};
			int nParamNum = 0;
			va_list ap;
			va_start(ap, szHashName);
			szParam[0] = szHashName;
			for (nParamNum = 1; nParamNum < nMaxParam; ++nParamNum)
			{
				szParam[nParamNum] = va_arg(ap, const char*);
				if (!szParam[nMaxParam])
				{
					break;
				}
			}	
			va_end(ap);
			switch (nParamNum)
			{
			case 1:
				pResType = (*XResType::m_afnType[byType].pfnInit)(szParam[0]);
				break;
			case 2:
				pResType = (*XResType::m_afnType[byType].pfnInit)(szParam[0], szParam[1]);
				break;
			case 3:
				pResType = (*XResType::m_afnType[byType].pfnInit)(szParam[0], szParam[1], szParam[2]);
				break;
			case 4:
				pResType = (*XResType::m_afnType[byType].pfnInit)(szParam[0], szParam[1], szParam[2], szParam[3]);
				break;
			}
			if (!pResType)
			{
				CCLOG("AddResLoad error 4");
				break;
			}
			strncpy(pResType->m_szPath, szHashName, sizeof(char)*MAX_PATH);
			// 加入hash
			pResType->m_wResID = m_wResIdUsed;
			m_aResInfo[m_wResIdUsed++] = pResType;
			m_stHashRes.Push(szHashName, pResType, false);
			// 初始化
			pResType->m_byType = byType;
			pResType->m_byStates = XResType::eWait;
			// 注册回调
			pResType->AddHandle(pObj, pfnHandle);
			// 加入Add列表
			m_stLockAdd.Lock();
			m_stLstAdd.AddObj(pResType);
			++_dwTotal;
			m_stLockAdd.Unlock();
		}
		return pResType->m_wResID;

	} while (false);
	CCLOG("XResLoader::AddResLoad failed:[%d]%s", byType, szHashName);
	XFUNC_END_RETURN(unsigned short, res, 0xFFFF);
}

unsigned short XResLoader::AddResReload(XEventObj* pObj, FUNC_HANDEL pfnHandle, const char* szHashName, ...)
{
	XFUNC_START();
	if (!szHashName)
	{
		CCLOG("AddResLoaded no name");
		return false;
	}
	XResPNG* pResType = XMP_CREATE(XResPNG);
	if (!pResType)
	{
		CCLOG("XResLoader::AddResLoad XMP_CREATE(XResPNG) failed!");
		return 0xFFFF;
	}
	strncpy(pResType->m_szPath, szHashName, sizeof(char)*MAX_PATH);
	pResType->m_wResID = 0xFFFF;
	pResType->m_byType = XResType::ePngReload;
	pResType->m_byStates = XResType::eWait;
	// 注册回调
	pResType->AddHandle(pObj, pfnHandle);
	// 加入Add列表
	m_stLockAdd.Lock();
	m_stLstAdd.AddObj(pResType);
	++_dwTotal;
	m_stLockAdd.Unlock();
	XFUNC_END_RETURN(unsigned short, res, 0xFFFF);
}

void XResLoader::StartLoadThread()
{
	XFUNC_START();
	m_stLoadThread.Start(this, &XResLoader::XThreadLoad);
	XFUNC_END();
}

void XResLoader::EndLoadThread()
{
	XFUNC_START();
	m_stLoadThread.End();
	XFUNC_END();
}
void XResLoader::UpdateCallback(float fSleepT)
{
	XFUNC_START();
	// 加载完队列转移到回调队列
	m_stLockLoaded.Lock();
	m_stLstCallback.AddList(m_stLstLoaded);
	m_stLstLoaded.Clear();
	m_stLockLoaded.Unlock();

	if (!m_stLstCallback._pHead)
	{
		// 尝试清理资源
		for(unsigned short wPos = 0; wPos < m_wResIdUsed; ++wPos)
		{
			if(m_aResInfo[wPos])
				m_aResInfo[wPos]->Release();
		}
		return;
	}
	// 计时
	static struct cocos2d::cc_timeval stNow;
	if (cocos2d::CCTime::gettimeofdayCocos2d(&stNow, NULL) != 0)
	{
		CCLOG("XResLoader::UpdateCallback error in gettimeofdayCocos2d stNow");
		return;
	}
	static struct cocos2d::cc_timeval stNext;
	float fRunTime = 0.f;
	// 遍历回调队列
	XResType* &pCBHeader = m_stLstCallback._pHead;
	if (pCBHeader)
	{
		XResType* pCBClear;
		unsigned long dwCBNum = 0, dwRunNum = 0;
		do 
		{
			// 回调
			dwRunNum = pCBHeader->OnCallbackOne();
			if (dwRunNum)
			{
				dwCBNum += dwRunNum;
			}
			// 检测超时
			if (cocos2d::CCTime::gettimeofdayCocos2d(&stNext, NULL) != 0)
			{
				CCLOG("XResLoader::UpdateCallback error in gettimeofdayCocos2d stNext");
				return;
			}
			fRunTime += ((stNext.tv_sec - stNow.tv_sec) + (stNext.tv_usec - stNow.tv_usec) / 1000000.f);
			//CCLOG("[XResLoader::UpdateCallback]%s[%fs]", pCBHeader->m_szPath, fRunTime);
			if (!dwRunNum)
			{
				pCBClear = pCBHeader;
				pCBHeader = pCBHeader->_pNext;
				pCBClear->m_byStates = XResType::eCallback;
				if (XResType::ePngReload == pCBClear->GetType())
				{
					XResPNG* pRes = dynamic_cast<XResPNG*>(pCBClear);
					if (pRes)
						XMP_RELEASE(XResPNG, pRes);
				}
				//if (XResType::ePng == pCBClear->GetType())
				//{
				//	XResPNG* pRes = dynamic_cast<XResPNG*>(pCBClear);
				//	if (pRes)
				//	{
				//		if(pRes->m_pTexture)
				//			CCLOG("[Texture]XResType::LoadRes_Png %s used %d", pRes->m_szPath, pRes->m_pTexture->getName());
				//	}
				//}
				++_dwLoaded;
			}
			//break;
		} while (fRunTime < fSleepT && pCBHeader);
		if (!pCBHeader)
		{
			m_stLstCallback.Clear();		
		}		
	}
	XFUNC_END();
}

void XResLoader::ClearAll()
{
	XFUNC_START();
	XFUNC_END();
}

void XSprCallback::ResHandle( long lParam )
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
				//CCLOG("[Texture]XSprCallback::ResHandle reload %s used %d", pResType->m_szPath, pTex->getName());
				// 已将图片转化为贴图，删除图片，释放内存。
				delete pRes->m_pImage;
				pRes->m_pImage = NULL;
			}
			//CCLOG("Reload %s", pRes->m_szPath);
			++XResLoader::_dwReloaded;
			_byReload = 0x00;
			pTex->release();
		}

		if(!dwRef)
		{
			//CCLOG("[Texture]XSprCallback::ResHandle DelSpr %s", pResType->m_szPath);
			XResLoader::Instance().m_stHashSprFile.Del(szTexPath, false);
			XSprCallback* pSpr = this;
			XMP_RELEASE(XSprCallback, pSpr);
		}
		//static unsigned long dwRes = 0;
		//CCLOG("XSprCallback::ResHandle [%d] %s", dwRes++, szPath);

		return;
	} while (0);
	CCLOG("[ERROR]XSprCallback::ResHandle load res failed.");
	XFUNC_END();
}

XSprCallback* XResLoader::RegSpr(const char* szSpr, cocos2d::CCTexture2D* pTex)
{
	XFUNC_START();
	if (!szSpr || !pTex)
	{
		CCLOG("RegSpr no name");
		return NULL;
	}
	XSprCallback* _pFileReload = NULL;
	// search hash
	_pFileReload = m_stHashSprFile.Get(szSpr, false);
	if (!_pFileReload)
	{
		_pFileReload = XMP_CREATE(XSprCallback);
		if(_pFileReload)
		{
			_pFileReload->_byReload = 0x00;
			_pFileReload->pTex = pTex;
			strncpy(_pFileReload->szTexPath, szSpr, sizeof(char) * MAX_PATH);
			m_stHashSprFile.Push(szSpr, _pFileReload, false);
			//CCLOG("[Texture]XResLoader::RegSpr %s used %d", szSpr, pTex->getName());
		}
	}
	if(_pFileReload)
		++_pFileReload->dwRef;
	return _pFileReload;
	XFUNC_END_RETURN(XSprCallback*, ptr, NULL);
	return NULL;
}

void XResLoader::DelSpr(XSprCallback* pSpr)
{
	XFUNC_START();
	if (!pSpr)
		return;
	if(pSpr->dwRef)
		--pSpr->dwRef;
	if(!pSpr->dwRef)
	{
		if(!pSpr->_byReload)
		{
			//CCLOG("[Texture]XResLoader::DelSpr %s", pSpr->szTexPath);
			m_stHashSprFile.Del(pSpr->szTexPath, false);
			XMP_RELEASE(XSprCallback, pSpr);
		}
	}
	return;
	XFUNC_END();
	return;
}

unsigned long XResLoader::XThreadLoad(long param, volatile bool& bExit)
{
	XFUNC_START();
	bool bLoadRes = false;
	unsigned char byType = XResType::eUnknow;
	while(!bExit)
	{
		// add 队列转移到加载队列
		m_stLockAdd.Lock();
		m_stLstLoading.AddList(m_stLstAdd);
		m_stLstAdd.Clear();
		m_stLockAdd.Unlock();
		if (!m_stLstLoading._pHead)
		{
			m_stLoadThread.SleepMs(30);
			continue;
		}
		// 遍历加载队列
		XResType* pLoadRes = m_stLstLoading._pHead;
		XResType* pLoadNow = NULL;
		while(pLoadRes)
		{
			pLoadNow = pLoadRes;
			pLoadRes = pLoadRes->_pNext;
			do 
			{
				byType = pLoadNow->m_byType;
				if (byType >= XResType::eNum)
				{
					break;
				}
				bLoadRes = (*XResType::m_afnType[byType].pfnLoad)(pLoadNow);
				if (bLoadRes)
				{
					pLoadNow->m_byStates = XResType::eLoadSucc;
				}
#				if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				// 降低内存峰值
				m_stLoadThread.SleepMs(30);
#				endif
			} while (false);
			if (XResType::eWait == pLoadNow->m_byStates)
			{
				pLoadNow->m_byStates = XResType::eLoadFailed;
			}
			m_stLockLoaded.Lock();
			m_stLstLoaded.AddObj(pLoadNow);
			m_stLockLoaded.Unlock();
		}
		// 清空加载队列
		m_stLstLoading.Clear();
	}
	XFUNC_END_RETURN(unsigned short, res, 0);
}