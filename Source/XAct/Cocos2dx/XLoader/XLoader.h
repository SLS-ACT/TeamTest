#ifndef __XLOADER_H__
#define __XLOADER_H__
#include "../../Common.h"
#include "../../XHelper/XHelper.h"
#include "../../XEvent/XEvent.h"
#include "../textures/XTextureCache.h"
#include "XA8Reader.h"

// 最多支持的资源数目：0xFFFF is invalid value
#define XRES_MAX			(0x2800)		// 10240
#define XRES_SPRFILE_MAX	(0x0100)		// 256

// 资源加载完成后回调链表
class XResCallback :
	public CppEvent,
	public XMemPool<XResCallback>
{
public:
	XResCallback()
	{
		Reset();
		XMemLog::m_dwNew += sizeof(XResCallback);
	}
	~XResCallback() {
		XMemLog::m_dwNew -= sizeof(XResCallback);
	}
public:
	XResCallback* _pNext;
	inline void Reset()
	{
		_pNext = NULL;
		pEvtObj = NULL;
		pFuncHandel = NULL;
	}
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XResCallback::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		Reset();
		return true;
	}
	virtual void OnRelease()
	{
		Reset();
	}
};

// 从文件创建的精灵在reload时的回调链表
class XSprCallback :
	public XMemPool<XSprCallback>
{
public:
	XSprCallback()
	{
		Reset();
		XMemLog::m_dwNew += sizeof(XSprCallback);
	}
	~XSprCallback() {
		XMemLog::m_dwNew -= sizeof(XSprCallback);
	}
public:
	void ResHandle(long lParam);
	unsigned char			_byReload;
	unsigned long			dwRef;
	cocos2d::CCTexture2D*	pTex;
	char					szTexPath[MAX_PATH];
public:
	inline void Reset()
	{
		_byReload = 0x00;
		dwRef = 0;
		pTex = NULL;
	}
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XSprCallback::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		Reset();
		return true;
	}
	virtual void OnRelease()
	{
		Reset();
	}
};

// 资源类型基类
class XResType
{
public:
	enum
	{
		eUnknow = -1,
		ePng,
		ePngReload,
		eXA8,
		eXA8Reload,

		eNum
	};
	enum
	{
		eWait = 0,
		eLoadSucc,
		eLoadFailed,
		eCallback,
		eClean,
	};
public:
	XResType() : _pNext(NULL), m_byType(eUnknow), m_byStates(eWait), m_wResID(0xFFFF) {
	}
	virtual ~XResType() {
	}
	virtual void Release() = 0;
	virtual void Reload() = 0;
public:
	inline void AddHandle(XEventObj* pObj, FUNC_HANDEL pfnHandle)
	{
		XResCallback* pRCB = XMP_CREATE(XResCallback);
		pRCB->pEvtObj = pObj;
		pRCB->pFuncHandel = pfnHandle;
		_pLink.AddObj(pRCB);
	}
	inline unsigned long OnCallback()
	{
		unsigned long dwNum = 0;
		XResCallback* pHead = _pLink._pHead;
		XResCallback* pDel;
		while(pHead)
		{
			++dwNum;
			(pHead->pEvtObj->*(pHead->pFuncHandel))(m_wResID);
			pDel = pHead;
			pHead = pHead->_pNext;
			XMP_RELEASE(XResCallback, pDel);
		}
		_pLink.Clear();
		return dwNum;
	}

	inline unsigned long OnCallbackOne()
	{
		XResCallback* &pHead = _pLink._pHead;
		XResCallback* pDel;
		CppHandle stHandle;
		if (pHead)
		{
			if (pHead->pEvtObj && pHead->pFuncHandel)
			{
				if(0xFFFF == m_wResID)
				{
					stHandle.pParam = this;
					(pHead->pEvtObj->*(pHead->pFuncHandel))(stHandle.lParam);
				}
				else
					(pHead->pEvtObj->*(pHead->pFuncHandel))(m_wResID);
			}
			pDel = pHead;
			pHead = pHead->_pNext;
			--_pLink._dwNum;
			if(_pLink._pEnd == pDel)
				_pLink._pEnd = NULL;
			XMP_RELEASE(XResCallback, pDel);
			return 1;
		}
		_pLink.Clear();
		return 0;
	}
	inline unsigned char GetType() {
		return m_byType;
	}
public:
	XResType* _pNext;
	char m_szPath[MAX_PATH];
protected:
	unsigned char			m_byType;
	volatile unsigned char	m_byStates;
	unsigned long			m_wResID;
	XSinglyLink<XResCallback>	_pLink;
protected:
	typedef XResType* (*PNG_InitRes)(const char* szHashName, ...);
	typedef void (*PFN_ResetRes)(XResType* pResInfo);
	typedef bool (*PNG_LoadRes)(XResType* pResInfo);
	struct XResInfo
	{
		PNG_InitRes		pfnInit;
		PFN_ResetRes	pfnReset;
		PNG_LoadRes		pfnLoad;
		XResInfo() : pfnLoad(NULL), pfnReset(NULL), pfnInit(NULL) {
			XMemLog::m_dwNew += sizeof(XResInfo);
		}
		~XResInfo() {
			XMemLog::m_dwNew -= sizeof(XResInfo);
		}
	};
	// 按类型加载
	static XResInfo m_afnType[eNum];
	// 初始化
	friend class XResLoader;
	static void InitLoadFunc()
	{
		memset(m_afnType, 0, sizeof(XResInfo) * eNum);
		m_afnType[ePng].pfnInit				= &XResType::InitRes_Png;
		m_afnType[ePng].pfnReset			= &XResType::ResetRes_Png;
		m_afnType[ePng].pfnLoad				= &XResType::LoadRes_Png;
		m_afnType[ePngReload].pfnInit		= &XResType::InitRes_Png;
		m_afnType[ePngReload].pfnReset		= &XResType::ResetRes_Png;
		m_afnType[ePngReload].pfnLoad		= &XResType::LoadRes_Png;
		m_afnType[eXA8].pfnInit				= &XResType::InitRes_XA8;
		m_afnType[eXA8].pfnReset			= &XResType::ResetRes_XA8;
		m_afnType[eXA8].pfnLoad				= &XResType::LoadRes_XA8;
		m_afnType[eXA8Reload].pfnInit		= &XResType::InitRes_XA8;
		m_afnType[eXA8Reload].pfnReset		= &XResType::ResetRes_XA8;
		m_afnType[eXA8Reload].pfnLoad		= &XResType::LoadRes_XA8;
	}
	// 只需要一个参数，图像路径
	static XResType* InitRes_Png(const char* szPath, ...);
	static void		ResetRes_Png(XResType* pResInfo);
	static bool		LoadRes_Png(XResType* pResInfo);
	// 压缩纹理
	static XResType* InitRes_XA8(const char* szPath, ...);
	static void		ResetRes_XA8(XResType* pResInfo);
	static bool		LoadRes_XA8(XResType* pResInfo);
};

// 资源加载器
class XResLoader :
	public XSingleton<XResLoader>
{
public:
	XResLoader()
	{
		XResType::InitLoadFunc();
		memset(m_aResInfo, 0, sizeof(XResType*) * XRES_MAX);
		m_wResIdUsed = 0;
		XMemLog::m_dwNew += sizeof(XResLoader);
	}
	~XResLoader() {
		XMemLog::m_dwNew -= sizeof(XResLoader);
	}
public:
	unsigned short AddResLoad(unsigned char byType, XEventObj* pObj, FUNC_HANDEL pfnHandle, const char* szHashName, ...);
	unsigned short AddResReload(XEventObj* pObj, FUNC_HANDEL pfnHandle, const char* szHashName, ...);
public:
	void StartLoadThread();
	void EndLoadThread();
	// 更新回调 ： 传入主线程可以休息的时间，内部变化这个值，如果不为0代表加载完成后主线程可以休眠这么多时间
	void UpdateCallback(float fSleepT);
	// 清理所有资源
	void ClearAll();
	// 重加载所有资源
	void Reload();
public:
	inline XResType*	GetResByID(unsigned short wID)
	{
		if (wID >= XRES_MAX)
		{
			return NULL;
		}
		return m_aResInfo[wID];		
	}
public:
	static unsigned long	_dwLoaded;
	static unsigned long	_dwTotal;
protected:
	OGDP::XThread<XResLoader>		m_stLoadThread;
	unsigned long XThreadLoad(long param, volatile bool& bExit);
	struct XResList :
		public XSinglyLink<XResType>
	{
		XResList() {
			XMemLog::m_dwNew += sizeof(XResList);
		}
		~XResList() {
			XMemLog::m_dwNew -= sizeof(XResList);
		}
		inline void AddList(XResList& stObj)
		{
			if (!_pHead)
			{
				_pHead = stObj._pHead;
			}
			if (_pEnd)
			{
				_pEnd->_pNext = stObj._pHead;
			}
			if (stObj._pHead)
			{
				_pEnd = stObj._pEnd;
			}			
		}
	};
	// 多线程锁
	OGDP::XThreadLock	m_stLockAdd;
	OGDP::XThreadLock	m_stLockLoaded;
	// 队列
	XResList			m_stLstAdd;			// 加入资源队列：主线程/加载线程
	XResList			m_stLstLoading;		// 加载队列：加载线程
	XResList			m_stLstLoaded;		// 已经加载完队列：主线程/加载线程
	XResList			m_stLstCallback;	// 回调队列：主线程
	XResList			m_stLstClear;		// 清理队列：主线程用，所有已加载完成的资源
	// hash and Res array
	typedef OGDP::XStrHash<XResType, XRES_MAX, MAX_PATH, 4096>	XHashRes;
	XHashRes			m_stHashRes;
	XResType*			m_aResInfo[XRES_MAX];
	unsigned short		m_wResIdUsed;
public:		// for XSprite::create reload
	XSprCallback* RegSpr(const char* szSpr, cocos2d::CCTexture2D* pTex);
	void DelSpr(XSprCallback* pSpr);
protected:	// hash for XSprite::create
	typedef OGDP::XStrHash<XSprCallback, XRES_SPRFILE_MAX, MAX_PATH, (XRES_SPRFILE_MAX >> 2)>	XHashSprFile;
	XHashSprFile		m_stHashSprFile;
	friend class XSprCallback;
public:
	static unsigned long	_dwReloaded;
	static unsigned long	_dwReTotal;
};

// 资源类型：PNG图片
class XResPNG :
	public XResType,
	public XMemPool<XResPNG>
{
public:
	XResPNG() {
		m_byType = XResType::ePng;
		XMemLog::m_dwNew += sizeof(XResPNG);
	}
	virtual ~XResPNG() {
		XMemLog::m_dwNew -= sizeof(XResPNG);
	}
	virtual void Release()
	{
		if(XResType::eCallback != m_byStates)
			return;
		if(m_pTexture)
		{
			// 没人引用了就删除
			if(2 >= m_pTexture->retainCount())
			{
				m_pTexture->release();
				//printf("[delete ref:%d]%s\n", m_pTexture->retainCount(), m_szPath);
				cocos2d::XTextureCache::Instance().removeTexture(m_pTexture);
				//printf("[delete ref:%d]%s\n", m_pTexture->retainCount(), m_szPath);
				m_pTexture = NULL;
				m_byStates = XResType::eClean;
			}
		}
		//XResPNG* pRes = this;
		//XMP_RELEASE(XResPNG, pRes);
	}
	virtual void Reload();
public:
	cocos2d::CCImage*		m_pImage;
	cocos2d::XTexture2D*	m_pTexture;
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XResPNG::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		m_byStates = XResType::eWait;
		memset(m_szPath, 0, sizeof(char)*MAX_PATH);
		m_pImage = NULL;
		m_pTexture = NULL;
		return true;
	}

	virtual void OnRelease()
	{
		m_byStates = XResType::eWait;
		CC_SAFE_DELETE(m_pImage);
		CC_SAFE_DELETE(m_pTexture);
	}
};

// 资源类型：XA8动画
#define MAX_FRAME	(32)
class XResXA8 :
	public XResType,
	public XMemPool<XResXA8>
{
public:
	XResXA8() {
		m_byType = XResType::eXA8;
		XMemLog::m_dwNew += sizeof(XResXA8);
	}
	virtual ~XResXA8() {
		XMemLog::m_dwNew -= sizeof(XResXA8);
	}
	virtual void Release()
	{
		if(XResType::eCallback != m_byStates)
			return;
		unsigned short wUnused = 0;
		for(int nPos = 0; nPos < MAX_FRAME; ++nPos)
		{
			if(m_pTexture[nPos])
			{
				if(2 >= m_pTexture[nPos]->retainCount())
					++wUnused;
			}
		}
		if(wUnused >= m_wFrameNum)
		{
			for(int nPos = 0; nPos < MAX_FRAME; ++nPos)
			{
				if(m_pTexture[nPos])
				{
					m_pTexture[nPos]->release();
					m_pTexture[nPos] = NULL;
				}
				if(m_pTexAlpha[nPos])
				{
					m_pTexAlpha[nPos]->release();
					m_pTexAlpha[nPos] = NULL;
				}
			}
			if(m_pTexPal)
			{
				m_pTexPal->release();
				m_pTexPal = NULL;
			}
			m_byStates = XResType::eClean;
		}
	}
	virtual void Reload();
public:
	XA8Reader*				m_pImage;
	unsigned short			m_wFrameNum;
	cocos2d::XTexture2D*	m_pTexture[MAX_FRAME];
	cocos2d::XTexture2D*	m_pTexAlpha[MAX_FRAME];
	cocos2d::XTexture2D*	m_pTexPal;
	struct XA8Offset {
		short nX;
		short nY;
	}						m_aOffset[MAX_FRAME];
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XResXA8::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		m_byStates = XResType::eWait;
		memset(m_szPath, 0, sizeof(char)*MAX_PATH);
		m_pImage = NULL;
		m_wFrameNum = 0;
		for(int nPos = 0; nPos < MAX_FRAME; ++nPos)
		{
			m_pTexture[nPos] = NULL;
			m_pTexAlpha[nPos] = NULL;
		}
		m_pTexPal = NULL;
		return true;
	}
	virtual void OnRelease()
	{
		m_byStates = XResType::eWait;
		CC_SAFE_DELETE(m_pImage);
		for(int nPos = 0; nPos < MAX_FRAME; ++nPos)
		{
			CC_SAFE_DELETE(m_pTexture[nPos]);
			CC_SAFE_DELETE(m_pTexAlpha[nPos]);
		}
		CC_SAFE_DELETE(m_pTexPal);
	}
};
#endif // __XLOADER_H__
