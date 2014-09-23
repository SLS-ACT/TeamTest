#ifndef __XANIMATION_H__
#define __XANIMATION_H__
/*
	动画类
*/
#include "../../Common.h"

#define MAX_TEX_NUM		(65536)
#define MAX_ANI_NUM		(65536)

#define MAX_EFFECT_NUM	(5)

class XResObj
{
public:
	XResObj() {
		m_dwRefNum = 0;
	}
	virtual ~XResObj() {}
public:
	virtual void OnLoad()		= 0;
	virtual void OnRelease()	= 0;
public:
	inline unsigned long AddRef() {
		if(!m_dwRefNum)
			OnLoad();
		return ++m_dwRefNum;
	}
	inline unsigned long DelRef() {
		if(m_dwRefNum) {
			--m_dwRefNum;
			if(!m_dwRefNum)
				OnRelease();
		}
		return 0;
	}
	inline unsigned long GetRef() {
		return m_dwRefNum;
	}
protected:
	unsigned long m_dwRefNum;
};

struct XFrameInfo 
{
	unsigned short	wDrawTexID;		// 渲染贴图ID
	short			nMoveX;			// 帧位移X
	short			nSpdH;			// 帧速度H
	short			nOffsetX;		// 渲染偏移X
	short			nOffsetH;		// 渲染偏移H

	short			nRcX;			// 判定块X坐标
	short			nRcY;			// 判定块Y坐标
	short			nRcW;			// 判定块宽度
	short			nRcH;			// 判定块高度
	unsigned char	byThickness;	// 判定块厚度
	unsigned char	byType;			// 判定块类型

	unsigned short	wEffectID[MAX_EFFECT_NUM];// 帧特效ID
	unsigned short	wCameraID;		// 镜头特效ID
	unsigned short	wSoundID;		// 帧音效ID
	unsigned char	byStay;			// 帧残影残留
	unsigned char	byBreak;		// 是否可以此帧后打断
	unsigned char	byHurtBreak;	// 手否是霸体帧
	bool			byFlipX;		// 是否镜像

	XFrameInfo() {
		wDrawTexID = 0xFFFF;
		nMoveX = 0;
		nSpdH = 0;
		nOffsetX = 0;
		nOffsetH = 0;
		nRcX = 0;
		nRcY = 0;
		nRcW = 0;
		nRcH = 0;
		byThickness = 0;
		byType = 0xFF;
		for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
		{
			wEffectID[byCnt] = 0xFFFF;
		}
		wSoundID = 0xFFFF;
		wCameraID = 0xFFFF;
		byStay = 0;
		byBreak = 0;
		byHurtBreak = 0;
		byFlipX = false;
		XMemLog::m_dwNew += sizeof(XFrameInfo);
	}
	virtual ~XFrameInfo() {
		XMemLog::m_dwNew -= sizeof(XFrameInfo);
	}
};

class XDrawTex :
	public XResObj
{
	friend class XDrawAnimation;
public:
	XDrawTex() {
		m_pSpr = NULL;
		m_wSprID = 0xFFFF;
		m_apSprBN = NULL;
		m_apSprBN_FlipX = NULL;
		m_wSprBNNum = 0;
		adwXSprCutID = NULL;
		wXSprCutNum = 0;
		awXTexCutID = NULL;
		byXTexCutNum = 0;
		m_nTexW = 0;
		m_nTexH = 0;
		m_dwMemUsed = 0;
		_fChildrenScaleY = 1.f;
		XMemLog::m_dwNew += sizeof(XDrawTex);
	}
	virtual ~XDrawTex() {
		XMemLog::m_dwNew -= m_dwMemUsed;
		XMemLog::m_dwNew -= sizeof(XDrawTex);
		if(m_apSprBN) {
			delete[] m_apSprBN;
			m_apSprBN = NULL;
		}
		if(m_apSprBN_FlipX) {
			delete[] m_apSprBN_FlipX;
			m_apSprBN_FlipX = NULL;
		}
		if(adwXSprCutID) {
			delete[] adwXSprCutID;
			adwXSprCutID = NULL;
		}
		if(awXTexCutID) {
			delete[] awXTexCutID;
			awXTexCutID = NULL;
		}
	}
	inline void SetSprId(unsigned short wSprID) {
		m_wSprID = wSprID;
	}
	inline virtual void setChildrenScaleY(float fScaleY) {
		_fChildrenScaleY = fScaleY;
	}
public:
	virtual void OnLoad();
	virtual void OnRelease();
public:
	virtual void Draw(float fPosX, float fPosY, float fScale, cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX);
	virtual void DrawEx(float fPosX, float fPosY, float fScaleX, float fScaleY,
		cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP);
protected:
	cocos2d::XSprite*				m_pSpr;
	unsigned short					m_wSprID;
	short							m_nTexW;
	short							m_nTexH;
	unsigned long					m_dwMemUsed;
	float							_fChildrenScaleY;
protected:
	// 裁剪精灵信息
	unsigned long*	adwXSprCutID;
	unsigned short	wXSprCutNum;
	// 裁剪贴图信息
	unsigned short*	awXTexCutID;
	unsigned char	byXTexCutNum;
	// 批次渲染
	cocos2d::XSpriteBatchNode**		m_apSprBN;
	cocos2d::XSpriteBatchNode**		m_apSprBN_FlipX;
	unsigned short					m_wSprBNNum;	
public:
	static void Initialize();
	static void DumpUsedInfo();
public:
	static XDrawTex*				m_oapTex[MAX_TEX_NUM];
public:
	static unsigned char			_byUseCutTex;
};

class XDrawAnimation :
	public XResObj
{
public:
	XDrawAnimation() {
		m_apFrames = NULL;
		m_wFrameNum = 0;
		m_wFlyH = 0;
		m_wFlySpd = 0;
		_fChildrenScaleY = 1.f;
		m_byColorAdd = 0;
		XMemLog::m_dwNew += sizeof(XDrawAnimation);
	}
	virtual ~XDrawAnimation() {
		XMemLog::m_dwNew -= sizeof(XDrawAnimation);
		if(m_apFrames) {
			delete[] m_apFrames;
			m_apFrames = NULL;
		}
	}
public:
	virtual void OnLoad();
	virtual void OnRelease();
public:
	inline void GetSize(unsigned short wFrame, unsigned short& wW, unsigned short& wH) {
		wW = wH = 0;
		if(!m_apFrames)
			return;
		if(wFrame >= m_wFrameNum)
			return;
		const XFrameInfo& stFI = m_apFrames[wFrame];
		if(0xFFFF == stFI.wDrawTexID)
			return;
		if(!XDrawTex::m_oapTex[stFI.wDrawTexID])
			return;
		wW = (unsigned short)XDrawTex::m_oapTex[stFI.wDrawTexID]->m_nTexW;
		wH = (unsigned short)XDrawTex::m_oapTex[stFI.wDrawTexID]->m_nTexH;
	}
	inline virtual void setChildrenScaleY(float fScaleY) {
		_fChildrenScaleY = fScaleY;
	}
public:
	virtual void OnDraw(unsigned short wFrame, float fPosX, float fPosY, float fScale,
		cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX);
	virtual void OnDrawEx(unsigned short wFrame, float fPosX, float fPosY, float fScaleX, float fScaleY,
		cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP);
public:
	XFrameInfo*			m_apFrames;		// Need AddRef
	unsigned short		m_wFrameNum;
	unsigned short		m_wFlyH;		// 飞行怪在当前动画停留高度
	unsigned short		m_wFlySpd;		// 飞行怪在低于停留高度时以该速度回到停留高度
	float				_fChildrenScaleY;
	unsigned char		m_byColorAdd;	// 是否开启饱和叠加
public:
	static void Initialize();
public:
	static XDrawAnimation*			m_oapAni[MAX_ANI_NUM];
	static unsigned short			m_oawCD[MAX_ANI_NUM];
};

class XAnimation :
	public XResObj
{
public:
	XAnimation() {
		m_wDrawAniID = 0xFFFF;
		m_wCurrFrame = 0xFFFF;
		m_wPauseFrame = 0;
		m_byIsEnd = 0xFF;
		m_pFrameInfo = NULL;
		m_byBreak = 0x00;
		m_byHurtBreak = 0x00;
		XMemLog::m_dwNew += sizeof(XAnimation);
	}
	virtual ~XAnimation() {
		XMemLog::m_dwNew -= sizeof(XAnimation);
	}
public:
	virtual void OnLoad();
	virtual void OnRelease();
	virtual void OnDraw(float fPosX, float fPosY, float fScale, cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX);
	virtual void OnDrawEx(float fPosX, float fPosY, float fScaleX, float fScaleY,
		cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP);
public:
	inline void SetDrawAni(unsigned short wID, float fChildrnScaleY) {
		m_wDrawAniID = wID;
		if(0xFFFF != wID && XDrawAnimation::m_oapAni[m_wDrawAniID])
			XDrawAnimation::m_oapAni[m_wDrawAniID]->setChildrenScaleY(fChildrnScaleY);
	}
	inline unsigned long OnFrame() {
		if(0xFE == m_byIsEnd)
			return 1;
		if(0xFFFF == m_wCurrFrame) {
			m_wCurrFrame = 0;
			FrameChange();
			return 0;
		}
		if(m_wPauseFrame) {
			if(0xFFFF != m_wPauseFrame)
				--m_wPauseFrame;
			return 0;
		}
		++m_wCurrFrame;
		if(m_wCurrFrame >= FrameNum())
		{
			m_wCurrFrame = 0;
			m_byIsEnd = 0xFF;
		}
		FrameChange();
		return 1;
	}
	inline void ToEndFrame(bool bNoUpdate = false) {
		unsigned short wFNum = FrameNum();
		if(wFNum)
			m_wCurrFrame = wFNum - 1;
		if(bNoUpdate)
			m_byIsEnd = 0xFE;
	}
	inline void ForceToHeadFrame() {
		m_wCurrFrame = 0;
		m_byBreak = 0x00;
		FrameChange();
	}
public:
	inline bool IsCanBreak()
	{
		return (m_byBreak > 0);
	}
	inline bool IsCanHurtBreak()
	{
		return (m_byHurtBreak > 0);
	}
	inline bool IsValid() {
		return (0xFFFF != m_wDrawAniID);
	}
	inline void Reset() {
		m_wPauseFrame = 0;
		m_wCurrFrame = 0xFFFF;
		m_byIsEnd = 0;
		m_pFrameInfo = NULL;
		m_wDrawFrameOnce = 0xFFFF;
		m_wTexW = 0;
		m_wTexH = 0;
		m_byBreak = 0x00;
		m_byHurtBreak = 0x00;
	}
	inline unsigned short CurrFrame() {
		if(0xFFFF == m_wCurrFrame)
			return 0;
		return m_wCurrFrame;
	}
	inline unsigned short FrameNum() {
		if(0xFFFF == m_wDrawAniID)
			return 0;
		if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
			return 0;
		return XDrawAnimation::m_oapAni[m_wDrawAniID]->m_wFrameNum;
	}
	inline void Pause(unsigned short wFrameNum) {
		if(m_wPauseFrame && 0xFFFF != wFrameNum)
			return;
		m_wPauseFrame = wFrameNum;
	}
	inline bool IsPause() {
		return (m_wPauseFrame ? true : false);
	}
	inline bool IsEnd() {
		if(m_byIsEnd)
			return true;
		return false;
	}
	inline void SetDrawFrameOnce(unsigned short wFrame) {
		if(wFrame >= FrameNum())
			return;
		m_wDrawFrameOnce = wFrame;
	}
	inline void GetFrameSize(unsigned short& wW, unsigned short& wH) {
		wW = m_wTexW;
		wH = m_wTexH;
	}
protected:
	unsigned short	m_wDrawAniID;
	unsigned short	m_wCurrFrame;
	unsigned short	m_wPauseFrame;
	unsigned short	m_wDrawFrameOnce;
	unsigned char	m_byIsEnd;
	unsigned short	m_wTexW;
	unsigned short	m_wTexH;
	unsigned char	m_byBreak;
	unsigned char	m_byHurtBreak;
public:
	inline XFrameInfo* GetFrameInfo() {
		XFrameInfo* pReturn = m_pFrameInfo;
		// 修复Bug:HitStop时m_pFrameInfo一直为空不会继续受创
		if(0 == m_wPauseFrame)
			m_pFrameInfo = NULL;
		return pReturn;
	}
	inline void GetFlyInfo(unsigned short& wFlyH, unsigned short& wFlySpd) {
		wFlyH = 0;
		wFlySpd = 0;
		if(0xFFFF == m_wDrawAniID || 0xFFFF == m_wCurrFrame)
			return;
		if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
			return;
		wFlyH = XDrawAnimation::m_oapAni[m_wDrawAniID]->m_wFlyH;
		wFlySpd = XDrawAnimation::m_oapAni[m_wDrawAniID]->m_wFlySpd;
	}
protected:
	inline void FrameChange() {
		if(0xFFFF == m_wDrawAniID || 0xFFFF == m_wCurrFrame)
			return;
		if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
			return;
		if(m_wCurrFrame >= XDrawAnimation::m_oapAni[m_wDrawAniID]->m_wFrameNum)
			return;
		XDrawAnimation::m_oapAni[m_wDrawAniID]->GetSize(m_wCurrFrame, m_wTexW, m_wTexH);
		if(!XDrawAnimation::m_oapAni[m_wDrawAniID]->m_apFrames)
			return;
		m_pFrameInfo = &XDrawAnimation::m_oapAni[m_wDrawAniID]->m_apFrames[m_wCurrFrame];
		// 设置当前帧开始能否被打断
		if (m_pFrameInfo)
		{
			if (m_pFrameInfo->byBreak)
			{
				m_byBreak = 0xFF;
			}
			m_byHurtBreak = m_pFrameInfo->byHurtBreak;
		}
	}
protected:
	XFrameInfo*			m_pFrameInfo;
};

class XTexInitThread
{
public:
	static void InitAllAnimation();
	static volatile unsigned char _byLoaded;						// 0x00未加载|0x01正在加载|0xFE加载失败|0xFF加载成功
protected:	// Multithreaded
	void Initialize();
	OGDP::XThread<XTexInitThread>	m_stLoadThread;
	unsigned long XThreadLoad(long param, volatile bool& bExit);
};

#endif // __XANIMATION_H__
