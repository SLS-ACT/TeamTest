#include "XRole.h"
#include "../XMap/XIZMap.h"
#include "../CommonFile.h"
#include "../XCamera/XCamera.h"
#include "../XUI/Control/XHeadEffect.h"
#include "../XHelper/XTable/XTableHeader.h"
#include "../XNumEffect/XNumEffect.h"
#include "../XConfig.h"
#include "../XSound/XSoundManager.h"

XCPP_DEFINED();

XRole::XRoleInfo XRole::m_oastRoleInfo[ROLE_TYPE_NUM];
unsigned char XRole::m_obySkillUse[XPD_SKILL_NUM];
cocos2d::XSprite* XRole::m_pShadowSprite = NULL;
cocos2d::ccColor3B XRole::m_stNoColor = cocos2d::ccc3(0xFF, 0xFF, 0xFF);

unsigned char XRole::m_byDeadAlphaOffSet = 4;
unsigned char XRole::m_byIsRevive = 0x00;

void XRole::SetRoleID(unsigned short wRoleID) {
	XFUNC_START();
	//if(wRoleID >= ROLE_TYPE_NUM)
	//	return;
	m_wRoleID = wRoleID;
	m_pRoleT = (XRoleT*)XTableFile::stTblRole.GetRowData(wRoleID, sizeof(XRoleT));
	unsigned short wRoleAniId = wRoleID;
	if(m_pRoleT)
	{
		wRoleAniId = m_pRoleT->valAniId;
		SetRoleScale(m_pRoleT->valRoleScale*0.01f);
	}
	if(0xFFFF == wRoleAniId)
		wRoleAniId = wRoleID;
	if(wRoleAniId >= ROLE_TYPE_NUM)
	{
		CCLOG("[XRole::SetRoleID] XRole.xls id[%d] used valAniId[%d] is invalid!", wRoleID, wRoleAniId);
		return;
	}
	for(int nPos = 0; nPos < ROLE_ACT_NUM; ++nPos)
	{
		unsigned short wAniID = m_oastRoleInfo[wRoleAniId].astActInfo[nPos].wAniId;
		m_astAnimation[nPos].SetDrawAni(wAniID, 1.f);

		// 设置CD
		if (nPos >= eAct_AttSkl_1 && nPos <= eAct_AttSkl_4)
		{
			m_stSklInfo[nPos - eAct_AttSkl_1].wCDTimer  = XDrawAnimation::m_oawCD[wAniID];
		}		
	}
	XFUNC_END();
}

bool XRole::LoadAction(unsigned char byActType)
{
	XFUNC_START();
	if(!Valid())
		return false;
	if(byActType >= eActType_Num)
		return false;
	if(m_byActType == byActType)
		return true;
	switch(byActType)
	{
	case eActType_None: break;
	case eActType_City:
		{
			// 加载城市动作
			for(int nPos = eAct_Wait; nPos < eAct_CityAct; ++nPos)
				m_astAnimation[nPos].AddRef();
			for(int nPos = eAct_WaitInCity; nPos < eAct_CityActExt; ++nPos)
				m_astAnimation[nPos].AddRef();
			// 城市NPC的小动作
			if(eCity == m_valTypeB)
				m_astAnimation[10].AddRef();
		} break;
	case eActType_All:
		{
			// 加载所有动作
			for(int nPos = eAct_Wait; nPos < eAct_All; ++nPos)
				m_astAnimation[nPos].AddRef();
		} break;
	case eActType_SelRole:
		{
			m_astAnimation[eAct_Wait].AddRef();
			for(int nPos = eAct_AttNor_1; nPos < eAct_AttNor_4; ++nPos)
				m_astAnimation[nPos].AddRef();
		} break;
	case eActType_Arena:
		{
			m_astAnimation[eAct_WaitInCity].AddRef();
		} break;
	}
	switch(m_byActType)
	{
	case eActType_None: break;
	case eActType_City:
		{
			for(int nPos = eAct_Wait; nPos < eAct_CityAct; ++nPos)
				m_astAnimation[nPos].DelRef();
			for(int nPos = eAct_WaitInCity; nPos < eAct_CityActExt; ++nPos)
				m_astAnimation[nPos].DelRef();
			if(eCity == m_valTypeB)
				m_astAnimation[10].DelRef();
		} break;
	case eActType_All:
		{
			for(int nPos = eAct_Wait; nPos < eAct_All; ++nPos)
				m_astAnimation[nPos].DelRef();
		} break;
	case eActType_SelRole:
		{
			m_astAnimation[eAct_Wait].DelRef();
			for(int nPos = eAct_AttNor_1; nPos < eAct_AttNor_4; ++nPos)
				m_astAnimation[nPos].DelRef();
		} break;
	case eActType_Arena:
		{
			m_astAnimation[eAct_WaitInCity].DelRef();
		} break;
	}
	m_byActType = byActType;
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

unsigned long XRole::OnFrame()
{
	XFUNC_START();
	unsigned long dwRun = 0;
	if(!Valid())
		return dwRun;
	//if(m_valMap && this == m_valMap->GetControl())
	//{
	//	CCLOG("【OnFrame】Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
	//		m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY);
	//}
	if(XMapObj::eDisabled == m_valTypeC || XMapObj::eUnDerive == m_valTypeC || XMapObj::eUnBorned == m_valTypeC)
	{
		if (XMapObj::ePet != m_valTypeB)
		{
			return dwRun;
		}
	}
	bool bIsMe = false;
	if(XMapObj::m_valMap)
	{
		if(this == XMapObj::m_valMap->GetControl())
			bIsMe = true;
	}
	// CD
	for (unsigned char byCnt = 0; byCnt < XPD_SKILL_NUM; ++byCnt)
	{
		if (m_stSklInfo[byCnt].byCDing)
		{
			float fTick = XDirector::Instance().GetRunTime() * 1000;
			if (fTick > m_stSklInfo[byCnt].dwCDTickEnd)
			{
				m_stSklInfo[byCnt].byCDing = 0x00;
			}			
		}		
	}
	if (XMap::eType_IZ == m_byMapType && m_dwMp < m_dwMaxMp)
	{
		unsigned long dwTick = XConfig::Instance().GetTickCount();
		if (dwTick >= m_fEndTick)
		{
			m_fEndTick = dwTick + 1000;
			if(m_dwMp < m_dwMaxMp)
			{
				m_dwMp += (3 + m_wReMp);		// 天赋技能影响能量恢复
				if (m_dwMp > m_dwMaxMp)
				{
					m_dwMp = m_dwMaxMp;
				}
				// 刷新MP
				XEventManger::ExecuteEvent(XCommonEvent::MP_CHANGE, 0);
			}
		}
	}
	// 无敌时间
	if(m_byInvincible)
		--m_byInvincible;
	bool bPause = IsPause();
	// 动作
	unsigned char byDoSlowFrame = 0xFF;
	if(eCmdA_Dead != m_byCmdTypeA)
	{
		if(XMapObj::m_bySlow)
		{
			static unsigned long dwSlowF = 1;
			if(!fIsZero(XMapObj::m_fSlowVal))
				dwSlowF = (unsigned long)(1.f / XMapObj::m_fSlowVal);
			if(XMapObj::m_bySlow % dwSlowF)
				byDoSlowFrame = 0;
			else
				ActionFrame();
		}
		else
			ActionFrame();
	}
	if(eCmdA_Disappear == m_byCmdTypeA)
	{
		if(m_byDeadAlpha >= XRole::m_byDeadAlphaOffSet)
			m_byDeadAlpha -= XRole::m_byDeadAlphaOffSet;
		else
		{
			m_byDeadAlpha = 0;
			m_byCmdTypeA = eCmdA_Dead;
			m_byCmdTypeB = eCmdB_Dead_End;
			// 玩家消失也走的这里，但是不知道是切换副本删除还是玩家下线，所以下线时把玩家类型变成NPC以便于删除
			m_valTypeB = eCity;
			m_valTypeC = eEnabled;
			// 临时添加角色消失事件
			if(ePlayer != m_valTypeB && ePet != m_valTypeB && eHero != m_valTypeB && 2 != XRole::m_byDeadAlphaOffSet)
				XEventManger::ExecuteEvent(XCommonEvent::ROLE_DISAPPEAR, 0);
		}
		return dwRun;
	}
	// 震动
	if(m_chHurtShake)
	{
		if(m_chHurtShake > 0)
			--m_chHurtShake;
		else
			++m_chHurtShake;
		m_chHurtShake = -m_chHurtShake;
	}
	unsigned char byChangeSklAct = 0x00;
	if(!bPause)
	{
		// 如果当前状态为等待，速度设为0
		if(eAct_Wait == m_byActId)
		{
			m_pHurtMove = NULL;
			m_bHurtDir = true;
			m_bHurtSky = false;
			Stop();
		}
		// AI
		if(0 == m_nWaitTick)
		{
			UpdateAI(this);
		}
		else
		{
			--m_nWaitTick;
		}
		// Player AI
		PAI_Update();
		// 主动动作可以打断
		if(m_byCmdTypeA < cCmdA_Active)
		{
			// 是否有技能
			if(0xFF != m_byMarTypeNext)
			{
				// 技能才允许开启打断规则 : 技能打断技能|技能打断GC
				bool bBreak = false;
				if(eSkl_Up <= m_byMarTypeNext && m_byMarTypeNext <= eSkl_Back)
				{
					if(m_astAnimation[m_byActId].IsCanBreak())
						bBreak = true;
				}
				// 动作完成或者不是技能动作时
				if(ActionEnd() || !SkillAct() || bBreak)
				{
					// GC | Run | Jump
					if(eSkl_Normal == m_byMarTypeNext)
					{
						switch(m_byActId)
						{
						//case eAct_Run:		ChangeAct(eAct_AttRun, true); break;
						case eAct_AttJump:
						case eAct_Jump:
							{
								if(fIsZero(m_fPosH))
								{
									ChangeAct(eAct_AttNor_1, true);
									byChangeSklAct = 0xFF;
								}
								else
								{
									if(!m_byJumpAttack)
									{
										m_byJumpAttack = 0xFF;
										ChangeAct(eAct_AttJump, true);
										byChangeSklAct = 0xFF;
									}
								}
							}break;
						case eAct_AttNor_1:	{ ChangeAct(eAct_AttNor_2, true, XRole::eSkl_Normal); byChangeSklAct = 0xFF; } break;
						case eAct_AttNor_2:	{ ChangeAct(eAct_AttNor_3, true, XRole::eSkl_Normal); byChangeSklAct = 0xFF; } break;
						case eAct_AttNor_3:	{ ChangeAct(eAct_AttNor_4, true, XRole::eSkl_Normal); byChangeSklAct = 0xFF; } break;
						default:			{ ChangeAct(eAct_AttNor_1, true, XRole::eSkl_Normal); byChangeSklAct = 0xFF; } break;
						}
					}
					// Skill
					else
					{
						// 跳起来的时候，不能出技能。
						if (m_byActId != eAct_Jump)
						{
							unsigned char byDir = m_byMarTypeNext - 1;
							if (byDir >= 4)
							{
								byDir = 0;
							}
							unsigned char byPos = 0;
							bool bFlag = true;
							if (bIsMe)
							{
								byPos = XRole::GetSKillPosByDir(byDir);
								bFlag = GetSkillLvl(byPos) && !GetIsCDing(byPos) && IsCanSkill();
							}
							else
							{
								byPos = m_byMarTypeNext - 1;
							}
							switch(m_byMarTypeNext)
							{
							case eSkl_Up:		if(bFlag){ byChangeSklAct = 0xFF; ChangeAct(eAct_AttSkl_1 + byPos, true, XRole::eSkl_Up); if(bIsMe){SetCDStart(byPos); SpendSkillMP(byPos,true);}} break;
							case eSkl_Down:		if(bFlag){ byChangeSklAct = 0xFF; ChangeAct(eAct_AttSkl_1 + byPos, true, XRole::eSkl_Down); if(bIsMe){SetCDStart(byPos); SpendSkillMP(byPos,true);}} break;
							case eSkl_Front:	if(bFlag){ byChangeSklAct = 0xFF; ChangeAct(eAct_AttSkl_1 + byPos, true, XRole::eSkl_Front); if(bIsMe){SetCDStart(byPos); SpendSkillMP(byPos,true);}} break;
							case eSkl_Back:		if(bFlag){ byChangeSklAct = 0xFF; ChangeAct(eAct_AttSkl_1 + byPos, true, XRole::eSkl_Back); if(bIsMe){SetCDStart(byPos); SpendSkillMP(byPos,true);}} break;
							}
						}
					}
					// Cmd : 如果动作改变成功
					if(SkillAct())
					{
						m_byIsMT = 0x00;
						m_byCmdTypeA = eCmdA_Skill;
					}
					m_byMarTypeNext = 0xFF;
				}
			}
			else
			{
				// 动作完成且为技能动作时
				if(ActionEnd() && SkillAct())
				{
					// 死亡招式播放完毕，走派生逻辑
					if (!m_dwHp && m_dwMaxHp && m_byDeadActID != 0xFF)
					{
						if (m_byDeriveType != 0xFF && m_pDeriveRole)
						{
							// 如果是坐标派生
							if (!m_byDeriveType)
							{
								m_pDeriveRole->SetPos(GetPosX(), GetPosY(), GetPosH());
							}
							else
							{
								m_pDeriveRole->SetPos(m_fDePosX, m_fDePosY, m_fDeposH);
							}
							
							m_pDeriveRole->SetTypeC(XMapObj::eEnabled);
							unsigned char byBornActID = m_pDeriveRole->GetBornActID();
							if (byBornActID != 0xFF)
							{
								m_pDeriveRole->ChangeActEx(byBornActID);
							}
							else
							{
								m_pDeriveRole->Wait(true);
							}
						}	
						// 设置死亡
						m_byCmdTypeA = eCmdA_Dead;
						m_byCmdTypeB = eCmdB_Dead_End;
					}
					else
					{
						m_byCmdTypeB = eCmdB_None;
						Wait(true);
						// 出招结束后还原之前的摇杆操作
						if(eCmdA_Wait == m_byCmdTypeA && eCmdA_Wait != m_stCmdLast.byTypeA && eCmdA_Skill != m_stCmdLast.byTypeA)
						{
							XRoleCmd stCmd = m_stCmdLast;
							m_stCmdLast.byTypeA = eCmdA_Move;
							m_stCmdLast.byTypeB = eCmdB_Move_Run;
							RecvCmd(stCmd);
						}
					}
				}
				// 落地时
				if(eCmdA_Jump == m_byCmdTypeA)
				{
					m_byCmdTypeB = eCmdB_None;
					Wait(true);
				}
				// 到达目的地时
				if(eCmdA_Move == m_byCmdTypeA || eCmdA_MoveTo == m_byCmdTypeA)
				{
					if(fIsZero(m_fSpdX) && fIsZero(m_fSpdY))
					{
						m_byCmdTypeB = eCmdB_None;
						Wait();
					}
				}
			}
		}
		// 被动动作
		else
		{
			// 击飞动作，落地后接躺地
			if(eAct_HurtFly == m_byActId)
			{
				if(fIsZero(m_fPosH) && fIsZero(m_fSpdH))
				{
					Stop();
					if(!m_dwHp && m_dwMaxHp && (m_byDeadActID != 0xFF || m_pDeriveRole))
					{
						// 播放死亡招式
						if (m_byDeadActID != 0xFF)
						{
							ChangeAct(m_byDeadActID);
							if(SkillAct())
							{
								m_byIsMT = 0x00;
								m_byCmdTypeA = eCmdA_Skill;
							}
							m_byMarTypeNext = 0xFF;
						}
						else if (m_pDeriveRole)
						{
							// 如果是坐标派生
							if (!m_byDeriveType)
							{
								m_pDeriveRole->SetPos(GetPosX(), GetPosY(), GetPosH());
							}
							else
							{
								float fX, fY, fH;
								m_pDeriveRole->GetDerivePos(fX, fY, fH);
								m_pDeriveRole->SetPos(fX, fY, fH);
							}

							m_pDeriveRole->SetTypeC(XMapObj::eEnabled);
							unsigned char byBornActID = m_pDeriveRole->GetBornActID();
							if (byBornActID != 0xFF)
							{
								m_pDeriveRole->ChangeActEx(byBornActID);
							}
							else
							{
								m_pDeriveRole->Wait(true);
							}
							// 设置死亡
							m_byCmdTypeA = eCmdA_Dead;
							m_byCmdTypeB = eCmdB_Dead_End;
						}
					}
					else
					{
						ChangeAct(eAct_Down, true);
					}
				}
			}
			else
			{
				// 完成
				if(ActionEnd())
				{
					// 躺地接倒地起身
					if(eAct_Down == m_byActId)
					{
						// 是否死亡
						if(m_dwHp || !m_dwMaxHp)
							ChangeAct(eAct_GetUp, true);
						else
						{
							if(eCmdA_Dead == m_byCmdTypeA)
							{
								if(eCmdB_Dead_Soul == m_byCmdTypeB || eCmdB_Dead_Hide == m_byCmdTypeB)
								{
									if(m_byDeadAlpha >= 8)
										m_byDeadAlpha -= 8;
									else
									{
										m_byDeadAlpha = 0;
										m_byCmdTypeB = eCmdB_Dead_End;
									}
								}
								else if(eCmdB_Relive_Soul == m_byCmdTypeB)
								{
									if(m_byDeadAlpha < 0xFF - 0x08)
										m_byDeadAlpha += 0x08;
									else
									{
										m_byDeadAlpha = 0xFF;
										m_byCmdTypeB = eCmdB_Relive_End;
										if (bIsMe)
										{
											OnRevive();
											// 玩家复活
											XRole* pRole = this;
											XEventManger::ExecuteEvent(XCommonEvent::EVENT_ONREVIVE, (*((unsigned long*)&(pRole))), "XRole", 0);
										}
									}
								}
								else if(eCmdB_Relive_End == m_byCmdTypeB)
								{
									m_byDeadAlpha = 0xFF;
									m_byCmdTypeA = eCmdA_Hurt;
									m_byCmdTypeB = eCmdB_None;
									SetMp(m_dwMaxMp);
									SetHp(m_dwMaxHp);
									ChangeAct(eAct_GetUp, true);
									if (bIsMe)
									{
										// 刷新MP
										XRole* pRole = this;
										XEventManger::ExecuteEvent(XCommonEvent::HP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
										// 刷新MP
										XEventManger::ExecuteEvent(XCommonEvent::MP_CHANGE, 0);
									}
								}
							}
							else
							{
								m_byCmdTypeA = eCmdA_Dead;
								m_byCmdTypeB = eCmdB_Dead_Hide;
								m_byDeadAlpha = 0xFF;
								m_astAnimation[m_byActId].ToEndFrame(true);
								// 是否出现灵魂
								if(m_pRoleT)
								{
									unsigned char bySoul = rand() % 100;
									if(bySoul < m_pRoleT->valSoul)
									{
										m_byCmdTypeB = eCmdB_Dead_Soul;
										// 用于渲染灵魂
										m_astAnimation[eAct_HurtUp].ToEndFrame(true);
									}
								}
							}
						}
					}
					// 倒地起身要检查AI
					else if(eAct_GetUp == m_byActId)
					{
						if(eHero == m_valTypeB || ePlayer == m_valTypeB)
						{
							static XConfigT* pCT = (XConfigT*)XTableFile::stConfig.GetRowData(13, sizeof(XConfigT));
							if(pCT)
								m_byInvincible = pCT->valParam2;
							if(XRole::m_byIsRevive)
							{
								m_byInvincible = m_byInvincible << 1;
								XRole::m_byIsRevive = 0x00;
							}
						}
						Wait(true);
						AIGetUp();
					}
					else
					{
						//if(eAct_HurtUp == m_byActId && m_pHurtMove)
						//{
						//	// 如果有击飞
						//	if(m_pHurtMove->valDisH)
						//		ChangeAct(eAct_HurtFly, true);
						//	// 其他接等待
						//	else
						//		Wait(true);
						//}
						//// 其他接等待
						//else
						// 此时是否是霸体（可能产生其他bug）
						unsigned char byHurtBreak = m_astAnimation[m_byActId].IsCanHurtBreak();
						if (!byHurtBreak)
						{
							Wait(true);
						}
					}
					// 受创结束后还原之前的摇杆操作
					if(eCmdA_Wait == m_byCmdTypeA && eCmdA_Wait != m_stCmdLast.byTypeA && eCmdA_Skill != m_stCmdLast.byTypeA)
					{
						XRoleCmd stCmd = m_stCmdLast;
						m_stCmdLast.byTypeA = eCmdA_Move;
						m_stCmdLast.byTypeB = eCmdB_Move_Run;
						RecvCmd(stCmd);
					}
				}
			}
		}
		{
			// 重力加速度
			if(fIsZero(m_fAcceH) && m_fPosH > 0.f)
				m_fAcceH = m_ofGravity;
			// 如果是技能，位移操作
			if(SkillAct() && eAct_AttJump != m_byActId)
			{
				// 技能位移操作逻辑
				SetMovScaleX(m_ofMovScale);
				SetMovScaleY(m_ofMovScale);
			}
			else
			{
				SetMovScaleX(1.f);
				SetMovScaleY(1.f);
			}
		}
		// 位移
		if(XMapObj::m_bySlow)
		{
			static float fT = SPF * XMapObj::m_fSlowVal;
			RunMove(fT);
		}
		else
			RunMove(SPF);
		// 该动作是否为飞行
		unsigned short wFlyH = 0, wFlySpd = 0;
		if(m_byActId < ROLE_ACT_NUM)
		{
			if(m_astAnimation[m_byActId].IsValid())
			{
				m_astAnimation[m_byActId].GetFlyInfo(wFlyH, wFlySpd);
				if(wFlyH)
				{
					if(m_fPosH < wFlyH)
					{
						// 此时禁止H移动
						StopH();
						// 上飞
						m_fPosH += wFlySpd;
						if(m_fPosH > wFlyH)
							m_fPosH = wFlyH;
					}
				}
			}
		}
		// 翻面
		if(!fIsZero(m_fSpdX))
		{
			if(!SkillAct() || byChangeSklAct)
			{
				if(m_fSpdX > 0.f)
					m_valFlipX = false;
				else
					m_valFlipX = true;
			}
		}
	}
	// 特效OnFrame
	XEffect* pEffect = m_stEffect._pHead;
	XEffect* pRelease;
	while (pEffect)
	{
		pRelease = pEffect;
		pEffect = pEffect->_pNext;
		{
			pRelease->OnFrame();
			if(pRelease->NeedRemove())
			{
				m_stEffect.DelObj(pRelease);
				pRelease->Cleanup();
				pRelease = NULL;
			}
		}
	}
	if(m_pRoleHead)
	{
		m_pRoleHead->OnFrame();
	}
	if(m_byColorFrame)
		--m_byColorFrame;
	// 动作事件
	if(m_byActId < ROLE_ACT_NUM)
	{
		// 特殊特效 : 每帧一个
		unsigned short wEffID = 0xFFFF;
		// 颜色类型
		unsigned char byColor = 0xFF;
		if(XBIT_VALUE(m_achEffMark, eEff_LvlUp))
		{
			XBIT_CLR(m_achEffMark, eEff_LvlUp);
			wEffID = 2601;
		}
		else if(XBIT_VALUE(m_achEffMark, eEff_Relive))
		{
			XBIT_CLR(m_achEffMark, eEff_Relive);
			wEffID = 2602;
		}
		else if(XBIT_VALUE(m_achEffMark, eEff_Supply_HP))
		{
			XBIT_CLR(m_achEffMark, eEff_Supply_HP);
			wEffID = 2603;
			byColor = 0x01;
		}
		else if(XBIT_VALUE(m_achEffMark, eEff_Supply_MP))
		{
			XBIT_CLR(m_achEffMark, eEff_Supply_MP);
			wEffID = 2603;
			byColor = 0x02;
		}
		if(0xFFFF != wEffID)
		{
			XEffect* pEffect = XMP_CREATE(XEffect);
			if(pEffect)
			{
				if(0x01 == byColor)
				{
					pEffect->SetColor(0x80, 0xFF, 0x00);
				}
				else if(0x02 == byColor)
				{
					pEffect->SetColor(0x37, 0xCA, 0xE8);
				}
				unsigned char bySkillPos = 0xFF;
				pEffect->SetSkillPos(bySkillPos);
				bool bBind = pEffect->SetSkillID(wEffID, this);
				if(bBind)
				{
					m_stEffect.AddObj(pEffect);
				}
				else
				{
					if(m_valMap)
					{
						m_valMap->AddMapObj(pEffect);
					}
					else
					{
						pEffect->Release();
					}
				}
			}
		}
		XFrameInfo* pFrmInfo = m_astAnimation[m_byActId].GetFrameInfo();
		if(!bPause)
		{
			if(m_astAnimation[m_byActId].IsValid())
			{
				if(pFrmInfo)
				{
					if(pFrmInfo->nMoveX)
						StopXY();
					// 动作位移
					if(m_valFlipX)
						m_fPosX -= pFrmInfo->nMoveX;
					else
						m_fPosX += pFrmInfo->nMoveX;
					m_fSpdH += pFrmInfo->nSpdH;
					for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
					{
						// 动作特效
						if(0xFFFF != pFrmInfo->wEffectID[byCnt])
						{
							XEffect* pEffect = XMP_CREATE(XEffect);
							if(pEffect)
							{
								unsigned char bySkillPos = 0xFF;
								if(eAct_AttSkl_1 <= m_byActId && m_byActId <= eAct_AttSkl_4)
									bySkillPos = m_byActId - eAct_AttSkl_1;
								pEffect->SetSkillPos(bySkillPos);
								bool bBind = pEffect->SetSkillID(pFrmInfo->wEffectID[byCnt], this);
								if(bBind)
									m_stEffect.AddObj(pEffect);
								else
								{
									if(m_valMap)
										m_valMap->AddMapObj(pEffect);
									else
										pEffect->Release();
								}
							}
						}
					}
					// 声音 
					if(0xFFFF != pFrmInfo->wSoundID)
					{
						//CCLOG("[ERROR]XDrawAnimation::OnDraw 0xFFFF == stFI.wDrawTexID!");
						XSoundManager::PlaySoundByID(pFrmInfo->wSoundID);
					}
					// 镜头特效
					if (0xFFFF != pFrmInfo->wCameraID)
					{
						XCamera::Instance().Effect(pFrmInfo->wCameraID);
					}
					// 动作残影
					if(pFrmInfo->byStay)
						RG_Used(pFrmInfo->byStay);
				}
			}
			// 受创位移
			if(m_pHurtMove)
			{
				do 
				{
					if(IsPause())
						break;
					if (m_bHurtSky)
					{
						if(m_pHurtMove->valSkyDisH)
						{
							if(m_pHurtMove->valFrameH && byDoSlowFrame)
							{
								++m_byHurtMoveF;
								// H
								float fT = m_pHurtMove->valFrameH * 0.5f;
								float fAcce = 2.f * m_pHurtMove->valSkyDisH / (fT * fT);
								float fVo = fAcce * fT;
								float fVt = fVo - fAcce * m_byHurtMoveF;
								float fMovH = fVt - 0.5f * fAcce;
								m_fPosH += fMovH;
								// X
								float fMovX = m_pHurtMove->valSkyDisX / m_pHurtMove->valFrameH;
								if (m_pHurtMove->valBothSides)
								{
									if(m_bHurtDir)
										m_fPosX -= fMovX;
									else
										m_fPosX += fMovX;
								}
								else
								{
									if(m_byHMFlipX)
										m_fPosX -= fMovX;
									else
										m_fPosX += fMovX;
								}
								if(m_fPosH > 0.01)
									break;
								m_fPosH = 0.f;
							}
							//}
						}
						else if(m_pHurtMove->valSkyDisX)
						{
							// 只有在上受创和下受创中才做击退位移
							if(eAct_HurtUp == m_byActId || eAct_HurtDown == m_byActId)
							{
								// 每帧的位移
								if(++m_byHurtMoveF <= GetActFrameNum())
								{
									float fMovX = m_pHurtMove->valSkyDisX / GetActFrameNum();
									if (m_pHurtMove->valBothSides)
									{
										if(m_bHurtDir)
											m_fPosX -= fMovX;
										else
											m_fPosX += fMovX;
									}
									else
									{
										if(m_byHMFlipX)
											m_fPosX -= fMovX;
										else
											m_fPosX += fMovX;
									}
									break;
								}
							}
						}
					}
					else
					{
						if(m_pHurtMove->valDisH)
						{
							//// 还没飞起来，等等再说
							//if(eAct_HurtUp == m_byActId)
							//	break;
							//// 飞起来了
							//if(eAct_HurtFly == m_byActId)
							//{
							if(m_pHurtMove->valFrameH && byDoSlowFrame)
							{
								++m_byHurtMoveF;
								// H
								float fT = m_pHurtMove->valFrameH * 0.5f;
								float fAcce = 2.f * m_pHurtMove->valDisH / (fT * fT);
								float fVo = fAcce * fT;
								float fVt = fVo - fAcce * m_byHurtMoveF;
								float fMovH = fVt - 0.5f * fAcce;
								m_fPosH += fMovH;
								// X
								float fMovX = m_pHurtMove->valDisX / m_pHurtMove->valFrameH;
								if (m_pHurtMove->valBothSides)
								{
									if(m_bHurtDir)
										m_fPosX -= fMovX;
									else
										m_fPosX += fMovX;
								}
								else
								{
									if(m_byHMFlipX)
										m_fPosX -= fMovX;
									else
										m_fPosX += fMovX;
								}
								if(m_fPosH > 0.01)
									break;
								m_fPosH = 0.f;
							}
							//}
						}
						else if(m_pHurtMove->valDisX)
						{
							// 只有在上受创和下受创中才做击退位移
							if(eAct_HurtUp == m_byActId || eAct_HurtDown == m_byActId)
							{
								// 每帧的位移
								if(++m_byHurtMoveF <= GetActFrameNum())
								{
									float fMovX = m_pHurtMove->valDisX / GetActFrameNum();
									if (m_pHurtMove->valBothSides)
									{
										if(m_bHurtDir)
											m_fPosX -= fMovX;
										else
											m_fPosX += fMovX;
									}
									else
									{
										if(m_byHMFlipX)
											m_fPosX -= fMovX;
										else
											m_fPosX += fMovX;
									}
									break;
								}
							}
						}
					}
					m_pHurtMove = NULL;
					m_bHurtDir = true;
					m_bHurtSky = false;
				} while (false);
			}
		}
		// 碰撞
		if(m_valMap && pFrmInfo && (pFrmInfo->nRcX || pFrmInfo->nRcY || pFrmInfo->nRcH || pFrmInfo->nRcW || pFrmInfo->byThickness))
		{
			// 主城中精灵不注册
			if(XMap::eType_IZ == m_valMap->GetType() || XMapObj::ePet != GetTypeB())
			{
				XCollisionArea stCArea;
				if(m_valFlipX)
				{
					stCArea.fLeft	= m_fPosX - pFrmInfo->nRcX*m_fScale;
					stCArea.fRight	= stCArea.fLeft - pFrmInfo->nRcW*m_fScale;
				}
				else
				{
					stCArea.fLeft	= m_fPosX + pFrmInfo->nRcX*m_fScale;
					stCArea.fRight	= stCArea.fLeft + pFrmInfo->nRcW*m_fScale;
				}
				stCArea.fTop	= m_fPosH + pFrmInfo->nRcY*m_fScale;
				stCArea.fBottom	= stCArea.fTop + pFrmInfo->nRcH*m_fScale;
				stCArea.fPosY	= m_fPosY;
				stCArea.fThickness = pFrmInfo->byThickness*m_fScale;
				stCArea.wType	= m_valTypeA;
				stCArea.pInfo	= this;
				m_valMap->CollideReg(XMapCollisionType::eHurt, m_valCamp, stCArea);
			}
		}
		// 重置
		m_byMapMove = 0x00;
	}
	RG_Run();
	// 角色特效
	if(m_pRoleEffect)
		m_pRoleEffect->AnimateFrame();
	// 套装特效
	if(m_pRoleEquipEffect)
		m_pRoleEquipEffect->AnimateFrame();
	dwRun += 1;
	return dwRun;
	XFUNC_END_RETURN(unsigned long, res, 0xFFFFFFFF);
}

void XRole::OnDrawSoul()
{
	XFUNC_START();
	if(!Valid())
		return;
	if(XMap::eType_IZ != m_byMapType || !IsDead())
		return;
	if(eCmdB_Dead_Soul != m_byCmdTypeB && eCmdB_Relive_Soul != m_byCmdTypeB)
		return;
	float fPosX = m_fPosX;
	float fPosY = m_fPosY + m_fPosH;
	XCamera::Instance().GetRenderPos(fPosX, fPosY);
	// Alpha为当前
	// 位置为[X, Y + 0xFF - Alpha]
	// 动作为上受创动作
	// 偏色为蓝白
	static cocos2d::ccColor3B stColor = cocos2d::ccc3(0xFF, 0xFF, 0xFF);
	unsigned char byOffsetY = 0xFF - m_byDeadAlpha;
	float fSX = 0.8f + byOffsetY * 0.001f;
	float fSY = byOffsetY * 0.004f;
	fSY = 1.f + fSY * fSY;
	//static cocos2d::ccBlendFunc stBFunc = { GL_DST_COLOR, GL_ONE };
	static cocos2d::ccBlendFunc stBFunc = { GL_SRC_ALPHA, GL_ONE };
	static cocos2d::XShaderSoul stShaderSoul;
	static cocos2d::XGLProgram* pGLP = stShaderSoul.GetXProgram();
	unsigned char byAlpha = m_byDeadAlpha > 0x7F ? 0xFF : (m_byDeadAlpha << 1);
	stShaderSoul.SetAlpha(byAlpha);
	static unsigned short wW, wH;
	unsigned char byAct = eAct_HurtUp;
	if(eCmdB_Relive_Soul == m_byCmdTypeB)
		byAct = eAct_HurtDown;
	m_astAnimation[byAct].GetFrameSize(wW, wH);
	stShaderSoul.SetTexRect(wW, wH);
	m_astAnimation[byAct].OnDrawEx(fPosX, fPosY + byOffsetY, fSX, fSY, stColor,
		byAlpha, stBFunc, m_valFlipX ? true : false,
		pGLP);
	XFUNC_END();
}

void XRole::OnDrawShadow()
{
	XFUNC_START();
	if(!m_byEnableSee)
		return;
	if(!Valid())
		return;
	// 触发器没有影子
	if (XMapObj::eSwitch == m_valTypeB)
	{
		return;
	}
	
	if(XMapObj::eDisabled == m_valTypeC || XMapObj::eUnDerive == m_valTypeC || XMapObj::eUnBorned == m_valTypeC)
		if (XMapObj::ePet != m_valTypeB)
		{
			return;
		}

	if(m_byActId >= ROLE_ACT_NUM)
		return;
	if(!m_pRoleT)
		return;
	if(!m_pRoleT->valSDScale)
		return;
	float fAlpha = 1.f - m_fPosH / ((SCR_H >> 1) + (SCR_H >> 2));
	if(fAlpha < 0.001f)
		return;
	unsigned char byAlpha = (unsigned char)(0xFF * fAlpha);
	if(XMapObj::m_valMap)
	{
		if(this != XMapObj::m_valMap->GetControl())
			byAlpha = (unsigned char)(m_byDeadAlpha * fAlpha);
	}
	m_pShadowSprite->setOpacity(byAlpha);
	float fPosX = m_fPosX;
	float fPosY = m_fPosY;
	XCamera::Instance().GetRenderPos(fPosX, fPosY);
	m_pShadowSprite->setPosition(ccp(fPosX, fPosY));
	float fScale = m_pRoleT->valSDScale * 0.001f * fAlpha;
	m_pShadowSprite->setScale(fScale*m_fScale);
	m_pShadowSprite->visit();
	// 玩家脚底特效
	if(m_pRoleEffect && XMapObj::m_valMap && this == XMapObj::m_valMap->GetControl() && XMapObj::m_valMap->GetOtherPlayerNum() > 0)
		m_pRoleEffect->OnGameEffectDraw(fPosX, fPosY);
	XFUNC_END();
}

void XRole::OnDraw()
{
	XFUNC_START();
	if(!m_byEnableSee)
		return;
	if(!Valid())
		return;
	if(XMapObj::eDisabled == m_valTypeC || XMapObj::eUnDerive == m_valTypeC || XMapObj::eUnBorned == m_valTypeC)
	{
		if (XMapObj::ePet != m_valTypeB)
		{
			return;
		}
	}
	if(m_byActId >= ROLE_ACT_NUM)
		return;
	bool bIsMe = false;
	if(XMapObj::m_valMap)
	{
		if(this == XMapObj::m_valMap->GetControl())
			bIsMe = true;
	}
	// 特效OnDraw【被角色盖住】
	XEffect* pEffect = m_stEffect._pHead;
	while (pEffect)
	{
		if(!pEffect->CoverRole())
			pEffect->OnDraw();
		pEffect = pEffect->_pNext;
	}
	// Camera
	// ...
	float fPosX = m_fPosX;
	float fPosY = m_fPosY + m_fPosH;
	float fPosG = m_fPosY;
	XCamera::Instance().GetRenderPos(fPosX, fPosY);
	// 震动
	fPosX += m_chHurtShake;

	////fPosX = (float)((int)fPosX);
	////fPosY = (float)((int)fPosY);
	////fPosG = (float)((int)fPosG);
	// 渲染颜色
	if(!m_byColorFrame)
	//if (!IsPause())
	{
		m_stColor = m_stNoColor;
	}
	cocos2d::XGLProgram* pGLPShader = NULL;
	//// 慢镜头黑白
	//if(XMapObj::m_bySlow)
	//{
	//	static cocos2d::XShaderWhite stShaderWhite;
	//	pGLPShader = stShaderWhite.GetXProgram();
	//	const float fC = 1.f / 255.f;
	//	stShaderWhite.SetParamf1((float)m_byDeadAlpha * fC);
	//}
	//else
	//{
		// 霸体特效
		unsigned char byHurtBreak = m_astAnimation[m_byActId].IsCanHurtBreak();
		if(m_valMap)
		{
			if(m_valMap->m_bySpecial)
				byHurtBreak = 0xFF;
			if(m_valMap->m_byActivity && eBoss == m_valTypeB)
				byHurtBreak = 0xFF;
		}
		if(byHurtBreak && ePet != m_valTypeB)
		{
			static cocos2d::XShaderStroke stShaderStroke;
			pGLPShader = stShaderStroke.GetXProgram();
			static unsigned short wW, wH;
			m_astAnimation[m_byActId].GetFrameSize(wW, wH);
			stShaderStroke.SetTexRect(wW, wH);
			static float fAlpha = 0.005f;
			fAlpha += 0.005f;
			if(fAlpha > 0.2f)
				fAlpha = 0.005f;
			if(!m_byHBLast)
			{
				m_byHBLast = byHurtBreak;
				fAlpha = 0.005f;
			}
			stShaderStroke.SetParamf1(fAlpha);
			if(m_byColorFrame)
				stShaderStroke.SetParamf2(1.f);
			else
				stShaderStroke.SetParamf2(0.f);
		}
		else
			m_byHBLast = 0x00;
	//}
	// 角色渲染
	static cocos2d::ccBlendFunc stBFuncNor = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	if(bIsMe)
	{
		unsigned char byAlpha = 0xFF;
		if(m_byInvincible % 2)
			byAlpha >>= 1;
		//if(m_byColorFrame % 2)
		m_astAnimation[m_byActId].OnDrawEx(fPosX, fPosY, m_fScale, m_fScale, m_stColor, byAlpha, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
		//else
		//	m_astAnimation[m_byActId].OnDrawEx(fPosX, fPosY, m_fScale, m_fScale, m_stNoColor, byAlpha, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
	}
	else
	{
		unsigned char byAlpha = m_byDeadAlpha;
		if(m_byInvincible % 2)
			byAlpha >>= 1;
		if(XMapObj::eWood == m_valTypeB)
		{
			if(m_byColorFrame % 4)
				m_astAnimation[m_byActId].OnDrawEx(fPosX, fPosY, m_fScale, m_fScale, m_stColor, byAlpha, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
			else
				m_astAnimation[m_byActId].OnDrawEx(fPosX, fPosY, m_fScale, m_fScale, m_stNoColor, byAlpha, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
		}
		else
			m_astAnimation[m_byActId].OnDrawEx(fPosX, fPosY, m_fScale, m_fScale, m_stColor, byAlpha, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
	}
	//if(bIsMe)
	//{
	//	//if(m_byColorFrame % 2)
	//	m_astAnimation[m_byActId].OnDraw(fPosX, fPosY, 1.f, m_stColor, 0xFF, m_valFlipX ? true : false);
	//	//else
	//	//	m_astAnimation[m_byActId].OnDraw(fPosX, fPosY, 1.f, m_stNoColor, 0xFF, m_valFlipX ? true : false);
	//}
	//else
	//{
	//	if(XMapObj::eWood == m_valTypeB)
	//	{
	//		if(m_byColorFrame % 4)
	//			m_astAnimation[m_byActId].OnDraw(fPosX, fPosY, 1.f, m_stColor, m_byDeadAlpha, m_valFlipX ? true : false);
	//		else
	//			m_astAnimation[m_byActId].OnDraw(fPosX, fPosY, 1.f, m_stNoColor, m_byDeadAlpha, m_valFlipX ? true : false);
	//	}
	//	else
	//		m_astAnimation[m_byActId].OnDraw(fPosX, fPosY, 1.f, m_stColor, m_byDeadAlpha, m_valFlipX ? true : false);
	//}
	if(!XMapObj::m_bySlow)
	{
		// 残影渲染
		RG_OnDraw();
		// 灵魂渲染
		if(cocos2d::XScrSprite::eShaderNone == cocos2d::XScrSprite::GetShaderType())
		{
			OnDrawSoul();
		}
		// 套装特效
		if(m_pRoleEquipEffect)
		{
			if(!bIsMe || !IsDead())
			{
				m_pRoleEquipEffect->OnGameEffectDraw(fPosX, fPosY);
			}
		}
		//名字
		unsigned char byType = GetTypeB();
		if(m_pRoleHead && !(XMapObj::eWood <= byType && XMapObj::eBoss >= byType))
		{
			float fW, fH;
			m_pRoleHead->GetSize(fW, fH);
			//m_pRoleName->SetPosition(fPosX + m_wNameOffSetX, fPosY + m_wNameOffSetY);
			m_pRoleHead->OnDraw(fPosX - fW * 0.5f, fPosY + m_wNameOffSetY*m_fScale);
		}
	}
	else
	{
		// 套装特效
		if(m_pRoleEquipEffect)
		{
			if(!bIsMe || !IsDead())
			{
				m_pRoleEquipEffect->OnGameEffectDraw(fPosX, fPosY);
			}
		}
	}

	// 特效OnDraw【盖住角色】
	pEffect = m_stEffect._pHead;
	while (pEffect)
	{
		if(pEffect->CoverRole())
			pEffect->OnDraw();
		pEffect = pEffect->_pNext;
	}
#	ifdef X_SHOWLINE
	if (XJoystick::m_bDrawLine)
	{
		// 位置线
		glLineWidth(1);
		cocos2d::ccDrawColor4B(0x00, 0x00, 0xFF, 0xFF);
		cocos2d::ccDrawLine(ccp(fPosX, fPosY), ccp(fPosX, fPosY + 100.f));
	}
#	endif
	if(m_pRoleEffect)
	{
		// 玩家自己
		if(XMapObj::m_valMap && m_pRoleT)
		{
			if(this != XMapObj::m_valMap->GetControl())
				m_pRoleEffect->OnGameEffectDraw(fPosX, fPosY + m_pRoleT->valNameH);
		}
	}
	XFUNC_END();
}

bool XRole::NeedRemove()
{
	XFUNC_START();
	if(!Valid())
		return true;
	if(XMapObj::m_valMap)
	{
		if(this == XMapObj::m_valMap->GetControl())
			return false;
	}
	return IsDeadEnd();
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::OnEnterMap(XMap* pMap)
{
	XFUNC_START();
	XMapObj::OnEnterMap(pMap);
	if (!pMap)
	{
		return;
	}
	unsigned short wEffAniId;
	m_byMapType = pMap->GetType();
	if(XMap::eType_City == m_byMapType)
	{
		LoadAction(eActType_City);
	}
	else if(XMap::eType_IZ == m_byMapType)
	{
		LoadAction(eActType_All);
		wEffAniId = XEffect::m_oastEffInfo[2602].wAniId;
		if(XDrawAnimation::m_oapAni[wEffAniId])
			XDrawAnimation::m_oapAni[wEffAniId]->AddRef();
		wEffAniId = XEffect::m_oastEffInfo[2603].wAniId;
		if(XDrawAnimation::m_oapAni[wEffAniId])
			XDrawAnimation::m_oapAni[wEffAniId]->AddRef();
	}
	else if (XMap::eType_SelRole == m_byMapType)
	{
		LoadAction(eActType_SelRole);
	}
	
	m_byDeadAlpha = 0xFF;
	XRoleCmd stCmd;
	stCmd.byTypeA = eCmdA_Wait;
	stCmd.byTypeB = eCmdB_None;
	RecvCmd(stCmd);
	Wait(true);
	wEffAniId = XEffect::m_oastEffInfo[2601].wAniId;
	if(XDrawAnimation::m_oapAni[wEffAniId])
		XDrawAnimation::m_oapAni[wEffAniId]->AddRef();
	XFUNC_END();
}

void XRole::OnLeaveMap(XMap* pMap)
{
	XFUNC_START();
	XMapObj::OnLeaveMap(pMap);
	unsigned short wEffAniId = XEffect::m_oastEffInfo[2601].wAniId;
	if(XDrawAnimation::m_oapAni[wEffAniId])
		XDrawAnimation::m_oapAni[wEffAniId]->DelRef();
	if (!pMap)
	{
		return;
	}
	m_byMapType = pMap->GetType();
	if (XMap::eType_IZ == m_byMapType)
	{
		wEffAniId = XEffect::m_oastEffInfo[2602].wAniId;
		if(XDrawAnimation::m_oapAni[wEffAniId])
			XDrawAnimation::m_oapAni[wEffAniId]->DelRef();
		wEffAniId = XEffect::m_oastEffInfo[2603].wAniId;
		if(XDrawAnimation::m_oapAni[wEffAniId])
			XDrawAnimation::m_oapAni[wEffAniId]->DelRef();
	}
	
	XFUNC_END();
}

void XRole::OnLevelUp()
{
	XFUNC_START();
	XBIT_MARK(m_achEffMark, eEff_LvlUp);
	XFUNC_END();
}

void XRole::OnRevive()
{
	XFUNC_START();
	XBIT_MARK(m_achEffMark, eEff_Relive);
	XFUNC_END();
}

void XRole::Release()
{
	XFUNC_START();
	// 清除特效
	XEffect* pEffect = m_stEffect._pHead;
	XEffect* pRelease;
	while (pEffect)
	{
		pRelease = pEffect;
		pEffect = pEffect->_pNext;
		pRelease->Cleanup();
	}
	m_stEffect.Clear();
	// 释放动作
	LoadAction(eActType_None);
	XFUNC_END();
}

void XRole::SetRoleName(const char* szRoleName)
{
	XFUNC_START();
	if(!szRoleName || !szRoleName[0])
		return;
	//sprintf(m_szRoleName, "%s", szRoleName);
	//CCLOG("【创建角色】%s", m_szRoleName);
	//const char* szUtf8Name = _stXStrCnvMainTh.XGB2312_TO_UTF8(szRoleName);
	if(!szRoleName || !szRoleName[0])
		return;
	if(!m_pRoleHead)
	{
		m_pRoleHead = XHeadEffect::ToLabel(XHeadEffect::Create());//cocos2d::CCLabelTTF::create(szUtf8Name, "黑体", 24);
		m_pRoleHead->ShowWindow(true);
	}

	if(m_pRoleHead)
	{
		//m_pRoleName->retain();
		//cocos2d::CCSize stNameSize = m_pRoleName->getContentSize();
		m_pRoleHead->SetSize(200, 100);
		if(XMapObj::eHero == GetTypeB())
		{
			m_pRoleHead->setNameColor(0,0xFF,0);
		}
		else if(XMapObj::ePlayer == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xFF,0xFF,0xFF);
		}
		else if(XMapObj::eCity == GetTypeB())
		{
			m_pRoleHead->setNameColor(0,0xFF,0xFF);
		}
		else if(XMapObj::eWood == GetTypeB())
		{

		}
		else if(XMapObj::eFight == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xFF,0,0xFF);
		}
		else if(XMapObj::eElite == GetTypeB())
		{
			
		}
		else if(XMapObj::eBoss == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xAA,0xBB,0);
		}
		if(0xFFFF != m_wRoleID /*&& m_wRoleID < ROLE_TYPE_NUM*/)
		{
			if(m_pRoleT)
			{
				m_wNameOffSetY = m_pRoleT->valNameH;
			}
		}
		m_pRoleHead->setRoleName(szRoleName);
	}
	XFUNC_END();
}
void XRole::SetRoleNameByUnicode(const char* szRoleName)
{
	XFUNC_START();
	const unsigned short* szUni = (const unsigned short*)szRoleName;
	if(!szUni || szUni[0] == '\0')
		return;
	////CCLOG("【创建角色】%s", szRoleName);
	//memset(m_szRoleName, 0, sizeof(char)*ROLE_NAME_LEN);
	//OGDP::xstr_wcsncpy((unsigned short*)m_szRoleName, (unsigned short*)szRoleName, ROLE_NAME_LEN >> 1);
	//m_szRoleName[ROLE_NAME_LEN - 2] = '\0';
	//m_szRoleName[ROLE_NAME_LEN - 1] = '\0';
	//int nNameLen = OGDP::xstr_wcslen((unsigned short*)szRoleName);
	//if(nNameLen < (ROLE_NAME_LEN >> 1))
	//{
	//	m_szRoleName[(nNameLen << 1) + 0] = '\0';
	//	m_szRoleName[(nNameLen << 1) + 1] = '\0';
	//}
	if(!m_pRoleHead)
	{
		m_pRoleHead = XHeadEffect::ToLabel(XHeadEffect::Create());//cocos2d::CCLabelTTF::create(szUtf8Name, "黑体", 24);
		m_pRoleHead->ShowWindow(true);
	}

	if(m_pRoleHead)
	{
		//m_pRoleName->retain();
		//cocos2d::CCSize stNameSize = m_pRoleName->getContentSize();
		m_pRoleHead->SetSize(200, 100);
		if(XMapObj::eHero == GetTypeB())
		{
			m_pRoleHead->setNameColor(0,0xFF,0);
		}
		else if(XMapObj::ePlayer == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xFF,0xFF,0xFF);
		}
		else if(XMapObj::eCity == GetTypeB())
		{
			m_pRoleHead->setNameColor(0,0xFF,0xFF);
		}
		else if(XMapObj::eWood == GetTypeB())
		{

		}
		else if(XMapObj::eFight == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xFF,0,0xFF);
		}
		else if(XMapObj::eElite == GetTypeB())
		{

		}
		else if(XMapObj::eBoss == GetTypeB())
		{
			m_pRoleHead->setNameColor(0xAA,0xBB,0);
		}
		if(0xFFFF != m_wRoleID /*&& m_wRoleID < ROLE_TYPE_NUM*/)
		{
			if(m_pRoleT)
			{
				m_wNameOffSetY = m_pRoleT->valNameH;
			}
		}
		m_pRoleHead->SetRoleNameByUnicode(szRoleName);
	}
	XFUNC_END();	
}
char* XRole::GetRoleTableName()
{
	XFUNC_START();
	if(m_pRoleT)
	{
		XSvrLangT* pSvrLang = (XSvrLangT*)XTableFile::stSvrLang.GetRowData(m_pRoleT->valName, sizeof(XSvrLangT));
		if(pSvrLang)
		{
			return pSvrLang->valStr;
		}
	}
	XFUNC_END_RETURN(char*, res, NULL);
}
void XRole::SendCmd(const XRoleCmd& stCmd)
{
	XFUNC_START();
	// AI时不响应动作
	if(ePAI_None != m_byPlayerAI)
		return;
	// 移动式不响应相同动作
	if(eCmdA_Move == stCmd.byTypeA && m_stCmdLast == stCmd)
		return;
	// 单人副本 | 战斗NPC ：直接响应
	// ...
	{
		RecvCmd(stCmd);
	}
	// 城市：直接响应移动操作
	// ...
	// 多人副本：发送给服务器
	// ...
	m_stCmdLast = stCmd;
	XFUNC_END();
}

void XRole::RecvCmd(const XRoleCmd& stCmd)
{
	XFUNC_START();
	if(stCmd.byTypeA >= cCmdA_Active)
		return;
	switch(stCmd.byTypeA)
	{
	case eCmdA_Wait:	{ m_byCmdTypeB = eCmdB_None; Wait(); } break; // 主动停才是真的停
	case eCmdA_Move:	Move(stCmd.nParam1 * 0.001f, stCmd.nParam2 * 0.001f, stCmd.byTypeB ? true : false); break;
	case eCmdA_MoveTo:	MoveTo(stCmd.nParam1, stCmd.nParam2, stCmd.byTypeB ? true : false); break;
	case eCmdA_Jump:	Jump(); break;
	case eCmdA_Dodge:	Dodge();break;
	case eCmdA_Skill:	UseSkill(stCmd.byTypeB);
	}
	if(!m_obyIZStart)
	{
		if(m_valMap && eCmdA_Wait != stCmd.byTypeA)
		{
			if(this == m_valMap->GetControl())
				m_obyIZStart = 0xFE;
		}
	}
	// 仅新手引导用
	if(m_valMap && eCmdA_Wait != stCmd.byTypeA)
	{
		if(this == m_valMap->GetControl())
			XEventManger::ExecuteEvent(XCommonEvent::GUILD_MOVEEND, 0);
	}
	//if(m_valMap && this == m_valMap->GetControl())
	//{
	//	CCLOG("【RecvCmd】Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
	//		m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY);
	//}
	XFUNC_END();
}

void XRole::Wait(bool bForce/* = false*/)
{
	XFUNC_START();
	if(!bForce)
	{
		if(m_byCmdTypeA >= cCmdA_WaitNoForce)
		{
			// CmdA为主动行为时，只要没有强制就返回
			if(m_byCmdTypeA < cCmdA_Active)
				return;
			// CmdA为被动行为时，只要没有播放完成就返回
			else
			{
				if(!ActionEnd())
					return;
			}
		}
	}
	// Command
	if(fIsZero(m_fPosH))
	{
		m_pHurtMove = NULL;
		m_bHurtDir = true;
		m_bHurtSky = false;
		// 如果是MoveTo，则代表主动停
		if(eCmdA_MoveTo == m_byCmdTypeA)
			m_byCmdTypeB = eCmdB_None;
		// 如果没主动停
		if(eCmdB_Move_Run == m_byCmdTypeB)
		{
			ChangeAct(eAct_Run);
			m_byCmdTypeA = eCmdA_Move;
		}
		else if(eCmdB_Move_Walk == m_byCmdTypeB)
		{
			ChangeAct(eAct_Walk);
			m_byCmdTypeA = eCmdA_Move;
		}
		else
		{
			if(0xFF != m_byMarAfterRun)
			{
				UseSkill(m_byMarAfterRun);
				m_byMarAfterRun = 0xFF;
				return;
			}
			// MoveObj
			StopXY();
			m_byCmdTypeA = eCmdA_Wait;
			if(m_byMapMove)
			{
				m_valFlipX = 0;
				ChangeAct(eAct_Walk);
			}
			else
			{
				if(XMap::eType_IZ == m_byMapType)
				{
					ChangeAct(eAct_Wait);
				}
				else
				{
					ChangeAct(eAct_WaitInCity);
				}
			}
		}
	}
	else
	{
		m_byCmdTypeA = eCmdA_Dodge;
		ChangeAct(eCmdA_Dodge);
	}
	XFUNC_END();
}

void XRole::MoveTo(unsigned short wPosX, unsigned short wPosY, bool bRun)
{
	XFUNC_START();
	if(!Valid())
		return;
	if(bRun)
		m_byCmdTypeB = eCmdB_Move_Run;
	else
		m_byCmdTypeB = eCmdB_Move_Walk;
	// 如果是被动动作，不可打断
	if(m_byCmdTypeA >= cCmdA_Active)
		return;
	if(bRun)
		XRenderObj::MoveTo(wPosX, wPosY, m_pRoleT->valSpdRun * m_wSpdScale * 0.01f);
	else
		XRenderObj::MoveTo(wPosX, wPosY, m_pRoleT->valSpdWalk * m_wSpdScale * 0.01f);
	// 如果在跳跃中或技能，不改变动作
	if(eCmdA_Jump == m_byCmdTypeA || eCmdA_Skill == m_byCmdTypeA)
		return;
	if(bRun)
		ChangeAct(eAct_Run);
	else
		ChangeAct(eAct_Walk);
	m_byCmdTypeA = eCmdA_MoveTo;
	XFUNC_END();
}

void XRole::Move(float fScaleX, float fScaleY, bool bRun)
{
	XFUNC_START();
	if(!Valid())
		return;
	if(bRun)
		m_byCmdTypeB = eCmdB_Move_Run;
	else
		m_byCmdTypeB = eCmdB_Move_Walk;
	// 如果是被动动作，不可打断
	if(m_byCmdTypeA >= cCmdA_Active)
		return;
	if(bRun)
	{
		fScaleX *= m_pRoleT->valSpdRun * m_wSpdScale * 0.01f;
		fScaleY *= m_pRoleT->valSpdRun * m_wSpdScale * 0.01f;
	}
	else
	{
		fScaleX *= m_pRoleT->valSpdWalk * m_wSpdScale * 0.01f;
		fScaleY *= m_pRoleT->valSpdWalk * m_wSpdScale * 0.01f;
	}
	XRenderObj::SetSpeed(fScaleX, fScaleY);
	// 如果在跳跃中或技能，不改变动作
	if(eCmdA_Jump == m_byCmdTypeA || eCmdA_Skill == m_byCmdTypeA)
		return;
	if(bRun)
		ChangeAct(eAct_Run);
	else
		ChangeAct(eAct_Walk);
	m_byCmdTypeA = eCmdA_Move;
	XFUNC_END();
}

void XRole::Jump()
{
	XFUNC_START();
	if(!Valid())
		return;
	// 如果是被动动作或技能，不可打断
	if(m_byCmdTypeA >= eCmdA_Skill)
		return;
	if(eCmdA_Jump == m_byCmdTypeA)
		return;
	ChangeAct(eAct_Jump, true);
	XRenderObj::SetSpdH(m_pRoleT->valSpdJump);
	m_byCmdTypeA = eCmdA_Jump;
	m_byJumpAttack = 0;
	XFUNC_END();
}

void XRole::Dodge()
{
	XFUNC_START();
	if(!Valid())
		return;
	if(m_byCmdTypeA >= eCmdA_Skill)
		return;
	if(m_byCmdTypeA == eCmdA_Dodge)
		return;
	ChangeAct(eAct_Dodge);
	XFUNC_END();
}

void XRole::UseSkill(unsigned char byType)
{
	XFUNC_START();
	// 记录下一技能，用最后的输入操作直接替换之前的记录
	m_byMarTypeNext = byType;
	XFUNC_END();
}

void XRole::UseSklAfterRunTo(unsigned short wPosX, unsigned short wPosY, unsigned char byType)
{
	XFUNC_START();
	m_byMarAfterRun = byType;
	MoveTo(wPosX, wPosY, true);
	XFUNC_END();
}

bool XRole::SkillAct()
{
	XFUNC_START();
	if(eAct_AttNor_1 <= m_byActId && m_byActId <= eAct_AttSkl_4)
		return true;
	XFUNC_END_RETURN(bool, res, false);
}

bool XRole::OnHurt(XEffect* pEffect, bool bDir)
{
	XFUNC_START();
	do 
	{
		m_byIsMT = 0x00;
		//CCLOG("【XRole::OnHurt】 %s", m_pRoleT->valName);
		//if(m_valMap && m_valMap->GetControl() == this)
		//	m_dwHp = 0xFFFF;
		if(!m_dwHp && (eCmdA_Hurt == m_byCmdTypeA || eCmdA_Dead == m_byCmdTypeA))
		{
			//CCLOG("role is dead, no hurt.");
			break;
		}
		// 副本切屏时，玩家位置必须超过切屏坐标才会激活怪物AI，这里是为了防止玩家利用这个规则在切屏线外远程攻击
		if(0xFF != m_obyIZStart)
		{
			if(ePlayer != m_valTypeB && eHero != m_valTypeB && ePet != m_valTypeB)
			{
				CCLOG("【XRole::OnHurt】 激活怪物AI");
				m_obyIZStart = 0xFF;
			}
		}

		if(!pEffect)
		{
			//CCLOG("effect is not exist, no hurt.");
			break;
		}

		// 生效死亡
		XRole* pRole = pEffect->GetOwner();
		if (pRole && pRole->GetTypeB() == XMapObj::eSwitch && (GetTypeB() == XMapObj::eHero || GetTypeB() == XMapObj::ePlayer))
		{
			if (pRole->GetHitDead())
			{
				if (!pRole->IsDead())
				{
					pRole->Dead();
				}				
			}
			return true;
		}
		// 掉落
		if (pRole && pRole->GetTypeB() == XMapObj::eSupply && GetTypeB() == XMapObj::eHero)
		{
			unsigned short wLogic = pRole->GetDropLogic();
			if(0xFFFF != wLogic)
			{
				pRole->SetDropLogic(0xFFFF);
				XItemLogicT* pLogic = (XItemLogicT*)XTableFile::stItemLogic.GetRowData(wLogic, sizeof(XItemLogicT));
				if(pLogic)
				{
					// HP
					if(1000 == pLogic->valType)
					{
						SetHp(m_dwHp + pLogic->valParam);
						XRole* pRole = this;
						XEventManger::ExecuteEvent(XCommonEvent::HP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
						XSupplyNum* pSupplyNum = XSupplyNum::Create(pLogic->valParam, this, XSupplyNum::eHp);
						if(pSupplyNum)
						{
							if(m_valMap)
								m_valMap->AddMapObj(pSupplyNum);
							else
								pSupplyNum->Release();
						}
						XBIT_MARK(m_achEffMark, eEff_Supply_HP);
					}
					// MP
					else if(1001 == pLogic->valType)
					{
						SetMp(m_dwMp + pLogic->valParam);
						XRole* pRole = this;
						XEventManger::ExecuteEvent(XCommonEvent::MP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
						XSupplyNum* pSupplyNum = XSupplyNum::Create(pLogic->valParam, this, XSupplyNum::eMp);
						if(pSupplyNum)
						{
							if(m_valMap)
								m_valMap->AddMapObj(pSupplyNum);
							else
								pSupplyNum->Release();
						}
						XBIT_MARK(m_achEffMark, eEff_Supply_MP);
					}
					// Buff
					else if(8 == pLogic->valType)
					{

					}
				}
			}
			if(m_valMap)
			{
				m_valMap->DelRole(pRole);
			}
			pRole->Cleanup();
			pRole = NULL;

			return true;
		}
		if(m_byInvincible)
		{
			//CCLOG("role is Invincible, no hurt.");
			break;
		}
		XEffect::XEffInfo* pInfo = pEffect->GetInfo();
		if(!pInfo)
		{
			//CCLOG("effect info is not exist, no hurt.");
			break;
		}
		if(!pInfo->pSkillT)
		{
			//CCLOG("effect table is not exist, no hurt.");
			break;
		}
		XSkillT* pST = pInfo->pSkillT;
		// Reset
		m_pHurtMove = NULL;
		m_bHurtDir = true;
		m_bHurtSky = false;
		m_byHurtMoveF = 0;
		m_byHMFlipX = pEffect->GetFlipX();
		m_byMarTypeNext = 0xFF;
		// 木桶类无晃动
		//if(XMapObj::eWood != GetTypeB())
		{
			// Shake
			if(ROLE_HURTSHAKE_PIX == m_chHurtShake)
				m_chHurtShake = -ROLE_HURTSHAKE_PIX;
			else
				m_chHurtShake = ROLE_HURTSHAKE_PIX;
		}
		// 变色
		{
			m_stColor = *((cocos2d::ccColor3B*)&(pST->valHitColor));
			unsigned char byTmp = m_stColor.r;
			m_stColor.r = m_stColor.b;
			m_stColor.b = byTmp;
			m_byColorFrame = 10;
		}
		
		// 此时是否是霸体（可能产生其他bug）
		unsigned char byHurtBreak = m_astAnimation[m_byActId].IsCanHurtBreak();
		if(m_valMap)
		{
			if(m_valMap->m_bySpecial)
				byHurtBreak = 0xFF;
			if(m_valMap->m_byActivity && eBoss == m_valTypeB)
				byHurtBreak = 0xFF;
		}
		if (byHurtBreak)
		{
			if(!XBIT_VALUE(m_achEffMark, eEff_Break))
			{
				XBIT_MARK(m_achEffMark, eEff_Break);
				RG_Used(FPS, XRoleGhosting::XRGFrame::eNoBreak, 0x9F);
			}
		}
		else
		{
			m_byCmdTypeB = eCmdB_None;
			if(eCmdA_Hurt != m_byCmdTypeA && (eBoss == m_valTypeB || eHero == m_valTypeB || ePlayer == m_valTypeB))
			{
				RG_Used(FPS, XRoleGhosting::XRGFrame::eHurt, 0x9F);
			}
			// Action
			if(pST->valDisH || m_fPosH > 1.f)
			{
				//// 击飞时先做上受创
				//ChangeAct(eAct_HurtUp, true);
				ChangeAct(eAct_HurtFly, true);
				m_pHurtMove = pST;
				m_bHurtDir = bDir;
				if (m_fPosH > 1.f)
					m_bHurtSky = true;
				else
					m_bHurtSky = false;				
			}
			else
			{
				if(pST->valDisX)
				{
					m_pHurtMove = pST;
					m_bHurtDir = bDir;
					m_bHurtSky = false;
				}
				// 如果当前是上受创
				if(eAct_HurtUp == m_byActId)
					ChangeAct(eAct_HurtDown, true);
				// 如果当前是下受创
				else if(eAct_HurtDown == m_byActId)
					ChangeAct(eAct_HurtUp, true);
				// 否则随机普通受创
				else if(rand() % 2)
					ChangeAct(eAct_HurtUp, true);
				else
					ChangeAct(eAct_HurtDown, true);
			}
		}
		// 是否暴击
		bool bCrit = false;
		// 计算
		XRole* pAttack = pEffect->GetOwner();
		if(pAttack)
		{
			float fHurtValue = 0.f;
			unsigned char bySklPos = pEffect->GetSkillPos();
			unsigned char bySklLvl = pAttack->GetSkillLvl(bySklPos);
			// 是不是技能
			if(0xFF == bySklPos)
			{
				// 普通攻击 : 是否破防
				if(pAttack->m_dwAttPhy > m_dwDefPhy)
				{
					// 普攻伤害=[(自身物理攻击－目标物理防御)*伤害浮动系数A，(自身物理攻击－目标物理防御)*伤害浮动系数B]
					unsigned long dwDiffPhy = DIFF(pAttack->m_dwAttPhy, m_dwDefPhy);
					unsigned long dwHVMin = dwDiffPhy * 900;
					unsigned long dwHVMax = dwDiffPhy * 1100;
					dwDiffPhy = DIFF(dwHVMin, dwHVMax);
					if (dwDiffPhy)
					{
						fHurtValue = (rand() % dwDiffPhy + dwHVMin) * 0.001f;
					}
					// 目标物理防御为0的处理
					else
					{
						fHurtValue = (dwHVMin) * 0.001f;
					}
					//CCLOG("【XRole::OnHurt】普通攻击 : %f", fHurtValue);
				}
				else
				{
					//CCLOG("【XRole::OnHurt】普通攻击 : 未破防");
				}
			}
			// 技能学会没有
			else if(0x00 != bySklLvl)
			{
				float fSkillAdd = 1.f;
				long lSkillAdd = 0;
				XRoleT* pAttRoleT = pAttack->m_pRoleT;
				unsigned char byType = pAttack->GetTypeB();
				// 玩家才有加成
				if( (XMapObj::ePlayer == byType || XMapObj::ePet == byType || XMapObj::eHero == byType) && 0xFF != bySklPos && 0x00 != bySklLvl && pAttRoleT)
				{
					unsigned long dwSklLvlRow = pAttRoleT->valSklLvlId + bySklLvl;
					XSkillLvlT* pSklLvlT = (XSkillLvlT*)XTableFile::stTblSklLvl.GetRowData(dwSklLvlRow, sizeof(XSkillLvlT));
					if(pSklLvlT)
					{
						switch(bySklPos)
						{
						case 0: { fSkillAdd = pSklLvlT->valSklAdd_1 * 0.0001f; lSkillAdd = pSklLvlT->valAttack_1; } break;
						case 1: { fSkillAdd = pSklLvlT->valSklAdd_2 * 0.0001f; lSkillAdd = pSklLvlT->valAttack_2; } break;
						case 2: { fSkillAdd = pSklLvlT->valSklAdd_3 * 0.0001f; lSkillAdd = pSklLvlT->valAttack_3; } break;
						case 3: { fSkillAdd = pSklLvlT->valSklAdd_4 * 0.0001f; lSkillAdd = pSklLvlT->valAttack_4; } break;
						}
					}
				}

				// 技能的伤害系数
				float fSkillParam = 1.f;
				if(pAttRoleT)
				{
					if(0 == bySklPos)
					{
						fSkillParam = ((float)pAttRoleT->valSklParam_1)*0.01f;
					}
					else if(1 == bySklPos)
					{
						fSkillParam = ((float)pAttRoleT->valSklParam_2)*0.01f;
					}
					else if(2 == bySklPos)
					{
						fSkillParam = ((float)pAttRoleT->valSklParam_3)*0.01f;
					}
					else if(3 == bySklPos)
					{
						fSkillParam = ((float)pAttRoleT->valSklParam_4)*0.01f;
					}
				}
				// 技能攻击 : 是否破防
				unsigned long dwAttSkl = pAttack->m_dwAttSkl * (fSkillParam * fSkillAdd) + lSkillAdd;
				if(dwAttSkl > m_dwDefSkl)
				{
					// 技能伤害=[(自身技能攻击*技能效果－目标技能防御)*伤害浮动系数A，(自身技能攻击*技能效果－目标技能防御)*伤害浮动系数B]
					unsigned long dwDiffSkl = (unsigned long)DIFF(dwAttSkl, m_dwDefSkl);
					unsigned long dwHVMin = dwDiffSkl * 900;
					unsigned long dwHVMax = dwDiffSkl * 1100;
					dwDiffSkl = DIFF(dwHVMin, dwHVMax);
					if (dwDiffSkl)
					{
						fHurtValue = (rand() % dwDiffSkl + dwHVMin) * 0.001f;
					}
					//CCLOG("【XRole::OnHurt】技能攻击 : %f", fHurtValue);
				}
				else
				{
					//CCLOG("【XRole::OnHurt】技能攻击 : 未破防");
				}
			}
			// 暴击计算
			// 暴 击 率=(基础暴击率＋(自身暴击等级－目标暴击等级)/暴击率系数)*100%
			{
				// 基础暴击率=15%【2014-02-26，《口袋DNF》现存问题整理.xlsx】
				//if(!m_dwCritRateCoefficient)
				//	m_dwCritRateCoefficient = 1;
				// 暴击率系数=0.005%【2014-02-26，《口袋DNF》现存问题整理.xlsx】
				unsigned long dwCritOdds = (0.15f + ((float)pAttack->m_wCritLvl - (float)m_wCritLvl) / (0.005f * 0.01f)) * 10000;
				//unsigned long dwCritOdds = (pAttack->m_fCritRate + ((float)pAttack->m_wCritLvl - (float)m_wCritLvl) / pAttack->m_dwCritRateCoefficient) * 10000;
				//unsigned long dwCritMin = (unsigned long)(pAttack->m_fCritRateMax * 10000);
				//unsigned long dwCritMax = (unsigned long)(pAttack->m_fCritRateMin * 10000);
				//if(dwCritOdds < dwCritMin)
				//	dwCritOdds = dwCritMin;
				//if(dwCritOdds > dwCritMax)
				//	dwCritOdds = dwCritMax;
				// 怪物攻击固定为10%【2014-02-26，《口袋DNF》现存问题整理.xlsx】
				if(ePlayer != m_valTypeB && eHero != m_valTypeB)
					dwCritOdds = 1000;
				// 最小暴击率：1%；最大暴击率：30%【2014-02-26，《口袋DNF》现存问题整理.xlsx】
				else
				{
					if(dwCritOdds < 100)
						dwCritOdds = 100;
					if(dwCritOdds > 3000)
						dwCritOdds = 3000;
				}
				// 随机概率
				if(rand() % 10000 >= (int)dwCritOdds)
					dwCritOdds = 0;
				// 暴击倍数加成
				if(dwCritOdds)
				{
					//fHurtValue += (fHurtValue * pAttack->m_fCriMul);
					fHurtValue *= pAttack->m_fCriMul;
					bCrit = true;
					//CCLOG("【XRole::OnHurt】暴击倍数 : %f", pAttack->m_fCriMul);
				}
			}
			// 元素计算
			if(XElement::eAttFire <= pAttack->m_byElemType && pAttack->m_byElemType <= XElement::eAttDark)
			{
				int nDiff = (int)pAttack->m_awElemAttr[pAttack->m_byElemType] - (int)m_awElemAttr[pAttack->m_byElemType + 4];
				//4.3.	自己的强度－对方抗性<-50
				//	4.3.1.	最终伤害=原来的伤害*（100＋（自己的强度－对方抗性－50）/4）/100
				if(nDiff < -50)
					fHurtValue = fHurtValue * (100.f + ((nDiff - 50) >> 2)) * 0.01f;
				//4.1.	-50<=自己的强度－对方抗性<=50
				//	4.1.1.	最终伤害=原来的伤害*（100＋（自己的强度－对方抗性）/2）/100
				else if(nDiff <= 50)
					fHurtValue = fHurtValue * (100.f + (nDiff >> 1)) * 0.01f;
				//4.2.	自己的强度－对方抗性>50
				//	4.2.1.	最终伤害=原来的伤害*（100＋（自己的强度－对方抗性＋50）/4）/100
				else
					fHurtValue = fHurtValue * (100.f + ((nDiff + 50) >> 2)) * 0.01f;
			}
			if(fHurtValue < 1.f)
			{
				// 随机1-10的伤害
				int nHValue = rand() % 9 + 1;
				//CCLOG("random nHValue is [%d]", nHValue);
				fHurtValue = (float)nHValue;
			}
			// 特效
			if(m_valMap)
			{
				float fNumEffect = 0.f;
				if(pAttack)
				{
					if(pAttack != XJoystick::m_opCtrlRole && (ePlayer == pAttack->m_valTypeB || eHero == pAttack->m_valTypeB) && !m_valMap->m_byArena)
					{
						//int nType = m_valMap->GetIZType();
						//if(0 == nType || 1 == nType || 2 == nType)
						//{
						//	fNumEffect = fHurtValue;
						//}
						//else
						{
							fNumEffect = 0.f;
						}
					}
					else
					{
						fNumEffect = fHurtValue;
					}
				}
				if(eRole == m_valTypeA && ePet == m_valTypeB)
				{
					fHurtValue = 1.f;
					fNumEffect = fHurtValue;
				}
				if(!fIsZero(fNumEffect))
				{
					if(bCrit)
					{

						XNumEffect* pNumEff = XNumEffect::CreateCrit(-fNumEffect, m_fPosX, m_fPosY + m_fPosH + 100);
						if(pNumEff)
						{
							if(m_valMap)
								m_valMap->AddMapObj(pNumEff);
							else
								pNumEff->Release();
						}
					}
					else
					{
						XNumEffect* pNumEff = XNumEffect::Create(-fNumEffect, m_fPosX, m_fPosY + m_fPosH + 100);
						if(pNumEff)
						{
							if(m_valMap)
								m_valMap->AddMapObj(pNumEff);
							else
								pNumEff->Release();
						}
					}
				}
			}
			// 自己不死，一刀杀死
			if(false)
			{
				if(this == XJoystick::m_opCtrlRole)
					fHurtValue = 0.f;
				else
					fHurtValue = m_dwHp;
			}

			//CCLOG("【%s::OnHurt】HP : %d - %f", m_pRoleT->valName, m_dwHp, fHurtValue);
			if(eRole == m_valTypeA && ePet == m_valTypeB)
			{
				fHurtValue = 1.f;
			}
			else
			{
				// 减血
				if(m_dwHp > fHurtValue)
				{
					if (XJoystick::m_opCtrlRole == this || !m_valMap->m_byActivity)
					{
						m_dwHp -= (unsigned long)fHurtValue;
					}
				}
				else
				{
					if(m_dwMaxHp)
					{
						if (XJoystick::m_opCtrlRole == this || !m_valMap->m_byActivity)
						{
							m_dwHp = 0;
						}
						if(eAct_HurtFly != m_byActId)
						{
							ChangeAct(eAct_HurtFly, true);
							static XSkillT* pDeadMoveT = (XSkillT*)XTableFile::stTblSkill.GetRowData(9999, sizeof(XSkillT));
							m_pHurtMove = pDeadMoveT;
							m_bHurtDir = bDir;
							m_bHurtSky = false;
						}
						// 如果是Boss，打开慢镜头
						if(XMapObj::eBoss == m_valTypeB)
							m_bySlow = 0xFF;
						if (XJoystick::m_opCtrlRole == this)
						{
							XJoystick::m_byKeyCache = 0xFF;
							// 死后消息
							XEventManger::ExecuteEvent(XCommonEvent::IZ_ONDEAD, 0);
						}
						if (m_pRoleT && m_pRoleT->valDeadMusicID != 65535 && m_pRoleT->valMusicOdds)
						{
							//播放死亡音效
							unsigned char byOdds = rand() % 100;
							if (byOdds <= m_pRoleT->valMusicOdds)
							{
								XSoundManager::PlaySoundByID(m_pRoleT->valDeadMusicID);
							}
						}
						//xyb: 怪物死亡时候是否有掉落
						if(m_pDropSupply)
						{
							m_pDropSupply->SetPos(m_fPosX, m_fPosY, 0);
							m_pDropSupply->SetTypeC(XMapObj::eEnabled);
						}
					}
				}
				if (XJoystick::m_opCtrlRole == this || !m_valMap->m_byActivity)
				{
					// 刷新HP
					XRole* pRole = this;
					XEventManger::ExecuteEvent(XCommonEvent::HP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
				}
				if (this->GetTypeB() == XMapObj::eBoss && m_valMap->m_byActivity && pEffect->GetOwner() == XJoystick::m_opCtrlRole)
				{
					m_valMap->m_dwHurtNum += (unsigned long)fHurtValue;
				}
			}
		}
		// 此时是否是霸体（可能产生其他bug）
		if (!byHurtBreak || !m_dwHp)
		{
			// Move
			Stop();
			// HitStop
			if(XMapObj::eWood != m_valTypeB)
			{
				if(pST->valHitStopH)
					Pause(pST->valHitStopH);
				// 受创特效
				if(0xFFFF != pST->valHitEff)
				{
					XEffect* pEffect = XMP_CREATE(XEffect);
					if(pEffect)
					{
						bool bBind = pEffect->SetSkillID(pST->valHitEff, this);	// 后期改为配置文件
						if(bBind)
							m_stEffect.AddObj(pEffect);
						else
						{
							if(m_valMap)
								m_valMap->AddMapObj(pEffect);
							else
								pEffect->Release();
						}
					}
				}
			}
		}
		// 木桶类无击飞
		if(XMapObj::eWood == GetTypeB())
		{
			m_pHurtMove = NULL;
			m_bHurtDir =  true;
			m_bHurtSky = false;
		}
		// 此时是否是霸体（可能产生其他bug）
		if (!byHurtBreak || !m_dwHp)
		{
			m_byCmdTypeA = eCmdA_Hurt;
			m_byCmdTypeB = eCmdB_None;
		}

		if(m_pHurtMove)
		{
			if((m_pHurtMove->valDisH && !m_bHurtSky) ||
				(m_pHurtMove->valSkyDisH && m_bHurtSky))
				m_fPosH += 1.f;
		}

		//if(m_valMap && this == m_valMap->GetControl())
		//{
		//	CCLOG("【OnHurt】Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
		//		m_byCmdTypeA, m_byCmdTypeB, m_byActId, m_fPosX, m_fPosY, m_fSpdX, m_fSpdY);
		//}
		return true;
	} while (false);
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::OnHit(XEffect* pEffect, XRole* pHurt)
{
	XFUNC_START();
	do 
	{
		if(!pEffect)
			break;
		XEffect::XEffInfo* pInfo = pEffect->GetInfo();
		if(!pInfo)
			break;
		XSkillT* pST = pInfo->pSkillT;
		if(!pST)
			break;
		if(pHurt)
		{
			if(XMapObj::eSwitch == pHurt->m_valTypeB || XMapObj::eSupply == pHurt->m_valTypeB)
				break;
			// 技能音效
			if(0xFFFF != pST->valHitMusicID)
			{
				XSoundManager::PlaySoundByID(pST->valHitMusicID);
			}
			if(XMapObj::eWood == pHurt->m_valTypeB)
				break;
		}
		// HitStop
		if(pInfo->byBind)
		{
			if(pST->valHitStopA)
				Pause(pST->valHitStopA);
		}
	} while (false);
	XFUNC_END();
}

void XRole::RG_Run()
{
	XFUNC_START();
	if(0xFF == m_stGhosting.byFrameUsedIdx)
		return;
	// 透明度衰减
	bool bExist = true;
	for(unsigned short wPos = 0; wPos < ROLE_GHOTSING_MAX; ++wPos)
	{
		XRoleGhosting::XRGFrame& stRGF = m_stGhosting.astFrames[wPos];
		if(0xFF != stRGF.byActId)
		{
			if(stRGF.byAlpha > stRGF.byAlphaSubPF)
			{
				stRGF.byAlpha -= stRGF.byAlphaSubPF;
				bExist = false;
			}
			else
				stRGF.byActId = 0xFF;
		}
	}
	if(bExist)
		m_stGhosting.byFrameUsedIdx = 0xFF;
	XFUNC_END();
}

void XRole::RG_OnDraw()
{
	XFUNC_START();
	if(0xFF == m_stGhosting.byFrameUsedIdx)
		return;
	// 获取渲染坐标偏移
	float fDX = 0.f;
	float fDY = 0.f;
	XCamera::Instance().GetRenderPos(fDX, fDY);
	static cocos2d::ccColor3B stColor = cocos2d::ccc3(0xFF, 0xFF, 0xFF);
	static cocos2d::ccBlendFunc stBFunc = { GL_SRC_ALPHA, GL_ONE };
	static cocos2d::XShaderSoul stShaderSoul;
	static cocos2d::XGLProgram* pGLP = stShaderSoul.GetXProgram();
	static unsigned short wW, wH;
	bool bFlipX;
	XBIT_CLR(m_achEffMark, eEff_Break);
	for(unsigned short wPos = 0; wPos < ROLE_GHOTSING_MAX; ++wPos)
	{
		XRoleGhosting::XRGFrame& stRGF = m_stGhosting.astFrames[wPos];
		// 无效的和本帧刚添加的记录,不渲染
		if(0xFF == stRGF.byActId || 0xFF == stRGF.byAlpha)
			continue;
		// 渲染
		stShaderSoul.SetAlpha(stRGF.byAlpha);
		m_astAnimation[stRGF.byActId].GetFrameSize(wW, wH);
		stShaderSoul.SetTexRect(wW, wH);
		m_astAnimation[stRGF.byActId].SetDrawFrameOnce(stRGF.wActFrame);
		if(XBIT_VALUE(&stRGF.byMark, XRoleGhosting::XRGFrame::eHurt))
			stShaderSoul.SetParamf2(1.f);
		else if(XBIT_VALUE(&stRGF.byMark, XRoleGhosting::XRGFrame::eNoBreak))
		{
			stShaderSoul.SetParamf2(0.5f);
			XBIT_MARK(m_achEffMark, eEff_Break);
		}
		else
			stShaderSoul.SetParamf2(0.f);
		if(XBIT_VALUE(&stRGF.byMark, XRoleGhosting::XRGFrame::eFlipX))
			bFlipX = true;
		else
			bFlipX = false;
		m_astAnimation[stRGF.byActId].OnDrawEx(stRGF.wPosX + fDX, stRGF.wPosY + fDY, m_fScale, m_fScale, stColor,
			stRGF.byAlpha, stBFunc, bFlipX, pGLP);
	}
	XFUNC_END();
}

void XRole::OnException(const char* szFile, const int nLine, const char* szInfo) {
	CCLOG("[XRole::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
}

bool XRole::OnCreate()
{
	XFUNC_START();
	Reset();
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::OnRelease()
{
	XFUNC_START();
	DelRoleEffect();
	DelRoleEquipEffect();
	// 释放头顶的特效
	if (m_pRoleHead)
	{
		m_pRoleHead->Cleanup();
		m_pRoleHead = NULL;
	}
	m_pElfAtt = NULL;
	m_pElfDef = NULL;
	XFUNC_END();
}

void XRole::Cleanup()
{
	XFUNC_START();
	//CCLOG("XRole::Cleanup %s[%p]", _stXStrCnvMainTh.XUNICODE_TO_GB2312(GetRoleTableName()), this);
	Release();
	if (XMemPool<XRole>::m_pXMPParent)
	{
		XRole* pRole = this;
		try
		{
			XMP_RELEASE(XRole, pRole);
		}
		catch (...)
		{

		}
	}
	XFUNC_END();
}

void XRole::Initialize()
{
	XFUNC_START();
	memset(m_obySkillUse, 0xFF, sizeof(unsigned char) * XPD_SKILL_NUM);
	tinyxml2::XMLDocument stXmlDocument;
	tinyxml2::XMLError eRes = stXmlDocument.LoadFile(XFILE_ROLE);
	if(tinyxml2::XML_SUCCESS != eRes)
	{
		CCLOG("[ERROR]XRole::Initialize load file %s failed!", XFILE_ROLE);
		return;
	}
	int nStep = 0;
	do 
	{
		tinyxml2::XMLElement *pRoles = stXmlDocument.RootElement();
		if(!pRoles)
			break;
		++nStep;
		int nValue;
		unsigned short wRoleID;
		tinyxml2::XMLElement* pRole = pRoles->FirstChildElement("Role");
		while(pRole)
		{
			eRes = pRole->QueryIntAttribute("id", &nValue);
			if(tinyxml2::XML_SUCCESS != eRes)
			{
				CCLOG("[ERROR]XRole::Initialize check file %s 'id' failed!", XFILE_ROLE);
				return;
			}
			if(nValue < 0 || nValue >= ROLE_TYPE_NUM)
			{
				CCLOG("[ERROR]XRole::Initialize check file %s 'id' %d failed!", XFILE_ROLE, nValue);
				return;
			}
			wRoleID = (unsigned short)nValue;
			// 属性
			m_oastRoleInfo[wRoleID].pRoleT = (XRoleT*)XTableFile::stTblRole.GetRowData(wRoleID, sizeof(XRoleT));
			if(!m_oastRoleInfo[wRoleID].pRoleT)
			{
				CCLOG("[ERROR]XRole::Initialize check file %s 'id' %d table failed!", XFILE_ROLE, wRoleID);
				return;
			}
			//eRes = pRole->QueryIntAttribute("spdWalk", &nValue);
			//if(tinyxml2::XML_SUCCESS == eRes)
			//	m_oastRoleInfo[wRoleID].m_wSpdWalk = (unsigned short)nValue;
			//eRes = pRole->QueryIntAttribute("spdRun", &nValue);
			//if(tinyxml2::XML_SUCCESS == eRes)
			//	m_oastRoleInfo[wRoleID].m_wSpdRun = (unsigned short)nValue;
			//eRes = pRole->QueryIntAttribute("spdJmp", &nValue);
			//if(tinyxml2::XML_SUCCESS == eRes)
			//	m_oastRoleInfo[wRoleID].m_wSpdJump = (unsigned short)nValue;
			// 查询动作
			tinyxml2::XMLElement* pAct = pRole->FirstChildElement("Act");
			while(pAct)
			{
				eRes = pAct->QueryIntAttribute("id", &nValue);
				if(tinyxml2::XML_SUCCESS != eRes)
				{
					CCLOG("[ERROR]XRole::Initialize check file %s 'id' %d act failed!", XFILE_ROLE, wRoleID);
					return;
				}
				if(0 <= nValue && nValue < ROLE_ACT_NUM)
				{
					XRoleInfo::XActInfo& stAI = m_oastRoleInfo[wRoleID].astActInfo[nValue];
					eRes = pAct->QueryIntAttribute("aniId", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stAI.wAniId = (unsigned short)nValue;
				}
				else
				{
					CCLOG("[ERROR]XRole::Initialize check file %s 'id' %d act %d failed!", XFILE_ROLE, wRoleID, nValue);
					return;
				}
				pAct = pAct->NextSiblingElement();
			}
			pRole = pRole->NextSiblingElement();
		}
		return;
	} while (false);
	CCLOG("[ERROR]XRole::Initialize load file %s with step[%d]!", XFILE_ROLE, nStep);
	XFUNC_END();
}
extern unsigned char g_byLoadPerc;
unsigned long XRole::OnUIRoleFrame(bool bUseSkill)
{
	XFUNC_START();
	if(!Valid())
		return 0;

	if(m_byActId >= ROLE_ACT_NUM)
		return 0;

	if(0x64 > g_byLoadPerc)
		return 0;

	if (bUseSkill)
	{
		static unsigned char byFrames = 0;
		if(eAct_WaitInCity == m_byActId)
		{
			ChangeAct(eAct_Wait, true);
			byFrames = 0;
		}
		if(eAct_Wait == m_byActId)
		{
			++byFrames;
			if(byFrames > 30)
				byFrames = 0;
			if(byFrames == 30)
				UseSkill(XRole::eSkl_Normal);
		}
		if(0xFF == m_byMarTypeNext && SkillAct())
		{
			if(eAct_AttNor_3 > m_byActId)
			{
				UseSkill(XRole::eSkl_Normal);
			}
		}
	}
	OnFrame();
	//// 动作
	//ActionFrame();
	//// 特效OnFrame
	//XEffect* pEffect = m_stEffect._pHead;
	//XEffect* pRelease;
	//while (pEffect)
	//{
	//	pRelease = pEffect;
	//	pEffect = pEffect->_pNext;
	//	{
	//		pRelease->OnFrame();
	//		if(pRelease->NeedRemove())
	//		{
	//			m_stEffect.DelObj(pRelease);
	//			pRelease->Cleanup();
	//			pRelease = NULL;
	//		}
	//	}
	//}
	return 0;
	XFUNC_END_RETURN(unsigned long, res, 0xFFFFFFFF);
}
void XRole::OnUIRoleDraw(float fX, float fY, float fScale /* = 1.f */, unsigned char byAlpha /* = 0xFF */ )
{
	XFUNC_START();
	if(!Valid())
		return;
	if(m_byActId >= ROLE_ACT_NUM)
		return;
	SetPosition(fX, fY, 0);
	OnDraw();
	XFUNC_END();
}

bool XRole::InitializeShodow( const char* szShadowPath )
{
	XFUNC_START();
	if (m_pShadowSprite)
	{
		return true;
	}
	if(!szShadowPath || !szShadowPath[0])
		return false;
	m_pShadowSprite = cocos2d::XSprite::createWithSpriteFrameName(szShadowPath);
	if(!m_pShadowSprite)
	{
		CCLOG("[%s][%d]create faild!", __XFUNCNAME__, 0);
		return false;
	}
	m_pShadowSprite->setAnchorPoint(ccp(0.5f, 0.5f));

	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::DelRoleEffect()
{
	XFUNC_START();
	if(m_pRoleEffect)
	{
		m_pRoleEffect->DelGameEffect();
		m_pRoleEffect = NULL;
	}
	m_wEffectId = 0xFFFF;
	XFUNC_END();
}

void XRole::SetEffectId(unsigned short wEffectId)
{
	XFUNC_START();
	DelRoleEffect();
	if(0xFFFF != wEffectId)
	{
		m_pRoleEffect = XEffect::CreateGameEffect(0.f, 0.f, wEffectId);
	}
	XFUNC_END();
}

void XRole::DelRoleEquipEffect()
{
	XFUNC_START();
	if(m_pRoleEquipEffect)
	{
		m_pRoleEquipEffect->DelGameEffect();
		m_pRoleEquipEffect = NULL;
	}
	m_wEquipEffectId = 0xFFFF;
	XFUNC_END();
}

void XRole::SetEquipEffectId(unsigned short wEffectId)
{
	XFUNC_START();
	DelRoleEquipEffect();
	if(0xFFFF != wEffectId)
	{
		m_pRoleEquipEffect = XEffect::CreateGameEffect(0.f, 0.f, wEffectId);
		m_wEquipEffectId = wEffectId;
	}
	XFUNC_END();
}

float XRole::GetIZFinishTick()
{
	XFUNC_START();
	if (m_byMapType == XMap::eType_IZ)
	{
		if (m_valMap)
		{
			return ((XIZMap*)m_valMap)->GetIZFinishTick();
		}	
	}
	return 0.f;
	XFUNC_END_RETURN(float, res, 0.f);
}

#define  SKILL_NEED_MP(bySklPos)	\
	dwMP = pSkillLvlT->valMpNeed_##bySklPos; \

bool XRole::SpendSkillMP(unsigned char bySklPos, bool bIsSpend)
{
	XFUNC_START();
#if defined(WIN32) && defined(_WINDOWS)
	unsigned char byStates = XGameStateManger::GetCurrState();
#ifdef X_SINGLE
	if (enState_Single == byStates)
	{
		return true;
	}
#endif

#endif
	if(bySklPos < XPD_SKILL_NUM)
	{
		if (m_pRoleT)
		{
			unsigned long dwSklLvlId = m_pRoleT->valSklLvlId + GetSkillLvl(bySklPos) - 1;
			XSkillLvlT* pSkillLvlT = (XSkillLvlT*)XTableFile::stTblSklLvl.GetRowData(dwSklLvlId, sizeof(XSkillLvlT));
			if (pSkillLvlT)
			{
				unsigned long dwMP = 0;
				switch(bySklPos)
				{
				case 0:
					SKILL_NEED_MP(1);
					break;
				case 1:
					SKILL_NEED_MP(2);
					break;
				case 2:
					SKILL_NEED_MP(3);
					break;
				case 3:
					SKILL_NEED_MP(4);
					break;
				default:
					break;
				}
				if (m_dwMp >= dwMP)
				{
					if (bIsSpend)
					{
						m_dwMp -= dwMP;
						// 刷新MP
						XEventManger::ExecuteEvent(XCommonEvent::MP_CHANGE, 0);
					}
					return true;
				}
				else
				{
					if (!bIsSpend)
					{
						XEventManger::ExecuteEvent(XCommonEvent::MP_NOTENOUGH, 0);
					}
					return false;
				}
			}				
		}			
	}
	
	return false;
	XFUNC_END_RETURN(bool, res, false);
}

void XRole::Revive()
{
	XFUNC_START();
	if (IsDead())
	{
		m_byCmdTypeA = eCmdA_Dead;
		m_byCmdTypeB = eCmdB_Relive_Soul;
		m_byActId = eAct_Down;
		m_astAnimation[eAct_Down].ToEndFrame(true);
		// 用于渲染灵魂
		m_astAnimation[eAct_HurtDown].ToEndFrame(true);
		XRole::m_byIsRevive = 0xFF;
	}
	XFUNC_END();
}

void XRole::Dead()
{
	XFUNC_START();
	if (!IsDead())
	{
		m_byCmdTypeA = eCmdA_Hurt;
		m_byCmdTypeB = eCmdB_None;
		m_dwHp = 0;
		ChangeAct(XRole::eAct_HurtFly, true);
		//if(m_pRoleT)
		//{
		//	CCLOG("【XRole::Dead】 %s", GetRoleTableName());
		//}
		//else
		//{
		//	CCLOG("【XRole::Dead】 m_pRoleT is NULL");
		//}
	}
	XFUNC_END();
}

void XRole::Disappear()
{
	XFUNC_START();
	//if (!IsDead())
	//{
		m_byCmdTypeA = eCmdA_Disappear;
		m_byCmdTypeB = eCmdB_None;
		m_byDeadAlpha = 0xFF;
		if(m_pRoleT)
		{
			CCLOG("【XRole::Disappear】 %s", GetRoleTableName());
		}
		else
		{
			CCLOG("【XRole::Disappear】 m_pRoleT is NULL");
		}
	//}
	XFUNC_END();
}

void XRole::ForceWait()
{
	XFUNC_START();
	m_byCmdTypeB = eCmdB_None;
	Wait(true);
	//CCLOG("【XRole::ForceWait】 %s", m_pRoleT->valName);
	XFUNC_END();
}
