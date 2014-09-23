#include "XRole.h"
#include "../XMap/XMap.h"
#include "../CommonFile.h"
#include "../XCamera/XCamera.h"
#include "../XHelper/XTable/XTableHeader.h"

XCPP_DEFINED();

unsigned char XRole::m_obyIZStart = 0x00;
unsigned char XRole::m_obyAIEnabled = 0xFF;
unsigned char XRole::m_obyIZEnd = 0x00;

void XRole::SetAI(unsigned short wAI, bool bUseAI)
{
	XFUNC_START();
	if(65535 == wAI || 65500 == wAI)
		return;
	m_wAI = wAI;

	if (!bUseAI)
	{
		return;
	}
	
	m_pAIT = (XAIT*)XTableFile::stTblAI.GetRowData(wAI, sizeof(XAIT));
	if(!m_pAIT)
	{
		CCLOG("[ERROR]XRole::SetAI %d is not exist!", wAI);
		return;
	}
	if(ePet == m_valTypeB)
		m_byAIState = eAI_Follow;
	XFUNC_END();
}

bool XRole::AICommon()
{
	XFUNC_START();
	if(!Valid())
		return false;
	/*if(eWood == m_valTypeB)
	return false;*/
	if(eAI_Follow == m_byAIState)
		return true;
	if(!m_dwHp && m_valTypeB != XMapObj::eCity)
		return false;
	if(!m_obyAIEnabled)
	{
		m_byCmdTypeB = eCmdB_None;
		Wait();
		m_byAIState = eAI_Wait;
		return false;
	}
	if(ePlayer != m_valTypeB && 0xFF != m_obyIZStart)
		return false;
	if(m_dwMaxHp)
		m_byHPPerc = (unsigned char)((float)m_dwHp / (float)m_dwMaxHp);
	if(eAct_Run == m_byActId || eAct_Walk == m_byActId)
	{
		if((fIsZero(m_fSpdX) && fIsZero(m_fSpdY)) || !m_byIsMT)
		{
			m_byIsMT = 0x00;
			m_byCmdTypeB = eCmdB_None;
			Wait();
		}
	}
	if(m_byCmdTypeA < cCmdA_WaitNoForce)
		return true;
	// 如果当前处于受创，无论当前是什么状态，下一次都为等待
	if(eCmdA_Hurt == m_byCmdTypeA)
	{
		m_pEnemy = NULL;
		m_byAIState = eAI_Wait;
	}
	// 敌人
	if(m_pEnemy)
	{
		// 如果敌人死亡，重新寻找
		if(m_pEnemy->IsDead())
			m_pEnemy = NULL;
	}
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::AIGetUp()
{
	XFUNC_START();
	if(!m_pAIT)
		return;
	// 如果有起身攻击
	if(-1 != m_pAIT->valUpCmd && m_pAIT->valUpOdds)
	{
		// 概率
		unsigned char byOdds = rand() % 100;
		if(byOdds < m_pAIT->valUpOdds)
			SendCmd(eCmdA_Skill, m_pAIT->valUpCmd, 0, 0);
	}
	XFUNC_END();
}

void XRole::AIFlee()
{
	XFUNC_START();
	if(!m_pAIT)
		return;
	if(!m_pEnemy)
		return;
	if(!m_valMap)
		return;
	if(fIsZero(m_pRoleT->valSpdWalk) && fIsZero(m_pRoleT->valSpdRun))
		return;
	float fPosX, fPosY, fPosH;
	m_pEnemy->GetPosition(fPosX, fPosY, fPosH);
	unsigned short wWalkL, wWalkR, wWalkT, wWalkB;
	m_valMap->GetWalkRect(wWalkL, wWalkR, wWalkT, wWalkB);
	// X
	float fDestX = rand() % (SCR_W >> 1);
	if(m_fPosX < fPosX)
		fDestX = m_fPosX - fDestX;
	else
		fDestX = m_fPosX + fDestX;
	if(fDestX < wWalkL)
		fDestX = wWalkL;
	if(fDestX > wWalkR)
		fDestX = wWalkR;
	// Y
	float fDestY = m_fPosY;
	if(wWalkT > wWalkB)
	{
		fDestY = rand() % (wWalkT - wWalkB);
		if(fDestY < wWalkB + 10)
			fDestY = wWalkB + 10;
		if(fDestY > wWalkT - 10)
			fDestY = wWalkT - 10;
	}
	else if(wWalkT < wWalkB)
	{
		fDestY = rand() % (wWalkB - wWalkT);
		if(fDestY < wWalkT + 10)
			fDestY = wWalkT + 10;
		if(fDestY > wWalkB - 10)
			fDestY = wWalkB - 10;
	}
	if(fDestX < m_valMap->m_wWalkL + 110.f)
		fDestX = m_valMap->m_wWalkL + 110.f;
	if(fDestX > m_valMap->m_wWalkR - XMAP_SPACE - 10.f)
		fDestX = m_valMap->m_wWalkR - XMAP_SPACE - 10.f;
	SendCmd(eCmdA_MoveTo, 0, (unsigned short)fDestX, (unsigned short)fDestY);
	XFUNC_END();
}

unsigned char XRole::AILogic_Wait()
{
	XFUNC_START();
	if(!AICommon())
		return eAI_Wait;
	if(!m_valMap)
		return eAI_Wait;
	if (m_valTypeB == XMapObj::ePet)
	{
		return eAI_Follow;
	}
	if (m_valTypeB == XMapObj::eCity)
	{
		// 技能比例
		unsigned short wOdds = rand() % 10000;
		if(wOdds < m_pAIT->valS1Odds)
			// Skill 1 技能1使用概率【上】【万分比，技能概率值和必须小于等于10000，不足10000的部分为普通攻击概率】
			SendCmd(eCmdA_Skill, eSkl_Up, 0, 0);

		return eAI_Skill;
	}
	// HP低于某值出招
	if(m_byHPPerc != m_byHPPercLast)
	{
		bool bAttack = false;
		if(-1 != m_pAIT->valHpCmd1 && m_byHPPerc <= m_pAIT->valHpPer1 && m_pAIT->valHpPer1 < m_byHPPercLast)
		{
			SendCmd(eCmdA_Skill, m_pAIT->valHpCmd1, 0, 0);
			bAttack = true;
		}
		else if(-1 != m_pAIT->valHpCmd2 && m_byHPPerc <= m_pAIT->valHpPer2 && m_pAIT->valHpPer2 < m_byHPPercLast)
		{
			SendCmd(eCmdA_Skill, m_pAIT->valHpCmd2, 0, 0);
			bAttack = true;
		}
		m_byHPPercLast = m_byHPPerc;
		if(bAttack)
			return eAI_Skill;
	}
	// 寻找敌人
	unsigned char byCampID = (XCamp::eCampFriend == m_valCamp ? XCamp::eCampEnemyNpc : XCamp::eCampFriend);
	m_pEnemy = m_valMap->FindCampObj(byCampID);
	if(!m_pEnemy)
		return eAI_Wait;
	return eAI_MoveTo;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}

unsigned char XRole::AILogic_MoveTo()
{
	XFUNC_START();
	if(!AICommon())
		return eAI_Wait;
	if(!m_valMap)
		return eAI_Wait;
	if(!m_pEnemy)
		return eAI_Wait;
	if(!m_pRoleT)
		return eAI_Wait;
	if(fIsZero(m_pRoleT->valSpdWalk) && fIsZero(m_pRoleT->valSpdRun))
		return eAI_Attack;
	// 移动过去，进入攻击阶段
	float fPosX, fPosY, fPosH;
	m_pEnemy->GetPosition(fPosX, fPosY, fPosH);
	float fDisY = fAbs(fPosY - m_fPosY);
	float fTX = 20.f - (float)(rand() % 40);
	float fTY = 10.f - (float)(rand() % 20);
	// 近战或Y在范围内，X优先
	if(0 == m_pAIT->valCheck || fDisY <= m_pRoleT->valAttDisMaxY || fDisY <= 10.f)
	{
		SendCmd(eCmdA_MoveTo, 0, (unsigned short)fPosX + fTX, (unsigned short)fPosY + fTY);
	}
	// 远程，Y不在范围内，先移Y
	else
	{
		SendCmd(eCmdA_MoveTo, 0, (unsigned short)m_fPosX + fTX, (unsigned short)fPosY + fTY);
	}
	return eAI_Attack;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}

unsigned char XRole::AILogic_Attack()
{
	XFUNC_START();
	if(!AICommon())
		return eAI_Attack;
	// 如果敌人不存在，进入等待
	if(!m_pEnemy)
		return eAI_Wait;
	if(!m_pRoleT)
		return eAI_Wait;
	// 判断距离
	float fPosX, fPosY, fPosH;
	m_pEnemy->GetPosition(fPosX, fPosY, fPosH);
	float fDisX = fAbs(fPosX - m_fPosX);
	float fDisY = fAbs(fPosY - m_fPosY);
	// 在有效距离内，进入技能阶段
	if(fDisX <= m_pRoleT->valAttDisMaxX && fDisY <= m_pRoleT->valAttDisMaxY)
	{
		// 停下来
		SendCmd(eCmdA_Wait, eCmdB_None, 0, 0);
		if(!fIsZero(m_pRoleT->valSpdWalk) || !fIsZero(m_pRoleT->valSpdRun))
		{
			// 面向敌人
			if(m_fPosX < fPosX)
				m_valFlipX = false;
			else
				m_valFlipX = true;
		}
		// 技能比例
		unsigned short wOdds = rand() % 10000;
		if(wOdds < m_pAIT->valS1Odds)
			// Skill 1 技能1使用概率【上】【万分比，技能概率值和必须小于等于10000，不足10000的部分为普通攻击概率】
			SendCmd(eCmdA_Skill, eSkl_Up, 0, 0);
		else if(wOdds < m_pAIT->valS1Odds + m_pAIT->valS2Odds)
			// Skill 2 技能2使用概率【上】【万分比，技能概率值和必须小于等于10000，不足10000的部分为普通攻击概率】
			SendCmd(eCmdA_Skill, eSkl_Down, 0, 0);
		else if(wOdds < m_pAIT->valS1Odds + m_pAIT->valS2Odds + m_pAIT->valS3Odds)
			// Skill 3 技能3使用概率【前】【万分比，技能概率值和必须小于等于10000，不足10000的部分为普通攻击概率】
			SendCmd(eCmdA_Skill, eSkl_Front, 0, 0);
		else if(wOdds < m_pAIT->valS1Odds + m_pAIT->valS2Odds + m_pAIT->valS3Odds + m_pAIT->valS4Odds)
			// Skill 4 技能4使用概率【后】【万分比，技能概率值和必须小于等于10000，不足10000的部分为普通攻击概率】
			SendCmd(eCmdA_Skill, eSkl_Back, 0, 0);
		else
			// Normal
			SendCmd(eCmdA_Skill, eSkl_Normal, 0, 0);
		return eAI_Skill;
	}
	// 否则继续向敌人移动
	else
	{
		if(!fIsZero(m_pRoleT->valSpdWalk) || !fIsZero(m_pRoleT->valSpdRun))
			return eAI_MoveTo;
		else
			return eAI_Flee;
	}
	return eAI_Attack;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}

unsigned char XRole::AILogic_Skill()
{
	XFUNC_START();
	if(!AICommon())
		return eAI_Skill;
	// 技能是否完成
	if(ActionEnd())
	{
		if (m_valTypeB == XMapObj::eCity)
		{
			return eAI_Wait;
		}
		
		if(m_pAIT->valFlee)
		{
			m_byFleeNum = rand() % 3;
			if(m_byFleeNum)
			{
				AIFlee();
				// 技能完成，进入逃离阶段
				return eAI_Flee;
			}
			else
				return eAI_Attack;
		}
		else
			return eAI_Attack;
	}
	return eAI_Skill;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}

unsigned char XRole::AILogic_Flee()
{
	XFUNC_START();
	if(!AICommon())
		return eAI_Flee;
	// 按照逃离时间计算的逻辑
	//// 逃离时间是否到达
	//if(m_wStateFrame >= m_pAIT->valFlee)
	//	// 时间到达，进入等待阶段
	//	return eAI_Wait;
	//// 等待？继续逃！
	//if(eAI_Wait == m_byActId)
	//{
	//	if(!m_pEnemy)
	//		return eAI_Wait;
	//	if(!m_valMap)
	//		return eAI_Wait;
	//	AIFlee();
	//}
	// 按照逃离次数计算的逻辑
	if(!m_byFleeNum || !m_pAIT->valFlee)
	{
		m_nWaitTick = 0;
		return eAI_Attack;
	}
	if(eAct_Wait == m_byActId || eAct_WaitInCity == m_byActId)
	{
		if(!m_pEnemy)
			return eAI_Wait;
		if(!m_valMap)
			return eAI_Wait;
		--m_byFleeNum;
		AIFlee();
	}
	return eAI_Flee;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}
unsigned char XRole::AILogic_Follow()
{
	XFUNC_START();
	if (!m_pEnemy)
		return eAI_Follow;
	m_nWaitTick = 0;
	m_byWaitFrame = 0;
	float fPosX, fPosY, fPosH;
	m_pEnemy->GetPosition(fPosX, fPosY, fPosH);
	fPosY += 1.f;
	unsigned char TypeC = GetTypeC();
	if (TypeC == XMapObj::eAtt)
		fPosX += 60.f;
	else
		fPosX -= 60.f;
	float fDiffX = fPosX - m_fPosX;
	float fDiffY = fPosY - m_fPosY;
	m_fPosX += fDiffX * 0.2f;
	m_fPosY += fDiffY * 0.2f;
	if(!fIsZero(fDiffX))
	{
		if(fDiffX > 0.f)
			m_valFlipX = false;
		else
			m_valFlipX = true;
	}
	return eAI_Follow;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}

XRole* XRole::PAI_FindEnemy()
{
	XFUNC_START();
	if(!m_valMap)
		return NULL;
	unsigned char byCampID = (XCamp::eCampFriend == m_valCamp ? XCamp::eCampEnemyNpc : XCamp::eCampFriend);
	return m_valMap->FindCampObj(byCampID, this);
	XFUNC_END_RETURN(XRole*, ptr, NULL);
}
void XRole::PAI_Flee()
{
	XFUNC_START();
	if(!m_valMap)
		return;
	unsigned short wWalkL, wWalkR, wWalkT, wWalkB;
	m_valMap->GetWalkRect(wWalkL, wWalkR, wWalkT, wWalkB);
	unsigned long dwPosYMid = ((wWalkT + wWalkB) >> 1);
	if(m_fPosY <= (float)dwPosYMid)
		MoveTo(m_fPosX, wWalkT - 10, true);
	else
		MoveTo(m_fPosX, wWalkB + 10, true);
	XFUNC_END();
}
bool XRole::PAI_FleeForEff()
{
	XFUNC_START();
	if(!m_byPAIFlee)
	{
		// 检测及躲避飞行道具
		bool bH = m_valMap->WouldHurtObj(this);
		if(bH)
		{
			m_byMarTypeNext = 0xFF;
			if(!SkillAct() && ePAI_Skill != m_byPlayerAI)
			{
				// 逃离
				PAI_Flee();
				m_byPAIFlee = 0xFF;
				m_pEnemy = NULL;
				++m_byPlayerAI;
				if(m_byPlayerAI > ePAI_Skill)
					m_byPlayerAI = ePAI_Skill;
			}
			return true;
		}
	}
	XFUNC_END_RETURN(bool, res, false);
}
extern unsigned char g_byLoadPerc;
void XRole::PAI_Update()
{
	XFUNC_START();
	if(0xFF != g_byLoadPerc)
		return;
	if(!Valid())
		return;
	if(!m_dwHp && m_dwMaxHp)
		return;
	if(m_valTypeB != XMapObj::ePlayer && m_valTypeB != XMapObj::eHero && m_valTypeB != XMapObj::ePet)
		return;
	if(ePAI_None == m_byPlayerAI)
		return;
	if(m_byCmdTypeA >= cCmdA_Active)
	{
		if(m_byActId < eAct_HurtUp)
		{
			m_byIsMT = 0x00;
			m_byCmdTypeB = eCmdB_None;
			Wait(true);
		}
		m_pEnemy = NULL;
		m_byPAIFlee = 0x00;
		return;
	}
	if(!m_valMap)
		return;
	if(m_pEnemy && m_fDPosX > m_valMap->m_wWalkR)
	{
		if(eAct_Wait != m_byActId)
		{
			m_byIsMT = 0x00;
			m_byCmdTypeB = eCmdB_None;
			Wait(true);
		}
		return;
	}
	if(!fIsZero(m_fPosH))
		return;
	if(!m_obyIZStart)
	{
		if(this == m_valMap->GetControl())
			m_obyIZStart = 0xFE;
	}
	if(!m_pEnemy)
	{
		m_pEnemy = PAI_FindEnemy();
		if(!m_pEnemy)
		{
			if(!XRole::m_obyIZEnd)
			{
				if(eAct_Wait == m_byActId)
					MoveTo(m_fPosX + (SCR_W >> 1), m_fPosY, true);
			}
			else if(!m_byPAIFlee && eAct_Wait == m_byActId)
			{
				// 逃离
				PAI_Flee();
				//CCLOG("【PAI_Update】No enemy : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
				//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
				m_byPAIFlee = 0xFF;
			}
			return;
		}
	}
	// 检测及躲避飞行道具
	bool bFlee = PAI_FleeForEff();
	if(eAct_Run == m_byActId || eAct_Walk == m_byActId)
	{
		if((fIsZero(m_fSpdX) && fIsZero(m_fSpdY)) || !m_byIsMT)
		{
			m_byIsMT = 0x00;
			m_byCmdTypeB = eCmdB_None;
			Wait();
			//CCLOG("【PAI_Update】Wait : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
			//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
		}
	}
	if(bFlee)
	{
		if(eAct_Wait == m_byActId)
			PAI_Flee();
		//CCLOG("【PAI_Update】FleeForEff : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
		//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
		return;
	}
	float fPosX, fPosY, fPosH;
	m_pEnemy->GetPosition(fPosX, fPosY, fPosH);
	if(fPosX < (float)m_valMap->m_wWalkL)
	{
		m_pEnemy = NULL;
		return;
	}
	if(fPosX > m_valMap->m_wWalkR)
	{
		if(eAct_Wait == m_byActId && m_valMap->m_wWalkRD == m_valMap->m_wWalkR)
			PAI_Flee();
		return;
	}
	if(eAct_Wait == m_byActId)
	{
		m_byPAIFlee = 0x00;
		MoveTo(fPosX, fPosY, true);
		//CCLOG("【PAI_Update】MoveTo : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
		//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
	}
	if(m_byPAIFlee)
		return;
	if(eAct_Run == m_byActId || eAct_Walk == m_byActId)
	{
		// 检查距离
		float fDisX = DIFF(m_fPosX, fPosX);
		float fDisY = DIFF(m_fPosY, fPosY);
		if(fDisX <= m_pRoleT->valAttDisMaxX && fDisY <= m_pRoleT->valAttDisMaxY)
		{
			m_byCmdTypeB = eCmdB_None;
			Wait();
			if(m_fPosX > fPosX)
				m_valFlipX = 0xFF;
			else
				m_valFlipX = 0x00;
			//CCLOG("【PAI_Update】Wait for attack : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
			//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
		}
		else
			return;
	}
	if(!SkillAct())
	{
		if(m_pEnemy->IsDead() || XRole::eAct_HurtFly <= m_pEnemy->GetActId())
		{
			m_pEnemy = NULL;
			m_byPlayerAI = ePAI_GCR1;
			m_byCmdTypeB = eCmdB_None;
			Wait();
			//CCLOG("【PAI_Update】Wait for enemy hurt : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
			//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
			return;
		}
	}
	switch(m_byPlayerAI)
	{
	case ePAI_GCR1:
	case ePAI_GCR2:
	case ePAI_GCR3:
		{
			if(0xFF == m_byMarTypeNext)
			{
				if(eAct_AttNor_3 > m_byActId)
				{
					// 面向敌人
					if(m_fPosX < fPosX)
						m_valFlipX = false;
					else
						m_valFlipX = true;
					UseSkill(XRole::eSkl_Normal);
					//CCLOG("【PAI_Update】UseSkill eSkl_Normal : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
					//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
				}
				else if(ActionEnd())
				{
					++m_byPlayerAI;
					m_byCmdTypeB = eCmdB_None;
					Wait(true);
					//CCLOG("【PAI_Update】wait for Skill round : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
					//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
				}
			}
		} break;
	case ePAI_Skill:
		{
			// 面向敌人
			if(m_fPosX < fPosX)
				m_valFlipX = false;
			else
				m_valFlipX = true;
			bool bUseSkl = false;
			unsigned char bySt = rand() % 4;
			for (unsigned char byAIDir = 0; byAIDir < 4; ++byAIDir, ++bySt)
			{
				unsigned char byDir = bySt % 4;
				unsigned char byPos = XRole::GetSKillPosByDir(byDir);
				if(GetSkillLvl(byPos) && !GetIsCDing(byPos) && IsCanSkill())
				{
					//CCLOG("【PAI_Update】UseSkill %d : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
					//	byDir + 1, m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
					UseSkill(byDir + 1);
					bUseSkl = true;
					break;
				}
			}
			if(!bUseSkl)
			{
				UseSkill(XRole::eSkl_Normal);
				//CCLOG("【PAI_Update】wait for no Skill : Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f DX:%f DY:%f",
				//	m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY, m_fDPosX, m_fDPosY);
			}
			m_byPlayerAI = ePAI_GCR1;
		} break;
	}
	XFUNC_END();
}
