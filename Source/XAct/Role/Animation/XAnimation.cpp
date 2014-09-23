#include "XAnimation.h"
#include "../../CommonFile.h"
#include "../../XHelper/XTable/XTableHeader.h"
#include "../../XHelper/XTable/XTableFile.h"
#include "../../XCamera/XCamera.h"
#include "../XEffect.h"

XCPP_DEFINED();

unsigned char g_byActionOut = 0x00;

XDrawTex* XDrawTex::m_oapTex[MAX_TEX_NUM] = { NULL };
unsigned char XDrawTex::_byUseCutTex = 0x00;

void XDrawTex::OnLoad()
{
	XFUNC_START();
	if(0xFFFF == m_wSprID)
	{
		CCLOG("[Error]XDrawTex::OnLoad failed!");
		return;
	}
	XTextureT* pTexSrc = (XTextureT*)XTableFile::stTblXSpr.GetRowData(m_wSprID, sizeof(XTextureT));
	if(!pTexSrc)
	{
		CCLOG("[Error]XDrawTex::OnLoad GetRowData<XTextureT> %d failed !", m_wSprID);
		return;
	}
	// 判断裁剪精灵是否存在
	if(adwXSprCutID && wXSprCutNum && awXTexCutID && byXTexCutNum)
	{
		m_wSprBNNum = byXTexCutNum;
		if(!m_apSprBN)
		{
			m_apSprBN = new cocos2d::XSpriteBatchNode*[byXTexCutNum];
			m_dwMemUsed += (sizeof(cocos2d::XSpriteBatchNode*) * byXTexCutNum);
			XMemLog::m_dwNew += (sizeof(cocos2d::XSpriteBatchNode*) * byXTexCutNum);
		}
		if(!m_apSprBN_FlipX)
		{
			m_apSprBN_FlipX = new cocos2d::XSpriteBatchNode*[byXTexCutNum];
			m_dwMemUsed += (sizeof(cocos2d::XSpriteBatchNode*) * byXTexCutNum);
			XMemLog::m_dwNew += (sizeof(cocos2d::XSpriteBatchNode*) * byXTexCutNum);
		}
		for(unsigned char byPos = 0; byPos < byXTexCutNum; ++byPos)
		{
			char szFile[MAX_PATH];
			sprintf(szFile, XPATH_TEXCUT, awXTexCutID[byPos]);
			m_apSprBN[byPos] = cocos2d::XSpriteBatchNode::create(szFile);
			if(m_apSprBN[byPos])
			{
				m_apSprBN[byPos]->setChildrenScaleY(_fChildrenScaleY);
				m_apSprBN[byPos]->setAnchorPoint(ccp(0.f, 0.f));
				m_apSprBN[byPos]->setPosition(0.f, 0.f);
				m_apSprBN[byPos]->EnabledAntiAlias();
				if(2 == pTexSrc->valCut)
					m_apSprBN[byPos]->EndbledScale(50);
				else if(2 < pTexSrc->valCut)
					m_apSprBN[byPos]->EndbledScale(pTexSrc->valCut);
			}
			m_apSprBN_FlipX[byPos] = cocos2d::XSpriteBatchNode::create(szFile);
			if(m_apSprBN_FlipX[byPos])
			{
				m_apSprBN_FlipX[byPos]->setChildrenScaleY(_fChildrenScaleY);
				m_apSprBN_FlipX[byPos]->setAnchorPoint(ccp(0.f, 0.f));
				m_apSprBN_FlipX[byPos]->setPosition(0.f, 0.f);
				m_apSprBN_FlipX[byPos]->EnabledAntiAlias();
				if(2 == pTexSrc->valCut)
					m_apSprBN_FlipX[byPos]->EndbledScale(50);
				else if(2 < pTexSrc->valCut)
					m_apSprBN_FlipX[byPos]->EndbledScale(pTexSrc->valCut);
			}
		}
		for(unsigned short wPos = 0; wPos < wXSprCutNum; ++wPos)
		{
			XTexCutT* pTexCut = (XTexCutT*)XTableFile::stTblXSprCut.GetRowData(adwXSprCutID[wPos], sizeof(XTexCutT));
			if(!pTexCut)
				continue;
			unsigned char byBNPos = 0;
			for(byBNPos = 0; byBNPos < byXTexCutNum; ++byBNPos)
			{
				if(pTexCut->valTCId == awXTexCutID[byBNPos])
					break;
			}
			if(byBNPos >= byXTexCutNum)
				continue;
			if(m_apSprBN[byBNPos])
			{
				cocos2d::CCRect stRect = cocos2d::CCRectMake(pTexCut->valLeftC, pTexCut->valTopC, pTexCut->valW, pTexCut->valH);
				m_apSprBN[byBNPos]->AddChildInfo(false, pTexCut->valLeftS, pTexCut->valTopS, stRect);
			}
			XTextureT* pTex = (XTextureT*)XTableFile::stTblXSpr.GetRowData(pTexCut->valSprId, sizeof(XTextureT));
			if(!pTex)
				continue;
			if(m_apSprBN_FlipX[byBNPos])
			{
				cocos2d::CCRect stRect = cocos2d::CCRectMake(pTexCut->valLeftC, pTexCut->valTopC, pTexCut->valW, pTexCut->valH);
				if(2 == pTexSrc->valCut)
					m_apSprBN_FlipX[byBNPos]->AddChildInfo(true, (short)(pTex->valWidth >> 1) - pTexCut->valLeftS - pTexCut->valW, pTexCut->valTopS, stRect);
				else if(2 < pTexSrc->valCut)
					m_apSprBN_FlipX[byBNPos]->AddChildInfo(true, (short)((float)pTex->valWidth * (float)pTexSrc->valCut * 0.01f) - pTexCut->valLeftS - pTexCut->valW, pTexCut->valTopS, stRect);
				else
					m_apSprBN_FlipX[byBNPos]->AddChildInfo(true, (short)pTex->valWidth - pTexCut->valLeftS - pTexCut->valW, pTexCut->valTopS, stRect);
			}
		}
		return;
	}
	// XAni是否存在
	XTexAniT* pTexAni = (XTexAniT*)XTableFile::stTblXAni.GetRowData(m_wSprID, sizeof(XTexAniT));
	if(pTexAni)
	{
		// 创建调色板精灵
		char szFile[MAX_PATH];
		sprintf(szFile, XPATH_TEXANI, pTexAni->valAniId);
		m_pSpr = cocos2d::XSprite::createWithXA8(szFile, pTexAni->valFrm);
		if(!m_pSpr)
		{
			CCLOG("[Error]XDrawTex::OnLoad XA8Sprite::create %s failed !", szFile);
			return;
		}
		if(2 == pTexSrc->valCut)
			m_pSpr->EndbledScale(50);
		else if(2 < pTexSrc->valCut)
			m_pSpr->EndbledScale(pTexSrc->valCut);
	}
	// 不存在则创建原始精灵
	else
	{
		m_pSpr = cocos2d::XSprite::create(pTexSrc->valImgPath, true);
		if(!m_pSpr)
		{
			CCLOG("[Error]XDrawTex::OnLoad XSprite::create %s failed !", pTexSrc->valImgPath);
			return;
		}
	}
	m_pSpr->EnabledAntiAlias();
	m_pSpr->setAnchorPoint(ccp(0.5f, 0.f));
	XFUNC_END();
}

void XDrawTex::OnRelease()
{
	XFUNC_START();
	if(m_pSpr)
	{
		m_pSpr->cleanup();
		m_pSpr = NULL;
	}
	if(!m_apSprBN || !m_apSprBN_FlipX || !m_wSprBNNum)
		return;
	for(unsigned short wPos = 0; wPos < m_wSprBNNum; ++wPos)
	{
		if(m_apSprBN[wPos])
		{
			m_apSprBN[wPos]->cleanup();
			m_apSprBN[wPos] = NULL;
		}
		if(m_apSprBN_FlipX[wPos])
		{
			m_apSprBN_FlipX[wPos]->cleanup();
			m_apSprBN_FlipX[wPos] = NULL;
		}
	}
	XFUNC_END();
}
extern unsigned char g_byShowLine;
void XDrawTex::Draw(float fPosX, float fPosY, float fScale, cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX)
{
	XFUNC_START();
	static cocos2d::ccBlendFunc stBFunc = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	if(m_pSpr)
	{
		// 需修改：不一样才赋值
		// ...
		if(m_pSpr->Loaded())
		{
			m_pSpr->setFlipX(bFlipX);
			m_pSpr->setScale(fScale);
			m_pSpr->setColor(stColor);
			m_pSpr->setOpacity(byAlpha);
			m_pSpr->setPosition(ccp(fPosX, fPosY));
			m_pSpr->setBlendFunc(stBFunc);
			m_pSpr->visit();
		}
	}
	if(!m_apSprBN || !m_apSprBN_FlipX || !m_wSprBNNum)
		return;
	for(unsigned short wPos = 0; wPos < m_wSprBNNum; ++wPos)
	{
		if(bFlipX)
		{
			if(m_apSprBN_FlipX[wPos])
			{
				if(m_apSprBN_FlipX[wPos]->Loaded())
				{
					m_apSprBN_FlipX[wPos]->setScale(fScale);
					m_apSprBN_FlipX[wPos]->setColor(stColor);
					m_apSprBN_FlipX[wPos]->setOpacity(byAlpha);
					m_apSprBN_FlipX[wPos]->setPosition((short)fPosX - (m_nTexW >> 1), (short)fPosY);
					m_apSprBN_FlipX[wPos]->setBlendFunc(stBFunc);
					m_apSprBN_FlipX[wPos]->visit();
				}
			}
		}
		else
		{
			if(m_apSprBN[wPos])
			{
				if(m_apSprBN_FlipX[wPos]->Loaded())
				{
					m_apSprBN[wPos]->setScale(fScale);
					m_apSprBN[wPos]->setColor(stColor);
					m_apSprBN[wPos]->setOpacity(byAlpha);
					m_apSprBN[wPos]->setPosition((short)fPosX - (m_nTexW >> 1), (short)fPosY);
					m_apSprBN[wPos]->setBlendFunc(stBFunc);
					m_apSprBN[wPos]->visit();
				}
			}
		}
	}
	XFUNC_END();
}

void XDrawTex::DrawEx(float fPosX, float fPosY, float fScaleX, float fScaleY,
	cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP)
{
	XFUNC_START();
	cocos2d::ccBlendFunc stBFuncOld;
	if(m_pSpr)
	{
		// 需修改：不一样才赋值
		// ...
		if(m_pSpr->Loaded())
		{
			m_pSpr->setFlipX(bFlipX);
			m_pSpr->setScaleX(fScaleX);
			m_pSpr->setScaleY(fScaleY);
			m_pSpr->setOpacity(byAlpha);
			m_pSpr->setPosition(ccp(fPosX, fPosY));
			stBFuncOld = m_pSpr->getBlendFunc();
			m_pSpr->setBlendFunc(stBFunc);
			m_pSpr->setColor(stColor);
			m_pSpr->SetShaderOnce(pGLP);
			m_pSpr->visit();
			m_pSpr->setBlendFunc(stBFuncOld);
		}
	}
	if(!m_apSprBN || !m_apSprBN_FlipX || !m_wSprBNNum)
		return;
	static cocos2d::ccBlendFunc stBFuncLine = {GL_SRC_COLOR, GL_DST_COLOR};
	static cocos2d::XShaderColor stShaderColor;
	static cocos2d::XGLProgram* pGLPColor = stShaderColor.GetXProgram();
	for(unsigned short wPos = 0; wPos < m_wSprBNNum; ++wPos)
	{
		if(bFlipX)
		{
			if(m_apSprBN_FlipX[wPos])
			{
				if(m_apSprBN_FlipX[wPos]->Loaded())
				{
					m_apSprBN_FlipX[wPos]->setScaleX(fScaleX);
					m_apSprBN_FlipX[wPos]->setScaleY(fScaleY);
					m_apSprBN_FlipX[wPos]->setOpacity(byAlpha);
					m_apSprBN_FlipX[wPos]->setPosition((short)fPosX - (((short)(m_nTexW*fScaleX)) >> 1), (short)fPosY);
					stBFuncOld = m_apSprBN_FlipX[wPos]->getBlendFunc();
					m_apSprBN_FlipX[wPos]->setBlendFunc(stBFunc);
					m_apSprBN_FlipX[wPos]->setColor(stColor);
					m_apSprBN_FlipX[wPos]->SetShaderOnce(pGLP);
					m_apSprBN_FlipX[wPos]->visit();
					if(g_byShowLine)
					{
#if defined(WIN32) && defined(_WINDOWS)
						glLineWidth(1);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						m_apSprBN_FlipX[wPos]->setBlendFunc(stBFuncLine);
						stShaderColor.SetColor(0xFF, 0xFF, 0x00);
						m_apSprBN_FlipX[wPos]->SetShaderOnce(pGLPColor);
						m_apSprBN_FlipX[wPos]->visit();
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
					}
					m_apSprBN_FlipX[wPos]->setBlendFunc(stBFuncOld);
				}
			}
		}
		else
		{
			if(m_apSprBN[wPos])
			{
				if(m_apSprBN_FlipX[wPos]->Loaded())
				{
					m_apSprBN[wPos]->setScaleX(fScaleX);
					m_apSprBN[wPos]->setScaleY(fScaleY);
					m_apSprBN[wPos]->setOpacity(byAlpha);
					m_apSprBN[wPos]->setPosition((short)fPosX - (((short)(m_nTexW*fScaleX)) >> 1), (short)fPosY);
					stBFuncOld = m_apSprBN[wPos]->getBlendFunc();
					m_apSprBN[wPos]->setBlendFunc(stBFunc);
					m_apSprBN[wPos]->setColor(stColor);
					m_apSprBN[wPos]->SetShaderOnce(pGLP);
					m_apSprBN[wPos]->visit();
					if(g_byShowLine)
					{
#if defined(WIN32) && defined(_WINDOWS)
						glLineWidth(1);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						m_apSprBN[wPos]->setBlendFunc(stBFuncLine);
						stShaderColor.SetColor(0xFF, 0xFF, 0x00);
						m_apSprBN[wPos]->SetShaderOnce(pGLPColor);
						m_apSprBN[wPos]->visit();
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
					}
					m_apSprBN[wPos]->setBlendFunc(stBFuncOld);
				}
			}
		}
	}
	XFUNC_END();
}

void XDrawTex::Initialize()
{
	XFUNC_START();
	XTextureT* pTex = NULL;
	unsigned long dwSprNum = XTableFile::stTblXSpr.GetMaxRowID();
	if(!dwSprNum)
	{
		CCLOG("[Warning]XDrawTex::Initialize sprite num is 0!");
		return;
	}
	for (unsigned long dwPos = 0; dwPos < dwSprNum; ++dwPos)
	{
		pTex = (XTextureT*)XTableFile::stTblXSpr.GetRowData(dwPos, sizeof(XTextureT));
		if(pTex)
		{
			m_oapTex[dwPos] = new XDrawTex;
			m_oapTex[dwPos]->SetSprId(dwPos);
			m_oapTex[dwPos]->m_nTexW = pTex->valWidth;
			m_oapTex[dwPos]->m_nTexH = pTex->valHeight;
		}
	}
	// 裁剪贴图信息
	XTexCutT* pTexCut = NULL;
	unsigned long dwSprCutNum = XTableFile::stTblXSprCut.GetMaxRowID();
	if(!dwSprCutNum)
	{
		CCLOG("[Warning]XDrawTex::Initialize sprite cut num is 0!");
		return;
	}
	_byUseCutTex = 0xFF;
	// 计算用
	const unsigned char byCutTexNumMax = 6;	// 最多只可能分布在几张贴图
	unsigned short	wTexCutID[byCutTexNumMax] = { 0xFFFF };
	memset(wTexCutID, 0xFFFFFFFF, sizeof(unsigned short) * byCutTexNumMax);
	unsigned char	byTexCutNum = 0;
	unsigned short	wTexID = 0xFFFF;
	unsigned long	dwPosSt = 0;
	for (unsigned long dwPos = 0; dwPos < dwSprCutNum; ++dwPos)
	{
		pTexCut = (XTexCutT*)XTableFile::stTblXSprCut.GetRowData(dwPos, sizeof(XTexCutT));
		if(!pTexCut)
		{
			CCLOG("[ERROR]Tex cut %d is not exist!", dwPos);
			return;
		}
		if(0xFFFF == wTexID)
		{
			wTexID = pTexCut->valSprId;
			dwPosSt = dwPos;
		}
		else if(wTexID != pTexCut->valSprId)
		{
			XDrawTex* pDTex = m_oapTex[wTexID];
			if(!pDTex)
			{
				CCLOG("[ERROR]Tex %d is not exist!", wTexID);
				return;
			}
			// 赋值
			pDTex->awXTexCutID = new unsigned short[byTexCutNum];
			pDTex->m_dwMemUsed += (sizeof(unsigned short) * byTexCutNum);
			XMemLog::m_dwNew += (sizeof(unsigned short) * byTexCutNum);
			pDTex->byXTexCutNum = byTexCutNum;
			for(unsigned char byNum = 0; byNum < byTexCutNum; ++byNum)
				pDTex->awXTexCutID[byNum] = wTexCutID[byNum];
			pDTex->adwXSprCutID = new unsigned long[dwPos - dwPosSt];
			pDTex->m_dwMemUsed += (sizeof(unsigned long) * (dwPos - dwPosSt));
			XMemLog::m_dwNew += (sizeof(unsigned long) * (dwPos - dwPosSt));
			pDTex->wXSprCutNum = dwPos - dwPosSt;
			for (unsigned long dwSetPos = dwPosSt; dwSetPos < dwPos; ++dwSetPos)
				pDTex->adwXSprCutID[dwSetPos - dwPosSt] = dwSetPos;
			// 重置
			wTexID = pTexCut->valSprId;
			memset(wTexCutID, 0xFFFFFFFF, sizeof(unsigned short) * byCutTexNumMax);
			byTexCutNum = 0;
			dwPosSt = dwPos;
		}
		// 赋值
		unsigned char byNum = 0;
		for(byNum = 0; byNum < byCutTexNumMax; ++byNum)
		{
			if(pTexCut->valTCId == wTexCutID[byNum])
				break;
		}
		if(byNum < byCutTexNumMax)
		{
			// 找到了，不管
		}
		else
		{
			if(byTexCutNum >= byCutTexNumMax)
				// 超过上限
				CCLOG("[ERROR]Tex cut num must be less than %d!", byCutTexNumMax);
			else
				wTexCutID[byTexCutNum++] = pTexCut->valTCId;
		}
	}
	// 最后一个
	XDrawTex* pDTex = m_oapTex[wTexID];
	if(!pDTex)
	{
		CCLOG("[ERROR]Tex %d is not exist!", wTexID);
		return;
	}
	// 赋值
	pDTex->awXTexCutID = new unsigned short[byTexCutNum];
	pDTex->m_dwMemUsed += (sizeof(unsigned short) * byTexCutNum);
	XMemLog::m_dwNew += (sizeof(unsigned short) * byTexCutNum);
	pDTex->byXTexCutNum = byTexCutNum;
	for(unsigned char byNum = 0; byNum < byTexCutNum; ++byNum)
		pDTex->awXTexCutID[byNum] = wTexCutID[byNum];
	pDTex->adwXSprCutID = new unsigned long[dwSprCutNum - dwPosSt];
	pDTex->m_dwMemUsed += (sizeof(unsigned long) * (dwSprCutNum - dwPosSt));
	XMemLog::m_dwNew += (sizeof(unsigned long) * (dwSprCutNum - dwPosSt));
	pDTex->wXSprCutNum = dwSprCutNum - dwPosSt;
	for (unsigned long dwSetPos = dwPosSt; dwSetPos < dwSprCutNum; ++dwSetPos)
		pDTex->adwXSprCutID[dwSetPos - dwPosSt] = dwSetPos;
	XFUNC_END();
}

void XDrawTex::DumpUsedInfo()
{
	XFUNC_START();
	CCLOG("XDrawTex::DumpUsedInfo : ");
	unsigned long dwSprNum = XTableFile::stTblXSpr.GetMaxRowID();
	if(dwSprNum)
	{
		XTextureT* pTex = NULL;
		unsigned long dwRef = 0;
		for (unsigned long dwPos = 0; dwPos < dwSprNum; ++dwPos)
		{
			if(!m_oapTex[dwPos])
				continue;
			pTex = (XTextureT*)XTableFile::stTblXSpr.GetRowData(dwPos, sizeof(XTextureT));
			if(!pTex)
				continue;
			dwRef = m_oapTex[dwPos]->GetRef();
			if(!dwRef)
				continue;
			CCLOG("[%d]ref[%d]%s", dwPos, dwRef, pTex->valImgPath);
		}
	}
	XFUNC_END();
}

XDrawAnimation* XDrawAnimation::m_oapAni[MAX_ANI_NUM] = { NULL };
unsigned short XDrawAnimation::m_oawCD[MAX_ANI_NUM] = {0};

void XDrawAnimation::OnLoad()
{
	XFUNC_START();
	if(!m_wFrameNum || !m_apFrames)
		return;
	for(unsigned short wPos = 0; wPos < m_wFrameNum; ++wPos)
	{
		const XFrameInfo& stFI = m_apFrames[wPos];
		if(0xFFFF == stFI.wDrawTexID)
			continue;
		if(!XDrawTex::m_oapTex[stFI.wDrawTexID])
			continue;
		XDrawTex::m_oapTex[stFI.wDrawTexID]->setChildrenScaleY(_fChildrenScaleY);
		XDrawTex::m_oapTex[stFI.wDrawTexID]->AddRef();
		// 特效的动画ID
		for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
		{
			if(0xFFFF != stFI.wEffectID[byCnt])
			{
				unsigned short wEffAniId = XEffect::m_oastEffInfo[stFI.wEffectID[byCnt]].wAniId;
				if(XDrawAnimation::m_oapAni[wEffAniId])
					XDrawAnimation::m_oapAni[wEffAniId]->AddRef();
			}
		}
	}
	XFUNC_END();
}

void XDrawAnimation::OnRelease()
{
	XFUNC_START();
	if(!m_wFrameNum || !m_apFrames)
		return;
	for(unsigned short wPos = 0; wPos < m_wFrameNum; ++wPos)
	{
		const XFrameInfo& stFI = m_apFrames[wPos];
		if(0xFFFF == stFI.wDrawTexID)
			continue;
		if(!XDrawTex::m_oapTex[stFI.wDrawTexID])
			continue;
		XDrawTex::m_oapTex[stFI.wDrawTexID]->DelRef();
		// 特效的动画ID
		for (unsigned char byCnt = 0; byCnt < MAX_EFFECT_NUM; ++byCnt)
		{
			if(0xFFFF != stFI.wEffectID[byCnt])
			{
				unsigned short wEffAniId = XEffect::m_oastEffInfo[stFI.wEffectID[byCnt]].wAniId;
				if(XDrawAnimation::m_oapAni[wEffAniId])
					XDrawAnimation::m_oapAni[wEffAniId]->DelRef();
			}
		}
		//if (0xFFFF != stFI.wSoundID)
		//{
		//	cocos2d::XSoundManager::StopSoundByID(stFI.wSoundID);
		//}
	}
	XFUNC_END();
}

void XDrawAnimation::OnDraw(unsigned short wFrame, float fPosX, float fPosY, float fScale, cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX)
{
	XFUNC_START();
	if(wFrame >= m_wFrameNum)
	{
		CCLOG("[ERROR]XDrawAnimation::OnDraw frame %d must be less than num %d!", wFrame, m_wFrameNum);
		return;
	}
	const XFrameInfo& stFI = m_apFrames[wFrame];
	if(0xFFFF == stFI.wDrawTexID)
	{
		//CCLOG("[ERROR]XDrawAnimation::OnDraw 0xFFFF == stFI.wDrawTexID!");
		return;
	}
	if(!XDrawTex::m_oapTex[stFI.wDrawTexID])
	{
		CCLOG("[ERROR]XDrawAnimation::OnDraw XDrawTex::m_oapTex[%d] Frame[%d]!", stFI.wDrawTexID, wFrame);
		return;
	}
	if(bFlipX)
		fPosX -= stFI.nOffsetX*fScale;
	else
		fPosX += stFI.nOffsetX*fScale;
	fPosY += stFI.nOffsetH*fScale;
	//CCLOG("DrawAni Frame [%d] Draw Tex [%d] flip[%s]", wFrame, stFI.wDrawTexID, bFlipX ? "true" : "false");
	if(m_byColorAdd)
	{
		static cocos2d::ccBlendFunc stBFuncCA = { GL_SRC_ALPHA, GL_ONE };
		XDrawTex::m_oapTex[stFI.wDrawTexID]->DrawEx(fPosX, fPosY, fScale, fScale, stColor, byAlpha, stBFuncCA, bFlipX^(bool)stFI.byFlipX, NULL);
	}
	else
		XDrawTex::m_oapTex[stFI.wDrawTexID]->Draw(fPosX, fPosY, fScale, stColor, byAlpha, bFlipX^(bool)stFI.byFlipX);

	//// 镜头特效
	//if (0xFFFF != stFI.wCameraID)
	//{
	//	XCamera::Instance().Effect(stFI.wCameraID);
	//}
	
	XFUNC_END();
}

void XDrawAnimation::OnDrawEx(unsigned short wFrame, float fPosX, float fPosY, float fScaleX, float fScaleY,
	cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP)
{
	XFUNC_START();
	if(wFrame >= m_wFrameNum)
	{
		CCLOG("[ERROR]XDrawAnimation::OnDrawEx frame %d must be less than num %d!", wFrame, m_wFrameNum);
		return;
	}
	const XFrameInfo& stFI = m_apFrames[wFrame];
	if(0xFFFF == stFI.wDrawTexID)
	{
		//CCLOG("[ERROR]XDrawAnimation::OnDraw 0xFFFF == stFI.wDrawTexID!");
		return;
	}
	if(!XDrawTex::m_oapTex[stFI.wDrawTexID])
	{
		CCLOG("[ERROR]XDrawAnimation::OnDrawEx XDrawTex::m_oapTex[%d] Frame[%d]!", stFI.wDrawTexID, wFrame);
		return;
	}
	if(bFlipX)
		fPosX -= stFI.nOffsetX*fScaleX;
	else
		fPosX += stFI.nOffsetX*fScaleX;
	fPosY += stFI.nOffsetH*fScaleY;
	if(m_byColorAdd)
	{
		static cocos2d::ccBlendFunc stBFuncCA = { GL_SRC_ALPHA, GL_ONE };
		stBFunc = stBFuncCA;
	}
	//CCLOG("DrawAni Frame [%d] Draw Tex [%d] flip[%s]", wFrame, stFI.wDrawTexID, bFlipX ? "true" : "false");
	XDrawTex::m_oapTex[stFI.wDrawTexID]->DrawEx(fPosX, fPosY, fScaleX, fScaleY, stColor, byAlpha, stBFunc, bFlipX^(bool)stFI.byFlipX, pGLP);

	//// 镜头特效
	//if (0xFFFF != stFI.wCameraID)
	//{
	//	XCamera::Instance().Effect(stFI.wCameraID);
	//}
	XFUNC_END();
}

void XDrawAnimation::Initialize()
{
	XFUNC_START();
	tinyxml2::XMLDocument stXmlDocument;
	tinyxml2::XMLError eRes = stXmlDocument.LoadFile(XFILE_ANIMATION, 0x02);
	if(tinyxml2::XML_SUCCESS != eRes)
	{
		CCLOG("[ERROR]XDrawAnimation::Initialize load file %s failed!", XFILE_ANIMATION);
		return;
	}
#	ifdef _WINDOWS_
	FILE* pfActOut = NULL;
	XAutoCloseFile stFClose;
	char szCurrDir[MAX_PATH];
	char szBuf[512];
	if(g_byActionOut)
	{
		pfActOut = fopen("CopyAction.bat", "wt");
		if(pfActOut)
		{
			stFClose.Bind(pfActOut);
			const char szHead[] = 
				"echo off\n"
				"for /f \"tokens=2,*\" %%i in (\'reg query "
				"\"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\" /v \"Desktop\"\') do ("
				"set desktop=%%j)\n"
				"echo Desktop path : %desktop%\n"
				"md %desktop%\\Action\n";
			fwrite(szHead, 1, strlen(szHead), pfActOut);
			GetCurrentDirectoryA(MAX_PATH, szCurrDir);
		}
	}
#	endif
	int nStep = 0;
	do 
	{
		tinyxml2::XMLElement *pAnimations = stXmlDocument.RootElement();
		if(!pAnimations)
			break;
		++nStep;
		int nValue;
		unsigned short wAniID;
		tinyxml2::XMLElement* pAnimate = pAnimations->FirstChildElement("Animate");
		while(pAnimate)
		{
			eRes = pAnimate->QueryIntAttribute("id", &nValue);
			if(tinyxml2::XML_SUCCESS != eRes)
				break;
			++nStep;
			if(nValue < 0 || nValue >= MAX_ANI_NUM)
				break;
			++nStep;
			wAniID = (unsigned short)nValue;
			if(m_oapAni[wAniID])
				break;
			eRes = pAnimate->QueryIntAttribute("cd", &nValue);
			if(tinyxml2::XML_SUCCESS != eRes)
			{
				m_oawCD[wAniID] = 0;
			}
			else
			{
				m_oawCD[wAniID] = nValue;
			}
			++nStep;
			m_oapAni[wAniID] = new XDrawAnimation;
			tinyxml2::XMLElement* pFrame;
#			ifdef _WINDOWS_
			if(pfActOut)
			{
				sprintf(szBuf, "md %%desktop%%\\Action\\%d\n", wAniID);
				fwrite(szBuf, 1, strlen(szBuf), pfActOut);
			}
#			endif
			// 查询帧数
			m_oapAni[wAniID]->m_wFrameNum = 0;
			eRes = pAnimate->QueryIntAttribute("flyH", &nValue);
			if(tinyxml2::XML_SUCCESS == eRes)
				m_oapAni[wAniID]->m_wFlyH = (unsigned short)nValue;
			eRes = pAnimate->QueryIntAttribute("flySpd", &nValue);
			if(tinyxml2::XML_SUCCESS == eRes)
				m_oapAni[wAniID]->m_wFlySpd = (unsigned short)nValue;
			pFrame = pAnimate->FirstChildElement("Frame");
			while(pFrame)
			{
				++m_oapAni[wAniID]->m_wFrameNum;
				pFrame = pFrame->NextSiblingElement();
			}
			if(!m_oapAni[wAniID]->m_wFrameNum)
				break;
			++nStep;
			// 初始化
			m_oapAni[wAniID]->m_apFrames = new XFrameInfo[m_oapAni[wAniID]->m_wFrameNum];
			if(!m_oapAni[wAniID]->m_apFrames)
				break;
			++nStep;
			// 赋值
			m_oapAni[wAniID]->m_wFrameNum = 0;
			pFrame = pAnimate->FirstChildElement("Frame");
			while(pFrame)
			{
				eRes = pFrame->QueryIntAttribute("id", &nValue);
				if(tinyxml2::XML_SUCCESS != eRes)
					break;
				++nStep;
				if(0 <= nValue && nValue < MAX_TEX_NUM)
				{
					XFrameInfo& stFI = m_oapAni[wAniID]->m_apFrames[m_oapAni[wAniID]->m_wFrameNum];
					stFI.wDrawTexID = (unsigned short)nValue;
#					ifdef _WINDOWS_
					if(pfActOut)
					{
						XTextureT* pTex = (XTextureT*)XTableFile::stTblXSpr.GetRowData(stFI.wDrawTexID, sizeof(XTextureT));
						if(pTex)
						{
							char* szTmp;
							sprintf(szBuf, "copy %s\\%s %%desktop%%\\Action\\%d\n",
								szCurrDir, pTex->valImgPath,
								wAniID);
							szTmp = szBuf;
							while(*szTmp)
							{
								if('/' == *szTmp)
									*szTmp = '\\';
								++szTmp;
							}
							fwrite(szBuf, 1, strlen(szBuf), pfActOut);
							char* szFileName = pTex->valImgPath;
							szTmp = pTex->valImgPath;
							while(*szTmp)
							{
								if('/' == *szTmp || '\\' == *szTmp)
									szFileName = szTmp + 1;
								++szTmp;
							}
							sprintf(szBuf, "ren %%desktop%%\\Action\\%d\\%s %d.png\n",
								wAniID, szFileName, m_oapAni[wAniID]->m_wFrameNum);
							szTmp = szBuf;
							while(*szTmp)
							{
								if('/' == *szTmp)
									*szTmp = '\\';
								++szTmp;
							}
							fwrite(szBuf, 1, strlen(szBuf), pfActOut);
						}
					}
#					endif
					eRes = pFrame->QueryIntAttribute("movX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nMoveX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("spdH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nSpdH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("osX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nOffsetX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("osH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nOffsetH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcY", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcY = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcW", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcW = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcTh", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byThickness = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("rcTp", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byType = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("effId", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[0] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_1", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[1] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_2", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[2] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_3", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[3] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_4", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[4] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("cameraID", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wCameraID = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("soundID", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wSoundID = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("stay", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byStay = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("break", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byBreak = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("hurtbreak", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byHurtBreak = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("FlipX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byFlipX = (bool)(nValue > 0);
				}
				++m_oapAni[wAniID]->m_wFrameNum;
				pFrame = pFrame->NextSiblingElement();
			}
			pAnimate = pAnimate->NextSiblingElement();
		}
	} while (false);

	// 初始化特效动画表
	tinyxml2::XMLDocument stXmlDocumentEff;
	eRes = stXmlDocumentEff.LoadFile(XFILE_EFFECTANI);
	if(tinyxml2::XML_SUCCESS != eRes)
	{
		CCLOG("[ERROR]XDrawAnimation::Initialize load file %s failed!", XFILE_EFFECTANI);
		return;
	}
	nStep = 0;
	do 
	{
		tinyxml2::XMLElement *pAnimations = stXmlDocumentEff.RootElement();
		if(!pAnimations)
			break;
		++nStep;
		int nValue;
		unsigned short wAniID;
		tinyxml2::XMLElement* pAnimate = pAnimations->FirstChildElement("Animate");
		while(pAnimate)
		{
			eRes = pAnimate->QueryIntAttribute("id", &nValue);
			if(tinyxml2::XML_SUCCESS != eRes)
				break;
			++nStep;
			if(nValue < 0 || nValue >= MAX_ANI_NUM)
				break;
			++nStep;
			wAniID = (unsigned short)nValue;
			if(m_oapAni[wAniID])
			{
				CCLOG("[ERROR]maybe role ani and effect ani are the same aniID! aniID = [%d]", wAniID);
				break;
			}
			++nStep;
			m_oapAni[wAniID] = new XDrawAnimation;
			eRes = pAnimate->QueryIntAttribute("add", &nValue);
			if(tinyxml2::XML_SUCCESS == eRes && nValue)
				m_oapAni[wAniID]->m_byColorAdd = 0xFF;
			tinyxml2::XMLElement* pFrame;
			// 查询帧数
			m_oapAni[wAniID]->m_wFrameNum = 0;
			pFrame = pAnimate->FirstChildElement("Frame");
			while(pFrame)
			{
				++m_oapAni[wAniID]->m_wFrameNum;
				pFrame = pFrame->NextSiblingElement();
			}
			if(!m_oapAni[wAniID]->m_wFrameNum)
				break;
			++nStep;
			// 初始化
			m_oapAni[wAniID]->m_apFrames = new XFrameInfo[m_oapAni[wAniID]->m_wFrameNum];
			if(!m_oapAni[wAniID]->m_apFrames)
				break;
			++nStep;
			// 赋值
			m_oapAni[wAniID]->m_wFrameNum = 0;
			pFrame = pAnimate->FirstChildElement("Frame");
			while(pFrame)
			{
				eRes = pFrame->QueryIntAttribute("id", &nValue);
				if(tinyxml2::XML_SUCCESS != eRes)
					break;
				++nStep;
				if(0 <= nValue && nValue < MAX_TEX_NUM)
				{
					XFrameInfo& stFI = m_oapAni[wAniID]->m_apFrames[m_oapAni[wAniID]->m_wFrameNum];
					stFI.wDrawTexID = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("movX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nMoveX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("spdH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nSpdH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("osX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nOffsetX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("osH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nOffsetH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcX = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcY", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcY = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcW", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcW = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcH", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.nRcH = (short)nValue;
					eRes = pFrame->QueryIntAttribute("rcTh", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byThickness = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("rcTp", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byType = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("effId", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[0] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_1", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[1] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_2", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[2] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_3", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[3] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("effId_4", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wEffectID[4] = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("soundID", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wSoundID = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("cameraID", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.wCameraID = (unsigned short)nValue;
					eRes = pFrame->QueryIntAttribute("stay", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byStay = (unsigned char)nValue;
					eRes = pFrame->QueryIntAttribute("FlipX", &nValue);
					if(tinyxml2::XML_SUCCESS == eRes)
						stFI.byFlipX = (bool)(nValue > 0);
				}
				++m_oapAni[wAniID]->m_wFrameNum;
				pFrame = pFrame->NextSiblingElement();
			}
			pAnimate = pAnimate->NextSiblingElement();
		}
		return;
	} while (false);
	CCLOG("[ERROR]XDrawAnimation::Initialize load file %s with step[%d]!", XFILE_ANIMATION, nStep);
	XFUNC_END();
}

void XAnimation::OnLoad()
{
	XFUNC_START();
	if(!IsValid())
		return;
	if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
		return;
	XDrawAnimation::m_oapAni[m_wDrawAniID]->AddRef();
	XFUNC_END();
}

void XAnimation::OnRelease()
{
	XFUNC_START();
	if(!IsValid())
		return;
	if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
		return;
	XDrawAnimation::m_oapAni[m_wDrawAniID]->DelRef();
	XFUNC_END();
}

void XAnimation::OnDraw(float fPosX, float fPosY, float fScale, cocos2d::ccColor3B stColor, unsigned char byAlpha, bool bFlipX)
{
	XFUNC_START();
	if(!IsValid())
	{
		//CCLOG("XAnimation::OnDraw [%d] !IsValid()", m_wDrawAniID);
		return;
	}
	if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
	{
		//CCLOG("XAnimation::OnDraw !XDrawAnimation::m_oapAni[%d]", m_wDrawAniID);
		return;
	}
	//static XAnimation* pAnimation = NULL;
	//if(!pAnimation)
	//	pAnimation = this;
	//if(pAnimation == this)
	//	CCLOG("Ani [%d] Draw frame [%d] flip[%s]", m_wDrawAniID, m_wCurrFrame, bFlipX ? "true" : "false");
	if(0xFFFF == m_wDrawFrameOnce)
	{
		if(0xFFFF == m_wCurrFrame)
		{
			//CCLOG("XAnimation::OnDraw [%d] 0xFFFF == m_wCurrFrame", m_wDrawAniID);
			return;
		}
		XDrawAnimation::m_oapAni[m_wDrawAniID]->OnDraw(m_wCurrFrame, fPosX, fPosY, fScale, stColor, byAlpha, bFlipX);
	}
	else
	{
		XDrawAnimation::m_oapAni[m_wDrawAniID]->OnDraw(m_wDrawFrameOnce, fPosX, fPosY, fScale, stColor, byAlpha, bFlipX);
		m_wDrawFrameOnce = 0xFFFF;
	}
	XFUNC_END();
}

void XAnimation::OnDrawEx(float fPosX, float fPosY, float fScaleX, float fScaleY,
	cocos2d::ccColor3B stColor, unsigned char byAlpha, cocos2d::ccBlendFunc stBFunc, bool bFlipX, cocos2d::XGLProgram* pGLP)
{
	XFUNC_START();
	if(!IsValid())
	{
		//CCLOG("XAnimation::OnDraw [%d] !IsValid()", m_wDrawAniID);
		return;
	}
	if(!XDrawAnimation::m_oapAni[m_wDrawAniID])
	{
		//CCLOG("XAnimation::OnDraw !XDrawAnimation::m_oapAni[%d]", m_wDrawAniID);
		return;
	}
	//static XAnimation* pAnimation = NULL;
	//if(!pAnimation)
	//	pAnimation = this;
	//if(pAnimation == this)
	//	CCLOG("Ani [%d] Draw frame [%d] flip[%s]", m_wDrawAniID, m_wCurrFrame, bFlipX ? "true" : "false");
	if(0xFFFF == m_wDrawFrameOnce)
	{
		if(0xFFFF == m_wCurrFrame)
		{
			//CCLOG("XAnimation::OnDraw [%d] 0xFFFF == m_wCurrFrame", m_wDrawAniID);
			return;
		}
		XDrawAnimation::m_oapAni[m_wDrawAniID]->OnDrawEx(m_wCurrFrame, fPosX, fPosY, fScaleX, fScaleY, stColor, byAlpha, stBFunc, bFlipX, pGLP);
	}
	else
	{
		XDrawAnimation::m_oapAni[m_wDrawAniID]->OnDrawEx(m_wDrawFrameOnce, fPosX, fPosY, fScaleX, fScaleY, stColor, byAlpha, stBFunc, bFlipX, pGLP);
		m_wDrawFrameOnce = 0xFFFF;
	}
	XFUNC_END();
}

volatile unsigned char XTexInitThread::_byLoaded = 0x00;

void XTexInitThread::InitAllAnimation()
{
	XFUNC_START();
	_byLoaded = 0x00;
	static XTexInitThread stInitThread;
	stInitThread.Initialize();
	XFUNC_END();
}

void XTexInitThread::Initialize()
{
	XFUNC_START();
	m_stLoadThread.Start(this, &XTexInitThread::XThreadLoad);
	XFUNC_END();
}

unsigned long XTexInitThread::XThreadLoad(long param, volatile bool& bExit)
{
	XFUNC_START();
	_byLoaded = 0x01;
	XDrawTex::Initialize();
	XDrawAnimation::Initialize();
	_byLoaded = 0xFF;
	XFUNC_END_RETURN(unsigned short, res, 0);
}

