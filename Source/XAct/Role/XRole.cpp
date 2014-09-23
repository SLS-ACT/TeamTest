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

		// ����CD
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
			// ���س��ж���
			for(int nPos = eAct_Wait; nPos < eAct_CityAct; ++nPos)
				m_astAnimation[nPos].AddRef();
			for(int nPos = eAct_WaitInCity; nPos < eAct_CityActExt; ++nPos)
				m_astAnimation[nPos].AddRef();
			// ����NPC��С����
			if(eCity == m_valTypeB)
				m_astAnimation[10].AddRef();
		} break;
	case eActType_All:
		{
			// �������ж���
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
	//	CCLOG("��OnFrame��Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
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
				m_dwMp += (3 + m_wReMp);		// �츳����Ӱ�������ָ�
				if (m_dwMp > m_dwMaxMp)
				{
					m_dwMp = m_dwMaxMp;
				}
				// ˢ��MP
				XEventManger::ExecuteEvent(XCommonEvent::MP_CHANGE, 0);
			}
		}
	}
	// �޵�ʱ��
	if(m_byInvincible)
		--m_byInvincible;
	bool bPause = IsPause();
	// ����
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
			// �����ʧҲ�ߵ�������ǲ�֪�����л�����ɾ������������ߣ���������ʱ��������ͱ��NPC�Ա���ɾ��
			m_valTypeB = eCity;
			m_valTypeC = eEnabled;
			// ��ʱ��ӽ�ɫ��ʧ�¼�
			if(ePlayer != m_valTypeB && ePet != m_valTypeB && eHero != m_valTypeB && 2 != XRole::m_byDeadAlphaOffSet)
				XEventManger::ExecuteEvent(XCommonEvent::ROLE_DISAPPEAR, 0);
		}
		return dwRun;
	}
	// ��
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
		// �����ǰ״̬Ϊ�ȴ����ٶ���Ϊ0
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
		// �����������Դ��
		if(m_byCmdTypeA < cCmdA_Active)
		{
			// �Ƿ��м���
			if(0xFF != m_byMarTypeNext)
			{
				// ���ܲ���������Ϲ��� : ���ܴ�ϼ���|���ܴ��GC
				bool bBreak = false;
				if(eSkl_Up <= m_byMarTypeNext && m_byMarTypeNext <= eSkl_Back)
				{
					if(m_astAnimation[m_byActId].IsCanBreak())
						bBreak = true;
				}
				// ������ɻ��߲��Ǽ��ܶ���ʱ
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
						// ��������ʱ�򣬲��ܳ����ܡ�
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
					// Cmd : ��������ı�ɹ�
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
				// ���������Ϊ���ܶ���ʱ
				if(ActionEnd() && SkillAct())
				{
					// ������ʽ������ϣ��������߼�
					if (!m_dwHp && m_dwMaxHp && m_byDeadActID != 0xFF)
					{
						if (m_byDeriveType != 0xFF && m_pDeriveRole)
						{
							// �������������
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
						// ��������
						m_byCmdTypeA = eCmdA_Dead;
						m_byCmdTypeB = eCmdB_Dead_End;
					}
					else
					{
						m_byCmdTypeB = eCmdB_None;
						Wait(true);
						// ���н�����ԭ֮ǰ��ҡ�˲���
						if(eCmdA_Wait == m_byCmdTypeA && eCmdA_Wait != m_stCmdLast.byTypeA && eCmdA_Skill != m_stCmdLast.byTypeA)
						{
							XRoleCmd stCmd = m_stCmdLast;
							m_stCmdLast.byTypeA = eCmdA_Move;
							m_stCmdLast.byTypeB = eCmdB_Move_Run;
							RecvCmd(stCmd);
						}
					}
				}
				// ���ʱ
				if(eCmdA_Jump == m_byCmdTypeA)
				{
					m_byCmdTypeB = eCmdB_None;
					Wait(true);
				}
				// ����Ŀ�ĵ�ʱ
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
		// ��������
		else
		{
			// ���ɶ�������غ���ɵ�
			if(eAct_HurtFly == m_byActId)
			{
				if(fIsZero(m_fPosH) && fIsZero(m_fSpdH))
				{
					Stop();
					if(!m_dwHp && m_dwMaxHp && (m_byDeadActID != 0xFF || m_pDeriveRole))
					{
						// ����������ʽ
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
							// �������������
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
							// ��������
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
				// ���
				if(ActionEnd())
				{
					// �ɵؽӵ�������
					if(eAct_Down == m_byActId)
					{
						// �Ƿ�����
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
											// ��Ҹ���
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
										// ˢ��MP
										XRole* pRole = this;
										XEventManger::ExecuteEvent(XCommonEvent::HP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
										// ˢ��MP
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
								// �Ƿ�������
								if(m_pRoleT)
								{
									unsigned char bySoul = rand() % 100;
									if(bySoul < m_pRoleT->valSoul)
									{
										m_byCmdTypeB = eCmdB_Dead_Soul;
										// ������Ⱦ���
										m_astAnimation[eAct_HurtUp].ToEndFrame(true);
									}
								}
							}
						}
					}
					// ��������Ҫ���AI
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
						//	// ����л���
						//	if(m_pHurtMove->valDisH)
						//		ChangeAct(eAct_HurtFly, true);
						//	// �����ӵȴ�
						//	else
						//		Wait(true);
						//}
						//// �����ӵȴ�
						//else
						// ��ʱ�Ƿ��ǰ��壨���ܲ�������bug��
						unsigned char byHurtBreak = m_astAnimation[m_byActId].IsCanHurtBreak();
						if (!byHurtBreak)
						{
							Wait(true);
						}
					}
					// �ܴ�������ԭ֮ǰ��ҡ�˲���
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
			// �������ٶ�
			if(fIsZero(m_fAcceH) && m_fPosH > 0.f)
				m_fAcceH = m_ofGravity;
			// ����Ǽ��ܣ�λ�Ʋ���
			if(SkillAct() && eAct_AttJump != m_byActId)
			{
				// ����λ�Ʋ����߼�
				SetMovScaleX(m_ofMovScale);
				SetMovScaleY(m_ofMovScale);
			}
			else
			{
				SetMovScaleX(1.f);
				SetMovScaleY(1.f);
			}
		}
		// λ��
		if(XMapObj::m_bySlow)
		{
			static float fT = SPF * XMapObj::m_fSlowVal;
			RunMove(fT);
		}
		else
			RunMove(SPF);
		// �ö����Ƿ�Ϊ����
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
						// ��ʱ��ֹH�ƶ�
						StopH();
						// �Ϸ�
						m_fPosH += wFlySpd;
						if(m_fPosH > wFlyH)
							m_fPosH = wFlyH;
					}
				}
			}
		}
		// ����
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
	// ��ЧOnFrame
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
	// �����¼�
	if(m_byActId < ROLE_ACT_NUM)
	{
		// ������Ч : ÿ֡һ��
		unsigned short wEffID = 0xFFFF;
		// ��ɫ����
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
					// ����λ��
					if(m_valFlipX)
						m_fPosX -= pFrmInfo->nMoveX;
					else
						m_fPosX += pFrmInfo->nMoveX;
					m_fSpdH += pFrmInfo->nSpdH;
					for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
					{
						// ������Ч
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
					// ���� 
					if(0xFFFF != pFrmInfo->wSoundID)
					{
						//CCLOG("[ERROR]XDrawAnimation::OnDraw 0xFFFF == stFI.wDrawTexID!");
						XSoundManager::PlaySoundByID(pFrmInfo->wSoundID);
					}
					// ��ͷ��Ч
					if (0xFFFF != pFrmInfo->wCameraID)
					{
						XCamera::Instance().Effect(pFrmInfo->wCameraID);
					}
					// ������Ӱ
					if(pFrmInfo->byStay)
						RG_Used(pFrmInfo->byStay);
				}
			}
			// �ܴ�λ��
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
							// ֻ�������ܴ������ܴ��в�������λ��
							if(eAct_HurtUp == m_byActId || eAct_HurtDown == m_byActId)
							{
								// ÿ֡��λ��
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
							//// ��û���������ȵ���˵
							//if(eAct_HurtUp == m_byActId)
							//	break;
							//// ��������
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
							// ֻ�������ܴ������ܴ��в�������λ��
							if(eAct_HurtUp == m_byActId || eAct_HurtDown == m_byActId)
							{
								// ÿ֡��λ��
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
		// ��ײ
		if(m_valMap && pFrmInfo && (pFrmInfo->nRcX || pFrmInfo->nRcY || pFrmInfo->nRcH || pFrmInfo->nRcW || pFrmInfo->byThickness))
		{
			// �����о��鲻ע��
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
		// ����
		m_byMapMove = 0x00;
	}
	RG_Run();
	// ��ɫ��Ч
	if(m_pRoleEffect)
		m_pRoleEffect->AnimateFrame();
	// ��װ��Ч
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
	// AlphaΪ��ǰ
	// λ��Ϊ[X, Y + 0xFF - Alpha]
	// ����Ϊ���ܴ�����
	// ƫɫΪ����
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
	// ������û��Ӱ��
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
	// ��ҽŵ���Ч
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
	// ��ЧOnDraw������ɫ��ס��
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
	// ��
	fPosX += m_chHurtShake;

	////fPosX = (float)((int)fPosX);
	////fPosY = (float)((int)fPosY);
	////fPosG = (float)((int)fPosG);
	// ��Ⱦ��ɫ
	if(!m_byColorFrame)
	//if (!IsPause())
	{
		m_stColor = m_stNoColor;
	}
	cocos2d::XGLProgram* pGLPShader = NULL;
	//// ����ͷ�ڰ�
	//if(XMapObj::m_bySlow)
	//{
	//	static cocos2d::XShaderWhite stShaderWhite;
	//	pGLPShader = stShaderWhite.GetXProgram();
	//	const float fC = 1.f / 255.f;
	//	stShaderWhite.SetParamf1((float)m_byDeadAlpha * fC);
	//}
	//else
	//{
		// ������Ч
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
	// ��ɫ��Ⱦ
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
		// ��Ӱ��Ⱦ
		RG_OnDraw();
		// �����Ⱦ
		if(cocos2d::XScrSprite::eShaderNone == cocos2d::XScrSprite::GetShaderType())
		{
			OnDrawSoul();
		}
		// ��װ��Ч
		if(m_pRoleEquipEffect)
		{
			if(!bIsMe || !IsDead())
			{
				m_pRoleEquipEffect->OnGameEffectDraw(fPosX, fPosY);
			}
		}
		//����
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
		// ��װ��Ч
		if(m_pRoleEquipEffect)
		{
			if(!bIsMe || !IsDead())
			{
				m_pRoleEquipEffect->OnGameEffectDraw(fPosX, fPosY);
			}
		}
	}

	// ��ЧOnDraw����ס��ɫ��
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
		// λ����
		glLineWidth(1);
		cocos2d::ccDrawColor4B(0x00, 0x00, 0xFF, 0xFF);
		cocos2d::ccDrawLine(ccp(fPosX, fPosY), ccp(fPosX, fPosY + 100.f));
	}
#	endif
	if(m_pRoleEffect)
	{
		// ����Լ�
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
	// �����Ч
	XEffect* pEffect = m_stEffect._pHead;
	XEffect* pRelease;
	while (pEffect)
	{
		pRelease = pEffect;
		pEffect = pEffect->_pNext;
		pRelease->Cleanup();
	}
	m_stEffect.Clear();
	// �ͷŶ���
	LoadAction(eActType_None);
	XFUNC_END();
}

void XRole::SetRoleName(const char* szRoleName)
{
	XFUNC_START();
	if(!szRoleName || !szRoleName[0])
		return;
	//sprintf(m_szRoleName, "%s", szRoleName);
	//CCLOG("��������ɫ��%s", m_szRoleName);
	//const char* szUtf8Name = _stXStrCnvMainTh.XGB2312_TO_UTF8(szRoleName);
	if(!szRoleName || !szRoleName[0])
		return;
	if(!m_pRoleHead)
	{
		m_pRoleHead = XHeadEffect::ToLabel(XHeadEffect::Create());//cocos2d::CCLabelTTF::create(szUtf8Name, "����", 24);
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
	////CCLOG("��������ɫ��%s", szRoleName);
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
		m_pRoleHead = XHeadEffect::ToLabel(XHeadEffect::Create());//cocos2d::CCLabelTTF::create(szUtf8Name, "����", 24);
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
	// AIʱ����Ӧ����
	if(ePAI_None != m_byPlayerAI)
		return;
	// �ƶ�ʽ����Ӧ��ͬ����
	if(eCmdA_Move == stCmd.byTypeA && m_stCmdLast == stCmd)
		return;
	// ���˸��� | ս��NPC ��ֱ����Ӧ
	// ...
	{
		RecvCmd(stCmd);
	}
	// ���У�ֱ����Ӧ�ƶ�����
	// ...
	// ���˸��������͸�������
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
	case eCmdA_Wait:	{ m_byCmdTypeB = eCmdB_None; Wait(); } break; // ����ͣ�������ͣ
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
	// ������������
	if(m_valMap && eCmdA_Wait != stCmd.byTypeA)
	{
		if(this == m_valMap->GetControl())
			XEventManger::ExecuteEvent(XCommonEvent::GUILD_MOVEEND, 0);
	}
	//if(m_valMap && this == m_valMap->GetControl())
	//{
	//	CCLOG("��RecvCmd��Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
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
			// CmdAΪ������Ϊʱ��ֻҪû��ǿ�ƾͷ���
			if(m_byCmdTypeA < cCmdA_Active)
				return;
			// CmdAΪ������Ϊʱ��ֻҪû�в�����ɾͷ���
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
		// �����MoveTo�����������ͣ
		if(eCmdA_MoveTo == m_byCmdTypeA)
			m_byCmdTypeB = eCmdB_None;
		// ���û����ͣ
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
	// ����Ǳ������������ɴ��
	if(m_byCmdTypeA >= cCmdA_Active)
		return;
	if(bRun)
		XRenderObj::MoveTo(wPosX, wPosY, m_pRoleT->valSpdRun * m_wSpdScale * 0.01f);
	else
		XRenderObj::MoveTo(wPosX, wPosY, m_pRoleT->valSpdWalk * m_wSpdScale * 0.01f);
	// �������Ծ�л��ܣ����ı䶯��
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
	// ����Ǳ������������ɴ��
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
	// �������Ծ�л��ܣ����ı䶯��
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
	// ����Ǳ����������ܣ����ɴ��
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
	// ��¼��һ���ܣ��������������ֱ���滻֮ǰ�ļ�¼
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
		//CCLOG("��XRole::OnHurt�� %s", m_pRoleT->valName);
		//if(m_valMap && m_valMap->GetControl() == this)
		//	m_dwHp = 0xFFFF;
		if(!m_dwHp && (eCmdA_Hurt == m_byCmdTypeA || eCmdA_Dead == m_byCmdTypeA))
		{
			//CCLOG("role is dead, no hurt.");
			break;
		}
		// ��������ʱ�����λ�ñ��볬����������Żἤ�����AI��������Ϊ�˷�ֹ������������������������Զ�̹���
		if(0xFF != m_obyIZStart)
		{
			if(ePlayer != m_valTypeB && eHero != m_valTypeB && ePet != m_valTypeB)
			{
				CCLOG("��XRole::OnHurt�� �������AI");
				m_obyIZStart = 0xFF;
			}
		}

		if(!pEffect)
		{
			//CCLOG("effect is not exist, no hurt.");
			break;
		}

		// ��Ч����
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
		// ����
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
		// ľͰ���޻ζ�
		//if(XMapObj::eWood != GetTypeB())
		{
			// Shake
			if(ROLE_HURTSHAKE_PIX == m_chHurtShake)
				m_chHurtShake = -ROLE_HURTSHAKE_PIX;
			else
				m_chHurtShake = ROLE_HURTSHAKE_PIX;
		}
		// ��ɫ
		{
			m_stColor = *((cocos2d::ccColor3B*)&(pST->valHitColor));
			unsigned char byTmp = m_stColor.r;
			m_stColor.r = m_stColor.b;
			m_stColor.b = byTmp;
			m_byColorFrame = 10;
		}
		
		// ��ʱ�Ƿ��ǰ��壨���ܲ�������bug��
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
				//// ����ʱ�������ܴ�
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
				// �����ǰ�����ܴ�
				if(eAct_HurtUp == m_byActId)
					ChangeAct(eAct_HurtDown, true);
				// �����ǰ�����ܴ�
				else if(eAct_HurtDown == m_byActId)
					ChangeAct(eAct_HurtUp, true);
				// ���������ͨ�ܴ�
				else if(rand() % 2)
					ChangeAct(eAct_HurtUp, true);
				else
					ChangeAct(eAct_HurtDown, true);
			}
		}
		// �Ƿ񱩻�
		bool bCrit = false;
		// ����
		XRole* pAttack = pEffect->GetOwner();
		if(pAttack)
		{
			float fHurtValue = 0.f;
			unsigned char bySklPos = pEffect->GetSkillPos();
			unsigned char bySklLvl = pAttack->GetSkillLvl(bySklPos);
			// �ǲ��Ǽ���
			if(0xFF == bySklPos)
			{
				// ��ͨ���� : �Ƿ��Ʒ�
				if(pAttack->m_dwAttPhy > m_dwDefPhy)
				{
					// �չ��˺�=[(������������Ŀ���������)*�˺�����ϵ��A��(������������Ŀ���������)*�˺�����ϵ��B]
					unsigned long dwDiffPhy = DIFF(pAttack->m_dwAttPhy, m_dwDefPhy);
					unsigned long dwHVMin = dwDiffPhy * 900;
					unsigned long dwHVMax = dwDiffPhy * 1100;
					dwDiffPhy = DIFF(dwHVMin, dwHVMax);
					if (dwDiffPhy)
					{
						fHurtValue = (rand() % dwDiffPhy + dwHVMin) * 0.001f;
					}
					// Ŀ���������Ϊ0�Ĵ���
					else
					{
						fHurtValue = (dwHVMin) * 0.001f;
					}
					//CCLOG("��XRole::OnHurt����ͨ���� : %f", fHurtValue);
				}
				else
				{
					//CCLOG("��XRole::OnHurt����ͨ���� : δ�Ʒ�");
				}
			}
			// ����ѧ��û��
			else if(0x00 != bySklLvl)
			{
				float fSkillAdd = 1.f;
				long lSkillAdd = 0;
				XRoleT* pAttRoleT = pAttack->m_pRoleT;
				unsigned char byType = pAttack->GetTypeB();
				// ��Ҳ��мӳ�
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

				// ���ܵ��˺�ϵ��
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
				// ���ܹ��� : �Ƿ��Ʒ�
				unsigned long dwAttSkl = pAttack->m_dwAttSkl * (fSkillParam * fSkillAdd) + lSkillAdd;
				if(dwAttSkl > m_dwDefSkl)
				{
					// �����˺�=[(�����ܹ���*����Ч����Ŀ�꼼�ܷ���)*�˺�����ϵ��A��(�����ܹ���*����Ч����Ŀ�꼼�ܷ���)*�˺�����ϵ��B]
					unsigned long dwDiffSkl = (unsigned long)DIFF(dwAttSkl, m_dwDefSkl);
					unsigned long dwHVMin = dwDiffSkl * 900;
					unsigned long dwHVMax = dwDiffSkl * 1100;
					dwDiffSkl = DIFF(dwHVMin, dwHVMax);
					if (dwDiffSkl)
					{
						fHurtValue = (rand() % dwDiffSkl + dwHVMin) * 0.001f;
					}
					//CCLOG("��XRole::OnHurt�����ܹ��� : %f", fHurtValue);
				}
				else
				{
					//CCLOG("��XRole::OnHurt�����ܹ��� : δ�Ʒ�");
				}
			}
			// ��������
			// �� �� ��=(���������ʣ�(�������ȼ���Ŀ�걩���ȼ�)/������ϵ��)*100%
			{
				// ����������=15%��2014-02-26�����ڴ�DNF���ִ���������.xlsx��
				//if(!m_dwCritRateCoefficient)
				//	m_dwCritRateCoefficient = 1;
				// ������ϵ��=0.005%��2014-02-26�����ڴ�DNF���ִ���������.xlsx��
				unsigned long dwCritOdds = (0.15f + ((float)pAttack->m_wCritLvl - (float)m_wCritLvl) / (0.005f * 0.01f)) * 10000;
				//unsigned long dwCritOdds = (pAttack->m_fCritRate + ((float)pAttack->m_wCritLvl - (float)m_wCritLvl) / pAttack->m_dwCritRateCoefficient) * 10000;
				//unsigned long dwCritMin = (unsigned long)(pAttack->m_fCritRateMax * 10000);
				//unsigned long dwCritMax = (unsigned long)(pAttack->m_fCritRateMin * 10000);
				//if(dwCritOdds < dwCritMin)
				//	dwCritOdds = dwCritMin;
				//if(dwCritOdds > dwCritMax)
				//	dwCritOdds = dwCritMax;
				// ���﹥���̶�Ϊ10%��2014-02-26�����ڴ�DNF���ִ���������.xlsx��
				if(ePlayer != m_valTypeB && eHero != m_valTypeB)
					dwCritOdds = 1000;
				// ��С�����ʣ�1%����󱩻��ʣ�30%��2014-02-26�����ڴ�DNF���ִ���������.xlsx��
				else
				{
					if(dwCritOdds < 100)
						dwCritOdds = 100;
					if(dwCritOdds > 3000)
						dwCritOdds = 3000;
				}
				// �������
				if(rand() % 10000 >= (int)dwCritOdds)
					dwCritOdds = 0;
				// ���������ӳ�
				if(dwCritOdds)
				{
					//fHurtValue += (fHurtValue * pAttack->m_fCriMul);
					fHurtValue *= pAttack->m_fCriMul;
					bCrit = true;
					//CCLOG("��XRole::OnHurt���������� : %f", pAttack->m_fCriMul);
				}
			}
			// Ԫ�ؼ���
			if(XElement::eAttFire <= pAttack->m_byElemType && pAttack->m_byElemType <= XElement::eAttDark)
			{
				int nDiff = (int)pAttack->m_awElemAttr[pAttack->m_byElemType] - (int)m_awElemAttr[pAttack->m_byElemType + 4];
				//4.3.	�Լ���ǿ�ȣ��Է�����<-50
				//	4.3.1.	�����˺�=ԭ�����˺�*��100�����Լ���ǿ�ȣ��Է����ԣ�50��/4��/100
				if(nDiff < -50)
					fHurtValue = fHurtValue * (100.f + ((nDiff - 50) >> 2)) * 0.01f;
				//4.1.	-50<=�Լ���ǿ�ȣ��Է�����<=50
				//	4.1.1.	�����˺�=ԭ�����˺�*��100�����Լ���ǿ�ȣ��Է����ԣ�/2��/100
				else if(nDiff <= 50)
					fHurtValue = fHurtValue * (100.f + (nDiff >> 1)) * 0.01f;
				//4.2.	�Լ���ǿ�ȣ��Է�����>50
				//	4.2.1.	�����˺�=ԭ�����˺�*��100�����Լ���ǿ�ȣ��Է����ԣ�50��/4��/100
				else
					fHurtValue = fHurtValue * (100.f + ((nDiff + 50) >> 2)) * 0.01f;
			}
			if(fHurtValue < 1.f)
			{
				// ���1-10���˺�
				int nHValue = rand() % 9 + 1;
				//CCLOG("random nHValue is [%d]", nHValue);
				fHurtValue = (float)nHValue;
			}
			// ��Ч
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
			// �Լ�������һ��ɱ��
			if(false)
			{
				if(this == XJoystick::m_opCtrlRole)
					fHurtValue = 0.f;
				else
					fHurtValue = m_dwHp;
			}

			//CCLOG("��%s::OnHurt��HP : %d - %f", m_pRoleT->valName, m_dwHp, fHurtValue);
			if(eRole == m_valTypeA && ePet == m_valTypeB)
			{
				fHurtValue = 1.f;
			}
			else
			{
				// ��Ѫ
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
						// �����Boss��������ͷ
						if(XMapObj::eBoss == m_valTypeB)
							m_bySlow = 0xFF;
						if (XJoystick::m_opCtrlRole == this)
						{
							XJoystick::m_byKeyCache = 0xFF;
							// ������Ϣ
							XEventManger::ExecuteEvent(XCommonEvent::IZ_ONDEAD, 0);
						}
						if (m_pRoleT && m_pRoleT->valDeadMusicID != 65535 && m_pRoleT->valMusicOdds)
						{
							//����������Ч
							unsigned char byOdds = rand() % 100;
							if (byOdds <= m_pRoleT->valMusicOdds)
							{
								XSoundManager::PlaySoundByID(m_pRoleT->valDeadMusicID);
							}
						}
						//xyb: ��������ʱ���Ƿ��е���
						if(m_pDropSupply)
						{
							m_pDropSupply->SetPos(m_fPosX, m_fPosY, 0);
							m_pDropSupply->SetTypeC(XMapObj::eEnabled);
						}
					}
				}
				if (XJoystick::m_opCtrlRole == this || !m_valMap->m_byActivity)
				{
					// ˢ��HP
					XRole* pRole = this;
					XEventManger::ExecuteEvent(XCommonEvent::HP_CHANGE, (*((unsigned long*)&(pRole))), "XRole", 0);
				}
				if (this->GetTypeB() == XMapObj::eBoss && m_valMap->m_byActivity && pEffect->GetOwner() == XJoystick::m_opCtrlRole)
				{
					m_valMap->m_dwHurtNum += (unsigned long)fHurtValue;
				}
			}
		}
		// ��ʱ�Ƿ��ǰ��壨���ܲ�������bug��
		if (!byHurtBreak || !m_dwHp)
		{
			// Move
			Stop();
			// HitStop
			if(XMapObj::eWood != m_valTypeB)
			{
				if(pST->valHitStopH)
					Pause(pST->valHitStopH);
				// �ܴ���Ч
				if(0xFFFF != pST->valHitEff)
				{
					XEffect* pEffect = XMP_CREATE(XEffect);
					if(pEffect)
					{
						bool bBind = pEffect->SetSkillID(pST->valHitEff, this);	// ���ڸ�Ϊ�����ļ�
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
		// ľͰ���޻���
		if(XMapObj::eWood == GetTypeB())
		{
			m_pHurtMove = NULL;
			m_bHurtDir =  true;
			m_bHurtSky = false;
		}
		// ��ʱ�Ƿ��ǰ��壨���ܲ�������bug��
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
		//	CCLOG("��OnHurt��Cmd A:%d B:%d Act:%d X:%f Y:%f VX:%f VY:%f",
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
			// ������Ч
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
	// ͸����˥��
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
	// ��ȡ��Ⱦ����ƫ��
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
		// ��Ч�ĺͱ�֡����ӵļ�¼,����Ⱦ
		if(0xFF == stRGF.byActId || 0xFF == stRGF.byAlpha)
			continue;
		// ��Ⱦ
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
	// �ͷ�ͷ������Ч
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
			// ����
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
			// ��ѯ����
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
	//// ����
	//ActionFrame();
	//// ��ЧOnFrame
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
						// ˢ��MP
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
		// ������Ⱦ���
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
		//	CCLOG("��XRole::Dead�� %s", GetRoleTableName());
		//}
		//else
		//{
		//	CCLOG("��XRole::Dead�� m_pRoleT is NULL");
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
			CCLOG("��XRole::Disappear�� %s", GetRoleTableName());
		}
		else
		{
			CCLOG("��XRole::Disappear�� m_pRoleT is NULL");
		}
	//}
	XFUNC_END();
}

void XRole::ForceWait()
{
	XFUNC_START();
	m_byCmdTypeB = eCmdB_None;
	Wait(true);
	//CCLOG("��XRole::ForceWait�� %s", m_pRoleT->valName);
	XFUNC_END();
}
