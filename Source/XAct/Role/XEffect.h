#ifndef __XEFFECT_H__
#define __XEFFECT_H__
/*
	技能特效类
	by 刘潇 @ 2013-07-25
*/

#include "../Common.h"
#include "Animation/XAnimation.h"
#include "../XMap/XMapObj.h"
#include "../XHelper/XTable/XTableHeader.h"

#define EFFECT_TYPE_NUM	(4096)

class XRole;

// Skill 和 Effect 在这个项目里是一码事，只是一个逻辑一个渲染罢了
class XEffect :
	public XMapObj,					// OnFrame Move OnDraw
	public XMemPool<XEffect>		// Memory Pool
{
public:
	XEffect*	_pNext;
public:
	XEffect() {
		m_valTypeA = XMapObj::eEffect;
		m_valTypeB = XMapObj::eSkill;
		Reset();
		XMemLog::m_dwNew += sizeof(XEffect);
	}
	virtual ~XEffect() {
		XMemLog::m_dwNew -= sizeof(XEffect);
	}
	virtual void Reset() {
		m_valFlipX = 0;
		m_valRendType = XMapObj::eRenderNoraml;
		_pNext = NULL;
		m_stAnimation.SetDrawAni(0xFFFF, 1.f);
		m_stAnimation.Reset();
		m_wSkillID = 0xFFFF;
		m_bySkillPos = 0xFF;
		m_wHitNum = 0;
		m_fScale = 1.f;
		m_pOwner = NULL;
		m_stColor.r = 0xFF;
		m_stColor.g = 0xFF;
		m_stColor.b = 0xFF;
		m_byCover = 0x00;
	}
	inline bool Valid() {
		return (0xFFFF != m_wSkillID && m_wSkillID < EFFECT_TYPE_NUM);
	}
	inline bool CoverRole() {
		return (0xFF != m_byCover && 2 != m_byCover && 3 != m_byCover);
	}
	inline XRole* GetOwner() {
		return m_pOwner;
	}
	inline void SetSkillPos(unsigned char bySklPos) {
		m_bySkillPos = bySklPos;
	}
	inline unsigned char GetSkillPos() {
		return m_bySkillPos;
	}
	// 返回该特效是否绑定角色
	bool SetSkillID(unsigned short wSkillID, XRole* pOwner);	// AddRef
	// 设置颜色
	void SetColor(unsigned char byR, unsigned char byG, unsigned char byB)
	{
		m_stColor.r = byR;
		m_stColor.g = byG;
		m_stColor.b = byB;
	}
public: // Animation
	inline void Pause(unsigned short wPauseF) {
		if(!m_stAnimation.IsValid())
			return;
		m_stAnimation.Pause(wPauseF);
	}
	inline unsigned short GetFrame() {
		if(!m_stAnimation.IsValid())
			return 0;
		return m_stAnimation.CurrFrame();
	}
	inline unsigned short GetFrameNum() {
		if(!m_stAnimation.IsValid())
			return 0;
		return m_stAnimation.FrameNum();
	}
	inline bool AnimateEnd() {
		if(!m_stAnimation.IsValid())
			return true;
		return m_stAnimation.IsEnd();
	}
	inline void AnimateFrame() {
		if(!m_stAnimation.IsValid())
			return;
		m_stAnimation.OnFrame();
	}
protected:
	unsigned short		m_wSkillID;
	float				m_fScale;
	unsigned char		m_bySkillPos;
	unsigned short		m_wHitNum;
	XRole*				m_pOwner;
	unsigned char		m_byCover;		// 是否盖住角色
	XAnimation			m_stAnimation;
	cocos2d::ccColor3B	m_stColor;
public:	// MapObj
	virtual unsigned long OnFrame();
	virtual void OnDraw();
	virtual bool NeedRemove();
	virtual void OnEnterMap(XMap* pMap);
	virtual void OnLeaveMap(XMap* pMap);
	virtual void Release();		// Release to pool
public:	// Attack
	virtual void OnHit(XRole* pHurt);
public:	// Memory Pool
	static const unsigned short _XMP_Alloc_Num = 16;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo);
	virtual bool OnCreate();
	virtual void Cleanup();
	virtual void OnRelease();
public:	// Effect Animation
	static void Initialize();
public:
	struct XEffInfo {
		unsigned short	wAniId;
		XSkillT*		pSkillT;
		unsigned char	byBind;
		short			nOffsetX;
		short			nOffsetY;
		short			nOffsetH;
		short			nMaxHit;
		unsigned char	byRenderType;
		XEffInfo() {
			wAniId = 0xFFFF;
			pSkillT = NULL;
			byBind = 0;
			nOffsetX = 0;
			nOffsetY = 0;
			nOffsetH = 0;
			nMaxHit = -1;
			byRenderType = 0;
		}
	};
	static XEffInfo	m_oastEffInfo[EFFECT_TYPE_NUM];
public:
	inline XEffInfo* GetInfo() {
		if(Valid())
			return &m_oastEffInfo[m_wSkillID];
		return NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	// xyb:界面特效临时逻辑 有待完善
public:
	inline bool IsEnd()
	{
		return m_stAnimation.IsEnd();
	}
	void SetDrawAni(unsigned short wAniId);
	void StopEffect();
	static XEffect* CreateGameEffect(float fPosX, float fPosY, unsigned short wEffectId);
	void DelGameEffect();
	void OnGameEffectDraw(float fScale = 1.f, unsigned char byAlpha  = 0xFF);
	void OnGameEffectDraw(float fX, float fY, float fScale = 1.f, unsigned char byAlpha  = 0xFF);
};

#endif // __XEFFECT_H__
