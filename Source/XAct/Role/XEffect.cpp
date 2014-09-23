#include "XEffect.h"
#include "../XMap/XMap.h"
#include "../CommonFile.h"
#include "XRole.h"
#include "../XCamera/XCamera.h"
#include "../XSound/XSoundManager.h"

XCPP_DEFINED();

XEffect::XEffInfo XEffect::m_oastEffInfo[EFFECT_TYPE_NUM];

bool XEffect::SetSkillID(unsigned short wSkillID, XRole* pOwner) {
	XFUNC_START();
	if(wSkillID >= EFFECT_TYPE_NUM)
		return true;
	m_wSkillID = wSkillID;
	m_pOwner = pOwner;
	m_fSpdH = 0;
	
	m_valRendType = m_oastEffInfo[wSkillID].byRenderType;
	// 设置缩放比，根据人物
	if (m_pOwner)
	{
		m_fScale = m_pOwner->GetRoleScale();
	}
	
	m_stAnimation.SetDrawAni(m_oastEffInfo[wSkillID].wAniId, 1.f);
	m_stAnimation.AddRef();
	// 如果是绑定特效，位置为0，渲染时按玩家位置偏移(加个补丁)
	if(m_oastEffInfo[wSkillID].byBind && m_oastEffInfo[wSkillID].byBind != 2 && m_oastEffInfo[wSkillID].byBind != 3)
	{
		SetPosition(0.f, 0.f, 0.f);
	}
	// 固定位置播放特效
	else if (m_oastEffInfo[wSkillID].byBind == 3)
	{
		m_fPosX = 0;
		m_fPosY = 0;
		XCamera::Instance().PosRenderToScenen(m_fPosX, m_fPosY);
		m_fPosX += m_oastEffInfo[wSkillID].nOffsetX;
		m_fPosY += m_oastEffInfo[wSkillID].nOffsetY;

		m_fPosH = m_oastEffInfo[wSkillID].nOffsetH;
	}
	// 否则位置为世界坐标
	else
	{
		if(pOwner)
			pOwner->GetPosition(m_fPosX, m_fPosY, m_fPosH);
	}
	if (m_oastEffInfo[wSkillID].byBind != 3)
	{
		// 创建时偏移
		if(pOwner)
		{
			m_valFlipX = pOwner->GetFlipX();
			if (2601 == wSkillID || 2602 == wSkillID || 2603 == wSkillID)
			{
				m_valFlipX = 0;
			}
		}
		if(m_valFlipX)
		{
			m_fPosX -= m_oastEffInfo[wSkillID].nOffsetX*m_fScale;
		}
		else
		{
			m_fPosX += m_oastEffInfo[wSkillID].nOffsetX*m_fScale;
		}
		m_fPosH += m_oastEffInfo[wSkillID].nOffsetY*m_fScale;
	}
	m_valCamp = pOwner->GetCamp();
	m_byCover = m_oastEffInfo[wSkillID].byBind;
	if( m_byCover && m_byCover != 3)
		return true;
	XFUNC_END_RETURN(bool, res, false);
}

unsigned long XEffect::OnFrame()
{
	XFUNC_START();
	unsigned long dwRun = 0;
	if(!Valid())
		return dwRun;
	// 动画
	if(XMapObj::m_bySlow)
	{
		static unsigned long dwSlowF = 1;
		if(!fIsZero(XMapObj::m_fSlowVal))
			dwSlowF = (unsigned long)(1.f / XMapObj::m_fSlowVal);
		if(!(XMapObj::m_bySlow % dwSlowF))
			AnimateFrame();
	}
	else
		AnimateFrame();
	bool bPause = m_stAnimation.IsPause();
	if(bPause)
		return dwRun;
	// 位移
	if(XMapObj::m_bySlow)
	{
		static float fT = SPF * XMapObj::m_fSlowVal;
		RunMove(fT);
	}
	else
		RunMove(SPF);
	// 动画事件
	if(m_stAnimation.IsValid())
	{
		XFrameInfo* pFrmInfo = m_stAnimation.GetFrameInfo();
		if(pFrmInfo)
		{
			// 重力加速度
			//if(fIsZero(m_fAcceH) && m_fPosH > 0.f)
			//	m_fAcceH = m_ofGravity;
			// 动画位移
			if(m_valFlipX)
				m_fPosX -= pFrmInfo->nMoveX*m_fScale;
			else
				m_fPosX += pFrmInfo->nMoveX*m_fScale;
			m_fSpdH += pFrmInfo->nSpdH;
			for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
			{
				// 特效派生
				if(0xFFFF != pFrmInfo->wEffectID[byCnt])
				{
					XEffect* pEffect = XMP_CREATE(XEffect);
					if(pEffect)
					{
						bool bBind = pEffect->SetSkillID(pFrmInfo->wEffectID[byCnt], m_pOwner);
						if(bBind)
						{
							if(m_pOwner)
								m_pOwner->m_stEffect.AddObj(pEffect);
							else
								pEffect->Release();
						}
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
			// 碰撞
			if(!m_stAnimation.IsEnd())
			{
				XMap* pMap = NULL;
				XCollisionArea stCArea;
				if(m_oastEffInfo[m_wSkillID].byBind && m_oastEffInfo[m_wSkillID].byBind != 3)
				{
					if(m_pOwner)
					{
						pMap = m_pOwner->m_valMap;
						if (m_oastEffInfo[m_wSkillID].byBind == 2)
						{
							if(m_valFlipX)
								stCArea.fLeft	= m_fPosX - pFrmInfo->nRcX*m_fScale;
							else
								stCArea.fLeft	= m_fPosX + pFrmInfo->nRcX*m_fScale;
							stCArea.fTop	= m_fPosH + pFrmInfo->nRcY*m_fScale;
							stCArea.fPosY	= m_fPosY;
						}
						else
						{
							float fPosX = 0.f, fPosY = 0.f, fPosH = 0.f;
							m_pOwner->GetPosition(fPosX, fPosY, fPosH);
							if(m_valFlipX)
								stCArea.fLeft	= fPosX + m_fPosX - pFrmInfo->nRcX*m_fScale;
							else
								stCArea.fLeft	= fPosX + m_fPosX + pFrmInfo->nRcX*m_fScale;
							stCArea.fTop	= m_fPosH + pFrmInfo->nRcY*m_fScale + fPosH;
							stCArea.fPosY	= m_fPosY + fPosY;
						}
					}
				}
				else if (m_oastEffInfo[m_wSkillID].byBind == 3)
				{
					pMap = m_valMap;
					if(m_valMap)
					{
						stCArea.fLeft	= m_fPosX + pFrmInfo->nRcX*m_fScale;
						stCArea.fTop	= m_fPosH + pFrmInfo->nRcY*m_fScale;
						stCArea.fPosY	= m_fPosY;
					}
				}				
				else
				{
					pMap = m_valMap;
					if(m_valMap)
					{
						if(m_valFlipX)
							stCArea.fLeft	= m_fPosX - pFrmInfo->nRcX*m_fScale;
						else
							stCArea.fLeft	= m_fPosX + pFrmInfo->nRcX*m_fScale;
						stCArea.fTop	= m_fPosH + pFrmInfo->nRcY*m_fScale;
						stCArea.fPosY	= m_fPosY;
					}
				}
				if(pMap && (pFrmInfo->nRcX || pFrmInfo->nRcY || pFrmInfo->nRcH || pFrmInfo->nRcW || pFrmInfo->byThickness))
				{
					if(m_valFlipX)
						stCArea.fRight	= stCArea.fLeft - pFrmInfo->nRcW*m_fScale;
					else
						stCArea.fRight	= stCArea.fLeft + pFrmInfo->nRcW*m_fScale;
					stCArea.fBottom	= stCArea.fTop + pFrmInfo->nRcH*m_fScale;
					stCArea.fThickness = pFrmInfo->byThickness*m_fScale;
					stCArea.wType	= m_valTypeA;
					stCArea.pInfo	= this;
					pMap->CollideReg(XMapCollisionType::eAttack, m_valCamp, stCArea);
				}
			}
		}
	}
	dwRun += 1;
	return dwRun;
	XFUNC_END_RETURN(unsigned long, res, 0xFFFFFFFF);
}

void XEffect::OnDraw()
{
	XFUNC_START();
	if(!Valid())
		return;
	// 如果是绑定特效，位置为0，渲染时按玩家位置偏移
	float fRenderPosX, fRenderPosY;
	//cocos2d::ccColor3B stColor = cocos2d::ccc3(0xFF, 0xFF, 0xFF);
	if(m_oastEffInfo[m_wSkillID].byBind && m_oastEffInfo[m_wSkillID].byBind != 2 && m_oastEffInfo[m_wSkillID].byBind != 3)
	{
		float fPosX = 0.f, fPosY = 0.f, fPosH = 0.f;
		if(m_pOwner)
			m_pOwner->GetPosition(fPosX, fPosY, fPosH);
		fPosX += m_fPosX;
		fPosY += m_fPosY;
		fPosH += m_fPosH;
		// Camera
		// ...
		fRenderPosX = fPosX;
		fRenderPosY = fPosY + fPosH;
	}
	else
	{
		// Camera
		// ...
		fRenderPosX = m_fPosX;
		fRenderPosY = m_fPosY + m_fPosH;
	}
	XCamera::Instance().GetRenderPos(fRenderPosX, fRenderPosY);
	fRenderPosX = (float)((int)fRenderPosX);
	fRenderPosY = (float)((int)fRenderPosY);
	XSkillT* pST = m_oastEffInfo[m_wSkillID].pSkillT;
	cocos2d::XGLProgram* pGLPShader = NULL;
	static cocos2d::ccBlendFunc stBFuncNor = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	//// 慢镜头黑白
	//if(XMapObj::m_bySlow)
	//{
	//	static cocos2d::XShaderWhite stShaderWhite;
	//	pGLPShader = stShaderWhite.GetXProgram();
	//}
	if(pST)
	{
		if(pST->valDraw)
		{
			static cocos2d::ccBlendFunc stBFunc = { GL_SRC_ALPHA, GL_ONE };
			m_stAnimation.OnDrawEx(fRenderPosX, fRenderPosY, m_fScale, m_fScale, m_stColor,
				0xFF, stBFunc, m_valFlipX ? true : false, pGLPShader);
		}
		else
			m_stAnimation.OnDrawEx(fRenderPosX, fRenderPosY, m_fScale, m_fScale, m_stColor, 0xFF, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
	}
	else
		m_stAnimation.OnDrawEx(fRenderPosX, fRenderPosY, m_fScale, m_fScale, m_stColor, 0xFF, stBFuncNor, m_valFlipX ? true : false, pGLPShader);
//#	ifdef X_SHOWLINE
//	// 位置线
//	glLineWidth(1);
//	cocos2d::ccDrawColor4B(0x00, 0xFF, 0xFF, 0xFF);
//	cocos2d::ccDrawLine(ccp(fRenderPosX, fRenderPosY), ccp(fRenderPosX, fRenderPosY + 100.f));
//#	endif
	XFUNC_END();
}

bool XEffect::NeedRemove()
{
	XFUNC_START();
	if(!Valid())
		return true;
	if (m_oastEffInfo[m_wSkillID].nMaxHit != -1)
	{
		if (m_wHitNum >= m_oastEffInfo[m_wSkillID].nMaxHit)
		{
			return true;
		}	
	}
	
	// 播放完就销毁
	return m_stAnimation.IsEnd();
	XFUNC_END_RETURN(bool, res, false);
}

void XEffect::OnEnterMap(XMap* pMap)
{
	XFUNC_START();
	XMapObj::OnEnterMap(pMap);
	XFUNC_END();
}

void XEffect::OnLeaveMap(XMap* pMap)
{
	XFUNC_START();
	XMapObj::OnLeaveMap(pMap);
	XFUNC_END();
}

void XEffect::Release()
{
	XFUNC_START();
	if(!Valid())
		return;
	m_stAnimation.DelRef();
	XFUNC_END();
}

void XEffect::OnHit(XRole* pHurt)
{
	XFUNC_START();
	do 
	{
		if(!Valid())
			break;
		if(!m_pOwner)
			break;
		if(!m_oastEffInfo[m_wSkillID].pSkillT)
			break;
		XSkillT* pST = m_oastEffInfo[m_wSkillID].pSkillT;
		// HitStop
		//if(m_oastEffInfo[m_wSkillID].byBind && m_oastEffInfo[m_wSkillID].byBind != 2 && m_oastEffInfo[m_wSkillID].byBind != 3)
		{
			if(pST->valHitStopA)
				Pause(pST->valHitStopA);
		}
		// Role On Hit
		m_pOwner->OnHit(this, pHurt);
		if (m_oastEffInfo[m_wSkillID].nMaxHit != -1)
		{
			++m_wHitNum;		
		}
		if(pST->valHitSnFrm)
		{
			cocos2d::ccColor3B stClr = *((cocos2d::ccColor3B*)&(pST->valHitSnClr));
			XMapObj::m_stLhtClr.r = stClr.b;
			XMapObj::m_stLhtClr.g = stClr.g;
			XMapObj::m_stLhtClr.b = stClr.r;
			XMapObj::m_stLhtClr.a = pST->valHitSnAlp;
			XMapObj::m_byLight = pST->valHitSnFrm;
		}
		if(0xFFFF != pST->valHitSnCam)
			XCamera::Instance().Effect(pST->valHitSnCam);
	} while (false);
	XFUNC_END();
}

void XEffect::OnException(const char* szFile, const int nLine, const char* szInfo) {
	CCLOG("[XEffect::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
}

bool XEffect::OnCreate()
{
	XFUNC_START();
	Reset();
	return true;
	XFUNC_END_RETURN(bool, res, false);
}

void XEffect::OnRelease()
{
	XFUNC_START();
	XFUNC_END();
}

void XEffect::Initialize()
{
	XFUNC_START();
	tinyxml2::XMLDocument stXmlDocument;
	tinyxml2::XMLError eRes = stXmlDocument.LoadFile(XFILE_EFFECT);
	if(tinyxml2::XML_SUCCESS != eRes)
	{
		CCLOG("[ERROR]XEffect::Initialize load file %s failed!", XFILE_EFFECT);
		return;
	}
	int nStep = 0;
	do 
	{
		tinyxml2::XMLElement *pEffects = stXmlDocument.RootElement();
		if(!pEffects)
			break;
		++nStep;
		int nValue;
		unsigned short wEffectID;
		tinyxml2::XMLElement* pEffect = pEffects->FirstChildElement("Effect");
		while(pEffect)
		{
			eRes = pEffect->QueryIntAttribute("id", &nValue);
			if(tinyxml2::XML_SUCCESS != eRes)
				break;
			++nStep;
			if(nValue < 0 || nValue >= EFFECT_TYPE_NUM)
				break;
			++nStep;
			wEffectID = (unsigned short)nValue;
			// 属性
			{
				XEffInfo& stEI = m_oastEffInfo[wEffectID];
				eRes = pEffect->QueryIntAttribute("aniId", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.wAniId = (unsigned short)nValue;
				stEI.pSkillT = (XSkillT*)XTableFile::stTblSkill.GetRowData(wEffectID, sizeof(XSkillT));
				eRes = pEffect->QueryIntAttribute("bind", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.byBind = (unsigned char)nValue;
				eRes = pEffect->QueryIntAttribute("offsetX", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.nOffsetX = (short)nValue;
				eRes = pEffect->QueryIntAttribute("offsetY", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.nOffsetY = (short)nValue;
				eRes = pEffect->QueryIntAttribute("offsetH", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.nOffsetH = (short)nValue;
				eRes = pEffect->QueryIntAttribute("maxHit", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.nMaxHit = (short)nValue;
				eRes = pEffect->QueryIntAttribute("renderType", &nValue);
				if(tinyxml2::XML_SUCCESS == eRes)
					stEI.byRenderType = (short)nValue;
			}
			pEffect = pEffect->NextSiblingElement();
		}
		return;
	} while (false);
	CCLOG("[ERROR]XEffect::Initialize load file %s with step[%d]!", XFILE_EFFECT, nStep);
	XFUNC_END();
}
void XEffect::StopEffect()
{
	XFUNC_START();
	if(m_stAnimation.IsValid())
	{
		m_stAnimation.Reset();
		m_stAnimation.ForceToHeadFrame();
	}
	XFUNC_END();
}
void XEffect::SetDrawAni(unsigned short wAniId)
{
	XFUNC_START();
	if(0xFFFF == wAniId)
		return;
	m_stAnimation.SetDrawAni(wAniId, 1.f);
	m_stAnimation.AddRef();
	XFUNC_END();
}
XEffect* XEffect::CreateGameEffect(float fPosX, float fPosY, unsigned short wEffectId)
{
	XFUNC_START();
	if(0xFFFF == wEffectId)
		return NULL;
	XEffect* pEffect = XEffect::CreateFromPool();
	if(pEffect)
	{
		pEffect->SetDrawAni(wEffectId);
		pEffect->SetPosX(fPosX);
		pEffect->SetPosY(fPosY);
		return pEffect;
	}
	XFUNC_END_RETURN(XEffect*, res, NULL);
}
void XEffect::DelGameEffect()
{
	XFUNC_START();
	m_stAnimation.DelRef();
	if (XMemPool<XEffect>::m_pXMPParent)
	{
		XEffect* pEffect = this;
		try
		{
			XMP_RELEASE(XEffect, pEffect);
		}
		catch (...)
		{

		}
	}
	XFUNC_END();
}
void XEffect::OnGameEffectDraw(float fScale/* = 1.f*/, unsigned char byAlpha/*  = 0xFF*/)
{
	XFUNC_START();
	float fX = m_fPosX, fY = m_fPosY;
	XCamera::Instance().GetRenderPos(fX, fY);
	OnGameEffectDraw(fX, fY);
	XFUNC_END();
}
void XEffect::OnGameEffectDraw(float fX, float fY, float fScale /* = 1.f */, unsigned char byAlpha /* = 0xFF */)
{
	XFUNC_START();
	static cocos2d::ccBlendFunc stBFunc = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	m_stAnimation.OnDrawEx(fX, fY, fScale, fScale, m_stColor, 0xFF, stBFunc, m_valFlipX ? true : false, NULL);
	XFUNC_END();
}

void XEffect::Cleanup()
{
	XFUNC_START();
	Release();
	if (XMemPool<XEffect>::m_pXMPParent)
	{
		XEffect* pEffect = this;
		try
		{
			XMP_RELEASE(XEffect, pEffect);
		}
		catch (...)
		{

		}
	}
	XFUNC_END();
}
