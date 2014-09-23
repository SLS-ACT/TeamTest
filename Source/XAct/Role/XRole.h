#ifndef __XROLE_H__
#define __XROLE_H__
/*
	角色类
	by 刘潇 @ 2013-04-18
*/

#include "../Common.h"
#include "Animation/XAnimation.h"
#include "../XMap/XMapObj.h"
#include "../XMap/XAIObj.h"
#include "XEffect.h"
#include "../Cocos2dx/XDirector.h"
#include "../XNumEffect/XComboEffect.h"
#include "../Game/XGameState.h"
#include "../XControl/XJSCtrl.h"
#include "../XEvent/XEventManger.h"
#include "../XNet/XProtocol.h"

struct XSkillT;

#define ROLE_TYPE_NUM		(512)
#define ROLE_ACT_NUM		XRole::eAct_All
#define	XPD_SKILL_NUM		(4)
#define ROLE_GHOTSING_MAX	(8)		// 残影的上限
#define ROLE_HURTSHAKE_PIX	(4)		// 受创震动位移
#define ROLE_NAME_LEN		(64)

struct XRoleCmd
{
	enum {			// TA
		eRunTo,		// P1 : PosX | P2 : PosY	[ Used in city ]
		eMove,		// TB : Run or Walk | P1 : ScaleX * 1K | P2 : SpeedY * 1K	[ Used in instance zones ]
		eJump,		// None	[ Used in instance zones ]
		eMar,		// TB : Key	[ Used in instance zones ][0:Normal|1:Up|2:Down|3:Front|4:Back]
		eNum
	};
	unsigned char	byTypeA;	// XRole::eCmdA
	unsigned char	byTypeB;	// XRole::eCmdB
	short			nParam1;	// 参见XRoleCmd的enum注释
	short			nParam2;	// 参见XRoleCmd的enum注释

	inline bool operator==(const XRoleCmd& stCmd) {
		return (byTypeA == stCmd.byTypeA &&
			byTypeB == stCmd.byTypeB &&
			nParam1 == stCmd.nParam1 &&
			nParam2 == stCmd.nParam2);
	}
};

struct XRoleGhosting 
{
	struct XRGFrame {					// 残影帧信息
		unsigned char	byActId;		// 动作ID
		enum {
			// Mark
			eFlipX = 0,					// X轴转向
			eHurt,						// 是否受创残影
			eNoBreak,					// 是否霸体残影
		};
		unsigned char	byMark;			// 标记
		unsigned short	wActFrame;		// 动作帧
		unsigned short	wPosX;			// 位置X
		unsigned short	wPosY;			// 位置Y + 位置H
		unsigned char	byAlpha;		// 透明度
		unsigned char	byAlphaSubPF;	// 每帧透明度减少值
		XRGFrame() : byActId(0xFF) {}
	}					astFrames[ROLE_GHOTSING_MAX];
	unsigned char		byFrameUsedIdx;	// 使用到数组的哪一个Idx
	XRoleGhosting() : byFrameUsedIdx(0xFF) {}
};

class XHeadEffect;
class XRole :
	public XMapObj,					// OnFrame Move OnDraw
	public XAIObj<XRole>,			// AI
	public XMemPool<XRole>			// Memory Pool
{
public:
	XRole() {
		m_valTypeA = XMapObj::eRole;
		m_valTypeB = XMapObj::eTypeB_Role;
		m_valTypeC = XMapObj::eTypeC_Pet;
		Reset();
		XMemLog::m_dwNew += sizeof(XRole);
	}
	virtual ~XRole() {
		XMemLog::m_dwNew -= sizeof(XRole);
	}
	static XRole* Create()
	{
		XRole* pRole = XMP_CREATE(XRole);
		if (pRole)
		{
			return pRole;
		}
		XMP_RELEASE(XRole, pRole);
		return NULL;
	}
	virtual void Reset() {
		m_valFlipX = 0;
		for(int nPos = 0; nPos < ROLE_ACT_NUM; ++nPos)
		{
			m_astAnimation[nPos].SetDrawAni(0xFFFF, 1.f);
			m_astAnimation[nPos].Reset();
		}
		m_pRoleHead = NULL;
		//memset(m_szRoleName, 0, sizeof(char) * 64);
		m_wNameOffSetX = 0;
		m_wNameOffSetY = 0;
		m_byRoleLvl = 0;
		m_wRoleID = 0xFFFF;
		m_pRoleT = NULL;
		m_byActId = 0xFF;
		m_fScale = 1.f;
		m_byActType = eActType_None;
		m_byJumpAttack = 0;
		m_byMapMove = 0;
		m_byCmdTypeA = eCmdA_Wait;
		m_byCmdTypeB = eCmdB_None;
		m_stCmdLast.byTypeA = eCmdA_Wait;
		m_stCmdLast.byTypeB = eCmdB_None;
		m_byMarTypeNext = 0xFF;
		m_byMarAfterRun = 0xFF;
		m_pHurtMove = NULL;
		m_bHurtDir = true;
		m_bHurtSky = false;
		m_byHurtMoveF = 0;
		m_byDeadAlpha = 0xFF;
		m_chHurtShake = 0x00;
		m_valTypeA = XMapObj::eRole;
		m_valTypeB = XMapObj::eTypeB_Role;
		m_valTypeC = XMapObj::eEnabled;
		m_valTypeD = XMapObj::eDeadFinish;
		m_valRendType = XMapObj::eRenderNoraml;
		m_byColorFrame = 0;
		m_byHBLast = 0;
		m_byInvincible = 0;
		m_byMapType = 0;
		m_byDeriveType = 0xFF;
		m_byBornActID = 0xFF;
		m_byDeadActID = 0xFF;
		m_bHitDead = false;
		m_bIsDeriveRole = false;
		m_fDePosX = 0.f;
		m_fDePosY = 0.f;
		m_fDeposH = 0.f;
		m_pDeriveRole = NULL;
		memset(m_achEffMark, 0, sizeof(char) * eEff_Mark);
		ResetAI();
		ResetPAI(0x00);
		ResetAttr();
		RG_Reset();
		m_pRoleEffect = NULL;
		m_wEffectId = 0xFFFF;
		m_pRoleEquipEffect = NULL;
		m_wEquipEffectId = 0xFFFF;
		m_pElfAtt = NULL;
		m_pElfDef = NULL;
		m_wDropLogic = 0xFFFF;
		m_pDropSupply = NULL;
		m_byEnableSee = 0xFF;
	}
	inline bool Valid() {
		if(0xFFFF != m_wRoleID /*&& m_wRoleID < ROLE_TYPE_NUM*/)
		{
			return (NULL != m_pRoleT);
		}
		return false;
	}
	void SetRoleID(unsigned short wRoleID);
	virtual void SetTypeA(unsigned char byTypeA)
	{
		XMapObj::SetTypeA(byTypeA);
	}
	virtual void SetTypeB(unsigned char byTypeB)
	{
		XMapObj::SetTypeB(byTypeB);
	}
	virtual void SetTypeC(unsigned char byTypeC)
	{
		XMapObj::SetTypeC(byTypeC);
	}
	inline void SetPos(float fX, float fY, float fH)
	{
		this->SetPosition(fX, fY, fH);
	}
	virtual unsigned char GetTypeA()
	{
		return XMapObj::GetTypeA();
	}
	virtual unsigned char GetTypeB()
	{
		return XMapObj::GetTypeB();
	}
	virtual unsigned char GetTypeC()
	{
		return XMapObj::GetTypeC();
	}
public:	// Action
	enum{
		eAct_Wait		= 0,				// 等待【无摇杆操作】
		eAct_Walk		= 1,				// 走路【摇杆】
		eAct_Run		= 2,				// 跑步【摇杆】
		eAct_CityAct,						// 【城市加载动作】
		eAct_Jump		= 3,				// 跳跃【跳】
		eAct_AttNor_1	= 4,				// 普攻1【点击】
		eAct_AttNor_2	= 5,				// 普攻2【点击】
		eAct_AttNor_3	= 6,				// 普攻3【点击】
		eAct_AttNor_4	= 7,				// 普攻4【点击】
		eAct_AttRun		= 8,				// 跑攻击【跑+点击】
		eAct_AttJump	= 9,				// 跳攻击【跳+点击】
		eAct_AttSkl_1	= 10,				// 技能1【上划】
		eAct_AttSkl_2	= 11,				// 技能2【下划】
		eAct_AttSkl_3	= 12,				// 技能3【前划】
		eAct_AttSkl_4	= 13,				// 技能4【后划】
		eAct_HurtUp		= 14,				// 上受创
		eAct_HurtDown	= 15,				// 下受创
		eAct_HurtFly	= 16,				// 击飞
		eAct_Down		= 17,				// 躺地
		eAct_GetUp		= 18,				// 倒地起身
		eAct_Dodge		= 19,				// 闪避
		eAct_IZAct,							// 【副本加载动作】
		eAct_WaitInCity	= 20,				// 城市待机
		eAct_CityActExt,					// 【城市加载动作】
		eAct_All = eAct_CityActExt			// 【所有动作标记】
	};
public: //Action
	enum {
		eActType_None = 0,
		eActType_City,
		eActType_All,
		eActType_SelRole,
		eActType_Arena,
		eActType_Num
	};
	bool LoadAction(unsigned char byActType);
	inline void Pause(unsigned short wPauseF) {
		if(m_byActId >= ROLE_ACT_NUM)
			return;
		if(!m_astAnimation[m_byActId].IsValid())
			return;
		m_astAnimation[m_byActId].Pause(wPauseF);
	}
	inline bool IsPause() {
		if(m_byActId >= ROLE_ACT_NUM)
			return false;
		if(!m_astAnimation[m_byActId].IsValid())
			return false;
		return m_astAnimation[m_byActId].IsPause();
	}
	inline bool ActExist(unsigned char byActID) {
		if(byActID >= ROLE_ACT_NUM)
			return false;
		if(!m_astAnimation[m_byActId].IsValid())
			return false;
		return true;
	}
	inline bool ChangeAct(unsigned char byActID, bool bReset = false, unsigned char byType = XRole::eSkl_Num) {
		if(!bReset && byActID == m_byActId)
			return false;
		if(byActID >= ROLE_ACT_NUM)
			return false;
		if(!m_astAnimation[byActID].IsValid())
			return false;
		if(byActID != m_byActId && m_byActId < ROLE_ACT_NUM)
			if(m_astAnimation[m_byActId].IsValid())
				m_astAnimation[m_byActId].Reset();
		m_byActId = byActID;
		if(bReset)
			m_astAnimation[m_byActId].Reset();
		m_astAnimation[m_byActId].ForceToHeadFrame();
		if (byType < XRole::eSkl_Num)
		{
			// 活动副本中动作请求
			if (XJoystick::m_opCtrlRole == this && m_byMapType == XMap::eType_IZ)
			{
				XEventManger::ExecuteEvent(XCommonEvent::EVENTS_ACTION, byType);
			}
		}
		m_byHBLast = 0x00;
		return true;
	}
	inline void ChangeActEx(unsigned char byActID)
	{
		/*ChangeAct(byActID);
		if(SkillAct())
		{
		m_byIsMT = 0x00;
		m_byCmdTypeA = eCmdA_Skill;
		}
		m_byMarTypeNext = 0xFF;*/
		if (byActID >= eAct_AttSkl_1 && byActID <= eAct_AttSkl_4)
		{
			UseSkill(byActID - eAct_AttSkl_1 + 1);
		}
	}
	inline void PlayBornSkill()
	{
		if (m_byBornActID != 0xFF)
		{
			if (GetTypeC() == XMapObj::eUnBorned)
			{
				SetPos(m_fDePosX, m_fDePosY, m_fDeposH);
				SetTypeC(XMapObj::eEnabled);
			}			
			
			ChangeActEx(m_byBornActID);
		}		
	}
	inline unsigned char GetActId() {
		return m_byActId;
	}
	inline unsigned short GetActFrame() {
		if(m_byActId >= ROLE_ACT_NUM)
			return 0;
		if(!m_astAnimation[m_byActId].IsValid())
			return 0;
		return m_astAnimation[m_byActId].CurrFrame();
	}
	inline unsigned short GetActFrameNum(unsigned char byActId = 0xFF) {
		if(0xFF == byActId)
			byActId = m_byActId;
		if(byActId >= ROLE_ACT_NUM)
			return 0;
		if(!m_astAnimation[byActId].IsValid())
			return 0;
		return m_astAnimation[byActId].FrameNum();
	}
	inline bool ActionEnd() {
		if(m_byActId >= ROLE_ACT_NUM)
			return true;
		if(!m_astAnimation[m_byActId].IsValid())
			return true;
		return m_astAnimation[m_byActId].IsEnd();
	}
	inline void ActionFrame() {
		if(m_byActId >= ROLE_ACT_NUM)
			return;
		if(!m_astAnimation[m_byActId].IsValid())
			return;
		m_astAnimation[m_byActId].OnFrame();
	}
	inline void ResetAttr()
	{
		m_byRoleLvl = 1;
		m_dwHp = 0;
		m_dwMaxHp = 0;
		m_dwMp = 0;
		m_dwMaxMp = 0;
		m_dwAttPhy = 0;
		m_dwDefPhy = 0;
		m_dwAttSkl = 0;
		m_dwDefSkl = 0;
		//m_fCritRate = 0.f;
		//m_dwCritRateCoefficient = 1;
		//m_fCritRateMax = 0.f;
		//m_fCritRateMin = 0.f;
		m_fCriMul = 0.f;
		m_wCritLvl = 0;
		m_wSpdScale = 100;
		m_dwFightNum = 0;
		m_wReMp = 0;
		m_byElemType = XElement::eNone;
		memset(m_awElemAttr, 0, sizeof(unsigned  short) * XElement::eNum);

		m_fEndTick = 0.f;
		//memset(m_abySklLvl, 0, sizeof(char) * XPD_SKILL_NUM);
		for (unsigned char byCnt = 0; byCnt < XPD_SKILL_NUM; ++byCnt)
		{
			m_stSklInfo[byCnt].Reset();
		}		

		SetSkillLvl(0, 1);
		SetSkillLvl(1, 1);
		SetSkillLvl(2, 1);
		SetSkillLvl(3, 1);
	}
	inline void SetLevel(unsigned char byRoleLvl)
	{
		m_byRoleLvl = byRoleLvl;
	}
	inline unsigned char GetLevel()
	{
		return m_byRoleLvl;
	}
	inline void SetHp(unsigned long dwHp)
	{
		m_dwHp = dwHp;
		if(m_dwHp > m_dwMaxHp)
			m_dwHp = m_dwMaxHp;
	}
	inline unsigned long GetHp()
	{
		return m_dwHp;
	}
	inline void SetMaxHp(unsigned long dwMaxHp)
	{
		m_dwMaxHp = dwMaxHp;
		if(m_dwHp > m_dwMaxHp)
			m_dwHp = m_dwMaxHp;
	}
	inline unsigned long GetMaxHp()
	{
		return m_dwMaxHp;
	}
	inline void SetMp(unsigned long dwMp)
	{
		m_dwMp = dwMp;
		if(m_dwMp > m_dwMaxMp)
			m_dwMp = m_dwMaxMp;
	}
	inline unsigned long GetMp()
	{
		return m_dwMp;
	}
	inline void SetMaxMp(unsigned long dwMaxMp)
	{
		m_dwMaxMp = dwMaxMp;
		if(m_dwMp > m_dwMaxMp)
			m_dwMp = m_dwMaxMp;
	}
	inline unsigned long GetMaxMp()
	{
		return m_dwMaxMp;
	}
	inline void SetAttPhy(unsigned long dwAttPhy)
	{
		m_dwAttPhy = dwAttPhy;
	}
	inline unsigned long GetAttPhy()
	{
		return m_dwAttPhy;
	}
	inline void SetDefPhy(unsigned long dwDefPhy)
	{
		m_dwDefPhy = dwDefPhy;
	}
	inline unsigned long GetDefPhy()
	{
		return m_dwDefPhy;
	}
	inline void SetAttSkl(unsigned long dwAttSkl)
	{
		m_dwAttSkl = dwAttSkl;
	}
	inline unsigned long GetAttSkl()
	{
		return m_dwAttSkl;
	}
	inline void SetDefSkl(unsigned long dwDefSkl)
	{
		m_dwDefSkl = dwDefSkl;
	}
	inline unsigned long GetDefSkl()
	{
		return m_dwDefSkl;
	}
	inline void SetCritRate(float fCritRate)
	{
		// 【作废】
		//m_fCritRate = fCritRate;
	}
	inline float GetCritRate()
	{
		// 【作废】
		//return m_fCritRate;
		return 0.f;
	}
	inline void SetCritRateCoefficient(unsigned long dwCritRateCoefficient)
	{
		// 【作废】
		//m_dwCritRateCoefficient = dwCritRateCoefficient;
	}
	inline unsigned long GetCritRateCoefficient()
	{
		// 【作废】
		//return m_dwCritRateCoefficient;
		return 0;
	}
	inline void SetCritRateMax(float fCritRateMax)
	{
		// 【作废】
		//m_fCritRateMax = fCritRateMax;
	}
	inline float GetCritRateMax()
	{
		// 【作废】
		//return m_fCritRateMax;
		return 0.f;
	}
	inline void SetCritRateMin(float fCritRateMin)
	{
		// 【作废】
		//m_fCritRateMin = fCritRateMin;
	}
	inline float GetCritRateMin()
	{
		// 【作废】
		//return m_fCritRateMin;
		return 0.f;
	}
	inline void SetCriMul(float fCriMul)
	{
		m_fCriMul = fCriMul;
	}
	inline float GetCriMul()
	{
		return m_fCriMul;
	}
	inline void SetCritLvl(unsigned short wCriMul)
	{
		m_wCritLvl = wCriMul;
	}
	inline unsigned short GetCritLvl()
	{
		return m_wCritLvl;
	}
	inline void SetSkillLvl(unsigned char bySklPos, unsigned char bySklLvl)
	{
		if(bySklPos < XPD_SKILL_NUM)
		{
			m_stSklInfo[bySklPos].bySklLvl = bySklLvl;
		}
	}
	inline void SetReMp(unsigned short wReMp)
	{
		m_wReMp = wReMp;
	}
	inline unsigned short GetReMp()
	{
		return m_wReMp;
	}
	inline void SetElemType(unsigned char byType)
	{
		if(byType < XElement::eNum)
			m_byElemType = byType;
	}
	inline void SetElemVal(unsigned char byType, unsigned short wVal)
	{
		if(byType < XElement::eNum)
			m_awElemAttr[byType] = wVal;
	}
	inline void SetSpdScale(unsigned short wPerc)
	{
		m_wSpdScale = wPerc;
	}
	inline unsigned short GetSpdScale()
	{
		return m_wSpdScale;
	}
	inline unsigned char GetSkillLvl(unsigned char bySklPos)
	{
#if defined(WIN32) && defined(_WINDOWS)
		unsigned char byStates = XGameStateManger::GetCurrState();
#ifdef X_SINGLE
		if (enState_Single == byStates)
		{
			return 1;
		}
#endif // _DEBUG

#endif
		if(bySklPos < XPD_SKILL_NUM)
			return m_stSklInfo[bySklPos].bySklLvl;
		return 0x00;
	}
	inline unsigned char GetIsCDing(unsigned char bySklPos)
	{
#if defined(WIN32) && defined(_WINDOWS)
		unsigned char byStates = XGameStateManger::GetCurrState();
#ifdef X_SINGLE
		if (enState_Single == byStates)
		{
			return false;
		}
#endif
#endif
		if(bySklPos < XPD_SKILL_NUM)
			return m_stSklInfo[bySklPos].byCDing;
		return 0x00;
	}
	inline unsigned short GetCDTimer(unsigned char bySklPos)
	{
#if defined(WIN32) && defined(_WINDOWS)
		unsigned char byStates = XGameStateManger::GetCurrState();
#ifdef X_SINGLE
		if (enState_Single == byStates)
		{
			return 0x00;
		}
#endif
#endif
		if(bySklPos < XPD_SKILL_NUM)
			return m_stSklInfo[bySklPos].wCDTimer;
		return 0x00;
	}
	inline void SetCDStart(unsigned char bySklPos)
	{
#if defined(WIN32) && defined(_WINDOWS)
		unsigned char byStates = XGameStateManger::GetCurrState();
#ifdef X_SINGLE
		if (enState_Single == byStates)
		{
			return;
		}
#endif
#endif
		if(bySklPos < XPD_SKILL_NUM)
		{
			unsigned long dwTick = cocos2d::XDirector::Instance().GetRunTime() * 1000;
			m_stSklInfo[bySklPos].dwCDTickEnd = m_stSklInfo[bySklPos].wCDTimer + dwTick;
			m_stSklInfo[bySklPos].byCDing = 0xFF;
		}
	}

	bool SpendSkillMP(unsigned char bySklPos, bool bIsSpend);

	inline void SetComboTimer(unsigned short wComboTimer)
	{
		XComboEffect::Instance().SetComboTimer(wComboTimer);
	}

	inline int GetComboMaxNum()
	{
		return XComboEffect::Instance().GetComboMaxNum();
	}

	float GetIZFinishTick();

	inline void SetFightNum(unsigned long dwFightNum)
	{
		m_dwFightNum = dwFightNum;
	}
	inline unsigned long GetFightNum()
	{
		return m_dwFightNum;
	}
protected:	// Attr
	unsigned char		m_byRoleLvl;	// 等级
	unsigned long		m_dwHp;			// 生命
	unsigned long		m_dwMaxHp;		// 最大生命
	unsigned long		m_dwMp;			// 能量
	unsigned long		m_dwMaxMp;		// 最大能量
	unsigned long		m_dwAttPhy;		// 物理攻击
	unsigned long		m_dwDefPhy;		// 物理防御
	unsigned long		m_dwAttSkl;		// 技能攻击
	unsigned long		m_dwDefSkl;		// 技能防御
	//float				m_fCritRate;	// 基础暴击率	【策划修正：固定为10%】
	//unsigned long		m_dwCritRateCoefficient; // 暴击系数	【策划修正：固定为0.005%】
	//float				m_fCritRateMax;	// 暴击率上限	【策划修正：固定为30%】
	//float				m_fCritRateMin;	// 暴击率下限	【策划修正：固定为1%】
	float				m_fCriMul;		// 暴击倍数
	unsigned short		m_wCritLvl;		// 暴击等级
	unsigned short		m_wSpdScale;	// 移动速度改变【百分比】

	unsigned char		m_byElemType;	// 武器的元素属性类型
	unsigned short		m_awElemAttr[XElement::eNum];	// 角色最终的元素属性数值
	unsigned short		m_wReMp;		// MP恢复的值
	float				m_fEndTick;
	// 战力
	unsigned long		m_dwFightNum;

	typedef struct _SklInfo
	{
		unsigned char	bySklLvl;
		unsigned char	bySklID;
		unsigned char	byCDing;
		unsigned short	wCDTimer;
		unsigned long	dwCDTickEnd;
		void Reset()
		{
			bySklLvl	= 0;
			bySklID		= 0;
			dwCDTickEnd	= 0;
			wCDTimer	= 0;
			byCDing		= 0x00;
		}

		_SklInfo()
		{
			Reset();
		}
	}SkillInfo;
	SkillInfo			m_stSklInfo[XPD_SKILL_NUM];	// 技能属性
protected:	// Role & Act & Animation
	unsigned short		m_wRoleID;
	XRoleT*				m_pRoleT;
	float				m_fScale;
	unsigned char		m_byActId;
	unsigned char		m_byActType;
	XAnimation			m_astAnimation[ROLE_ACT_NUM]; 
	unsigned char		m_byMapType;
	unsigned char		m_byJumpAttack;
public:
	inline unsigned short GetRoleId()
	{
		return m_wRoleID;
	}
	inline XRoleT*		GetRoleT()
	{
		return m_pRoleT;
	}
	inline void SetRoleScale(float fScale)
	{
		m_fScale = fScale;
	}
	inline float GetRoleScale()
	{
		return m_fScale;
	}
	inline unsigned short GetNameOffSetY()
	{
		return m_wNameOffSetY;
	}
	inline unsigned short GetNameOffSetX()
	{
		return m_wNameOffSetX;
	}
protected:	// Name
	unsigned short		m_wNameOffSetX;
	unsigned short		m_wNameOffSetY;
	XHeadEffect*		m_pRoleHead;
	//char				m_szRoleName[ROLE_NAME_LEN];
public:
	void SetRoleName(const char* szRoleName);
	void SetRoleNameByUnicode(const char* szRoleName);
	inline XHeadEffect* GetRoleHeadEffect()
	{
		return m_pRoleHead;
	}
	inline char* GetRoleName()
	{
		//return m_szRoleName;
		return GetRoleTableName();
	}
	char* GetRoleTableName();
protected:	// Image Retention
	XRoleGhosting		m_stGhosting;
public:
	inline void RG_Reset() {
		memset(&m_stGhosting, 0xFFFFFFFF, sizeof(XRoleGhosting));
	}
	inline void RG_Used(unsigned char byStay, unsigned char byHurtType = 0, unsigned char byAlphaSt = 0xFF) {
		if(!byStay)
			return;
		if(m_stGhosting.byFrameUsedIdx >= ROLE_GHOTSING_MAX)
			m_stGhosting.byFrameUsedIdx = 0;
		XRoleGhosting::XRGFrame& stRGF = m_stGhosting.astFrames[m_stGhosting.byFrameUsedIdx++];
		stRGF.byActId = m_byActId;
		stRGF.wActFrame = m_astAnimation[m_byActId].CurrFrame();
		stRGF.byMark = 0;
		if(m_valFlipX)
			XBIT_MARK(&stRGF.byMark, XRoleGhosting::XRGFrame::eFlipX);
		if(XRoleGhosting::XRGFrame::eHurt == byHurtType)
			XBIT_MARK(&stRGF.byMark, XRoleGhosting::XRGFrame::eHurt);
		if(XRoleGhosting::XRGFrame::eNoBreak == byHurtType)
			XBIT_MARK(&stRGF.byMark, XRoleGhosting::XRGFrame::eNoBreak);
		stRGF.wPosX = (unsigned short)m_fPosX;
		stRGF.wPosY = (unsigned short)(m_fPosY + m_fPosH);
		stRGF.byAlpha = byAlphaSt;
		stRGF.byAlphaSubPF = stRGF.byAlpha / byStay;
		if(!stRGF.byAlphaSubPF)
			stRGF.byAlphaSubPF = 0x01;
	}
	inline void RG_Run();
	inline void RG_OnDraw();
public:	// MapObj
	virtual unsigned long OnFrame();
	virtual void OnDraw();
	virtual void OnDrawSoul();
	virtual void OnDrawShadow();
	virtual bool NeedRemove();
	virtual void OnEnterMap(XMap* pMap);
	virtual void OnLeaveMap(XMap* pMap);
	virtual void Release();		// Release to pool
	virtual void OnLevelUp();
	virtual void OnRevive();
public:
	inline void SetMapMove() {
		if(eAct_Wait == m_byActId)
			ChangeAct(eAct_Walk);
		m_byMapMove = 0xFF;
	}
protected:
	unsigned char		m_byMapMove;	// 被地图推进
public:		// Command
	enum {
		// 主动
		eCmdA_Wait = 0,
		eCmdA_Move,
		eCmdA_MoveTo,
		cCmdA_WaitNoForce,	// 分隔符
		eCmdA_Jump = cCmdA_WaitNoForce,
		eCmdA_Dodge,
		eCmdA_Skill,
		cCmdA_Active,		// 分隔符
		// 被动
		eCmdA_Hurt = cCmdA_Active,
		eCmdA_Dead,
		eCmdA_Disappear,
		eCmdA_Num,

		eCmdB_None = 0,
		eCmdB_Exist,

		eCmdB_Move_Walk = eCmdB_Exist,
		eCmdB_Move_Run,

		eCmdB_Dead_Hide = eCmdB_Exist,
		eCmdB_Dead_Soul,
		eCmdB_Dead_End,
		eCmdB_Relive_Soul,
		eCmdB_Relive_End,
	};
	inline void SendCmd(			// Used in lua
		unsigned char	byTypeA,	// XRole::eCmdA
		unsigned char	byTypeB,	// XRole::eCmdB
		short			nParam1,	// 参见XRoleCmd的enum注释
		short			nParam2		// 参见XRoleCmd的enum注释
		)
	{
		XRoleCmd stCmd;
		stCmd.byTypeA = byTypeA;
		stCmd.byTypeB = byTypeB;
		stCmd.nParam1 = nParam1;
		stCmd.nParam2 = nParam2;
		SendCmd(stCmd);
	}
	virtual void SendCmd(const XRoleCmd& stCmd);
	virtual void RecvCmd(const XRoleCmd& stCmd);
	unsigned char GetCmdTypeA()
	{
		return m_byCmdTypeA;
	}
protected:	// Command
	unsigned char	m_byCmdTypeA;
	unsigned char	m_byCmdTypeB;
	XRoleCmd		m_stCmdLast;
public:		// Attr
	virtual void Wait(bool bForce = false);	// 在主动行为中调用Wait必须强制
	virtual void ForceWait();
	virtual void MoveTo(unsigned short wPosX, unsigned short wPosY, bool bRun);
	virtual void Move(float fScaleX, float fScaleY, bool bRun);
	virtual void Jump();
	virtual void Dodge();
protected:	// Attr
public:		// Attack
	enum {
		eSkl_Normal = 0,
		eSkl_Up,
		eSkl_Down,
		eSkl_Front,
		eSkl_Back,
		eSkl_Num,

	};
	virtual void UseSkill(unsigned char byType);
	virtual void UseSklAfterRunTo(unsigned short wPosX, unsigned short wPosY, unsigned char byType);
	virtual bool SkillAct();
	virtual bool OnHurt(XEffect* pEffect, bool bDir = true);// bDir 表示默认向左
	virtual void OnHit(XEffect* pEffect, XRole* pHurt);
	inline bool IsDead() {
		return (0 == m_dwHp);
	}
	// 复活的接口 : 调用此接口复活时无需调用SetHp
	void Revive();
	// 强制死亡的接口
	void Dead();
	// 强制消失然后在消失后回收的接口
	void Disappear();
	inline bool IsWait(){
		return  (m_byActId == eAct_WaitInCity || m_byActId == eAct_Wait);
	}
	inline bool IsCanSkill()
	{
		if (m_byMapType == XMap::eType_IZ)
		{
			return ((m_byActId == eAct_WaitInCity || m_byActId == eAct_Wait) || m_astAnimation[m_byActId].IsCanBreak());
		}
		else
		{
			return true;
		}
	}
	inline bool IsDeadEnd() {
		return (eCmdA_Dead == m_byCmdTypeA && eCmdB_Dead_End == m_byCmdTypeB);
	}
	//xyb: 设置玩家消失的OffSet
	static void SetDeadAlphaOffSet(unsigned char byOffSet)
	{
		m_byDeadAlphaOffSet = byOffSet;
	}
protected:	// Attack
	unsigned char	m_byMarTypeNext;
	unsigned char	m_byMarAfterRun;
	XSkillT*		m_pHurtMove;		// 击退、击飞
	bool			m_bHurtDir;			// onhurt时候的方向
	bool			m_bHurtSky;			// 受创时候是否在空中
	unsigned char	m_byHurtMoveF;		// 已经执行受创位移的帧数 
	unsigned char	m_byHMFlipX;		// 受创位移朝向 true:左飞|false:右飞 
	unsigned char	m_byDeadAlpha;		// 死亡渐隐
	static unsigned char m_byDeadAlphaOffSet;	// 角色消失Alpha的Offset
	signed char		m_chHurtShake;		// 受创震动
	cocos2d::ccColor3B			m_stColor;		// 受创变色
	unsigned char				m_byColorFrame;	// 受创变色持续时间
	static cocos2d::ccColor3B	m_stNoColor;	// 默认颜色
	unsigned char	m_byHBLast;			// 上一帧是否霸体
	unsigned char	m_byInvincible;		// 无敌持续帧
	static unsigned char m_byIsRevive;	// 是否是复活
	/* modify by lihaofeng on 2014.5.14 */
	unsigned char	m_byDeriveType;		// 死亡派生类型
	unsigned char	m_byBornActID;		// 出生招式ID（0xFF表示没有）
	unsigned char	m_byDeadActID;		// 死亡招式ID（0xFF表示没有）
	bool			m_bHitDead;			// 是否死亡生效
	bool			m_bIsDeriveRole;	// 是否是派生怪物
	XRole*			m_pDeriveRole;		// 死亡后派生的NPC
	float			m_fDePosX;			// 死亡派生绝对坐标X
	float			m_fDePosY;			// 死亡派生绝对坐标Y
	float			m_fDeposH;			// 死亡派生绝对坐标H
// 死亡派生相关接口
public:
	inline void SetDeriveType(unsigned char	byDeriveType)
	{
		m_byDeriveType = byDeriveType;
	}
	inline void SetBornActID(unsigned char	byBornActID)
	{
		m_byBornActID = byBornActID;
	}
	inline unsigned char GetBornActID()
	{
		return m_byBornActID;
	}
	inline void SetDeadActID(unsigned char	byDeadActID)
	{
		m_byDeadActID = byDeadActID;
	}
	inline unsigned char GetDeadActID()
	{
		return m_byDeadActID;
	}
	inline void SetDeriveRole(XRole* pDeriveRole)
	{
		m_pDeriveRole = pDeriveRole;
	}
	inline void SetDerivePos(float fX, float fY, float fH)
	{
		m_fDePosX = fX;
		m_fDePosY = fY;
		m_fDeposH = fH;
	}
	inline void GetDerivePos(float& fX, float& fY, float& fH)
	{
		fX = m_fDePosX;
		fY = m_fDePosY;
		fH = m_fDeposH;
	}
	inline void SetHitDead(bool bHitDead)
	{
		m_bHitDead = bHitDead;
	}
	inline bool GetHitDead()
	{
		return m_bHitDead;
	}
	inline void SetIsDeriveRole(bool bIsDeriveRole)
	{
		m_bIsDeriveRole = bIsDeriveRole;
	}
	inline bool GetIsDeriveRole()
	{
		return m_bIsDeriveRole;
	}
protected:	// Effect
	friend class XEffect;
	typedef XSinglyLink<XEffect> XEffectList;
	XEffectList		m_stEffect;
	enum {
		eEff_Relive		= 0,
		eEff_LvlUp,
		eEff_Break,						// 是否具备霸体特效
		eEff_Supply_HP,					// 补给特效_HP
		eEff_Supply_MP,					// 补给特效_MP

		eEff_Num,
		eEff_Mark = ((eEff_Num + 7) >> 3)
	};
	char			m_achEffMark[eEff_Mark];
public:		// AI
	void SetAI(unsigned short wAI, bool bUseAI = true);
	unsigned short GetAIID()
	{
		return m_wAI;
	}

	inline void AIEnabled(bool bEnabled)
	{
		if(bEnabled)
			m_obyAIEnabled = 0xFF;
		else
			m_obyAIEnabled = 0x00;
	}
	inline void SetAITarget(XRole* pTarget)
	{
		m_pEnemy = pTarget;
		if (GetTypeB() == XMapObj::ePet)
		{
			if (pTarget)
			{
				if (GetTypeC() == XMapObj::eAtt)
				{
					pTarget->m_pElfAtt = this;
				}				
				else
				{
					pTarget->m_pElfDef = this;
				}
			}			
		}
	}
	inline void SetRoleElf(unsigned char byType, XRole* pElf)
	{
		if (byType == XMapObj::eAtt)
		{
			m_pElfAtt = pElf;
		}
		else if (byType == XMapObj::eDef)
		{
			m_pElfDef = pElf;
		}		
	}
	inline bool UsedAI() {
		return (NULL != m_pAIT);
	}
	inline void SetWaitTick(unsigned int nTick)
	{
		m_nWaitTick = nTick;
	}
private:	// AI
	unsigned int	m_nWaitTick;
	unsigned char	m_byFleeNum;
	unsigned short	m_wAI;
public:		// Battle AI
	static unsigned char	m_obyIZStart;
	static unsigned char	m_obyAIEnabled;
	static unsigned char	m_obyIZEnd;
	XRole*  m_pElfAtt;	// 攻击精灵
	XRole*	m_pElfDef;	// 防御精灵
protected:	// AI
	enum {
		eAI_Wait = 0,
		eAI_MoveTo,
		eAI_Attack,
		eAI_Skill,
		eAI_Flee,
		eAI_Follow,
	};
	XRole*	m_pEnemy;
	bool AICommon();
	void AIGetUp();
	void AIFlee();
	unsigned char AILogic_Wait();
	unsigned char AILogic_MoveTo();
	unsigned char AILogic_Attack();
	unsigned char AILogic_Skill();
	unsigned char AILogic_Flee();
	unsigned char AILogic_Follow();
	inline void ResetAI() {
		ResetAIObj();
		m_pEnemy = NULL;
		m_afnAIState[eAI_Wait]		= &XRole::AILogic_Wait;
		m_afnAIState[eAI_MoveTo]	= &XRole::AILogic_MoveTo;
		m_afnAIState[eAI_Attack]	= &XRole::AILogic_Attack;
		m_afnAIState[eAI_Skill]		= &XRole::AILogic_Skill;
		m_afnAIState[eAI_Flee]		= &XRole::AILogic_Flee;
		m_afnAIState[eAI_Follow]	= &XRole::AILogic_Follow;
		m_nWaitTick = 0;
		m_byFleeNum = 0;
		m_wAI = 65535;
	}
protected:	// Player AI
	enum {
		ePAI_None = 0,
		ePAI_GCR1,
		ePAI_GCR2,
		ePAI_GCR3,
		ePAI_Skill,
	};
	unsigned char	m_byPlayerAI;
	unsigned char	m_byPAIFlee;
public:
	inline void UsePlayerAI() {
		m_byPlayerAI = ePAI_GCR1;
	}
	inline void ResetPAI(unsigned char byCheckDead = 0xFF) {
		m_byPlayerAI = ePAI_None;
		m_byPAIFlee = 0x00;
		m_pEnemy = NULL;
		if(!IsDead() || !byCheckDead)		// xyb:玩家死亡时候调用，会站起来
		{
			XRoleCmd stCmd;
			stCmd.byTypeA = eCmdA_Wait;
			stCmd.byTypeB = eCmdB_None;
			RecvCmd(stCmd);
		}
	}
	inline bool IsUsePAI() {
		return (ePAI_None != m_byPlayerAI);
	}
	XRole* PAI_FindEnemy();
	void PAI_Flee();
	bool PAI_FleeForEff();
	void PAI_Update();
public:	// Memory Pool
	static const unsigned short _XMP_Alloc_Num = 16;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo);
	virtual void Cleanup();
	virtual bool OnCreate();
	virtual void OnRelease();
public:	// Role Action
	static void Initialize();
public:
	struct XRoleInfo 
	{
		XRoleT*		pRoleT;

		struct XActInfo {
			unsigned short	wAniId;
			short			nRcX;
			short			nRcY;
			short			nRcW;
			short			nRcH;
			unsigned char	byThickness;
			unsigned char	byType;
			XActInfo() {
				wAniId = 0xFFFF;
				nRcX = 0;
				nRcY = 0;
				nRcW = 0;
				nRcH = 0;
				byThickness = 0;
				byType = 0xFF;
			}
		};
		XActInfo	astActInfo[ROLE_ACT_NUM];

		XRoleInfo() {
			pRoleT = NULL;
			//memset(astActInfo, 0, sizeof(XActInfo) * ROLE_ACT_NUM);
			XMemLog::m_dwNew += sizeof(XRoleInfo);
		}
		~XRoleInfo() {
			XMemLog::m_dwNew -= sizeof(XRoleInfo);
		}
	};
	static XRoleInfo	m_oastRoleInfo[ROLE_TYPE_NUM];
	static unsigned char m_obySkillUse[XPD_SKILL_NUM];
	static cocos2d::XSprite* m_pShadowSprite;
public:
	static unsigned char GetSKillPosByDir(unsigned char byDir)
	{
		if (byDir >= 0 && byDir <= XPD_SKILL_NUM)
		{
			return m_obySkillUse[byDir];
		}
		
		return 0xFF;
	}
	static void setSKillPosByDir(unsigned char byDir, unsigned char byPos)
	{
		if (byDir >= 0 && byDir < XPD_SKILL_NUM)
		{
			m_obySkillUse[byDir] = byPos;
		}
	}
public:
	static bool InitializeShodow(const char* szShadowPath);
public:	// UIRole
	virtual unsigned long OnUIRoleFrame(bool bUseSkill = false);
	virtual void OnUIRoleDraw(float fX, float fY, float fScale = 1.f, unsigned char byAlpha  = 0xFF );
public:
	void DelRoleEffect();
	void SetEffectId(unsigned short wEffectId);

	void DelRoleEquipEffect();
	void SetEquipEffectId(unsigned short wEffectId);

private:
	//unsigned char m_byEffectType;
	unsigned short m_wEffectId;
	XEffect*	m_pRoleEffect;

	// 玩家套装特效
	unsigned short m_wEquipEffectId;
	XEffect*	m_pRoleEquipEffect;

//xyb: 掉落的使用逻辑，为了当做陷阱简化逻辑，所以当做掉落角色处理
public:
	unsigned short GetDropLogic()
	{
		return m_wDropLogic;
	}
	void SetDropLogic(unsigned short wDropLogic)
	{
		m_wDropLogic = wDropLogic;
	}
	void SetDropSupply(XRole* pDropSupply)
	{
		m_pDropSupply = pDropSupply;
	}
	XRole* GetDropSupply()
	{
		return m_pDropSupply;
	}
private:
	unsigned short	m_wDropLogic;		// 补给类型
	XRole*			m_pDropSupply;		// 死亡后掉落的补给
public:
	virtual void SetEnableSee(unsigned char byEnableSee)
	{
		m_byEnableSee = byEnableSee;
		if(m_pElfAtt)
		{
			m_pElfAtt->SetEnableSee(byEnableSee);
		}
		if(m_pElfDef)
		{
			m_pElfDef->SetEnableSee(byEnableSee);
		}
	}
	virtual unsigned char GetEnableSee()
	{
		return m_byEnableSee;
	}
private:
	unsigned char	m_byEnableSee;	// 是否可点击
};

#endif // __XROLE_H__
