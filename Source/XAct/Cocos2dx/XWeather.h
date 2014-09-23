#ifndef __XWEATHER_H__
#define __XWEATHER_H__
/*
	天气类
	by 刘潇 @ 2013-12-12
*/

#include "cocos2d.h"
#include "../XHelper/XHelper.h"

// 闪电落地粒子
#define XLHT_PART_MAX		(32)
// 天气系统粒子上限
#define XWTH_PART_MAX		(512)

class XWeather : 
	public XSingleton<XWeather>
{
public:
	enum {
		eNone		= 0,	// 未开启
		eSnow		= 1,	// 雪
		eBlizzard	= 2,	// 暴风雪
		eRain		= 3,	// 雨

		eTypeNum,

		eBGRain		= eTypeNum,	// 雨水落地水花
		eBGLight,				// 闪电落地碎光

		eBGEnd,
		eBGNum		= eBGEnd - eTypeNum,

		eLtXLen		= 204,
		eLtFrame	= 0x08,
		eLtParticle	= 0x04,
	};
public:
    XWeather(void)
	{
		m_byType = eNone;
		memset(m_achInitMark, 0, sizeof(char) * eTypeMarkLen);
		m_wActiveNum = 0;
		m_byLtFrame = 0x00;
		m_wLtSpace = 0x0000;
		m_byLtInited = 0x00;
		m_byLtActvNum = 0;
		m_byReload = 0x00;
		XMemLog::m_dwNew += sizeof(XWeather);

		//// Read PNG
		//FILE* pfImg = fopen("BGRain.png", "rb");
		//fseek(pfImg, 0, SEEK_END);
		//unsigned long dwSize = (unsigned long)ftell(pfImg);
		//unsigned char* abyBuf = new unsigned char[dwSize];
		//fseek(pfImg, 0, SEEK_SET);
		//unsigned long dwRead = (unsigned long)fread(abyBuf, 1, dwSize, pfImg);
		//fclose(pfImg);
		//pfImg = fopen("codes.txt", "w");
		//char* szW = "static unsigned char szDataBGRain[] = {\n";
		//fwrite(szW, 1, strlen(szW), pfImg);
		//char szWBuf[16];
		//for(unsigned long dwPos = 0; dwPos < dwRead; ++dwPos) {
		//	sprintf(szWBuf, "0x%02x, ", abyBuf[dwPos]);
		//	fwrite(szWBuf, 1, strlen(szWBuf), pfImg);
		//	if(0 == (dwPos + 1) % 8) {
		//		szW = "\n";
		//		fwrite(szW, 1, strlen(szW), pfImg);
		//	}
		//}
		//szW = "\n};";
		//fwrite(szW, 1, strlen(szW), pfImg);
		//fclose(pfImg);
	}
    ~XWeather(void)
	{
		XMemLog::m_dwNew -= sizeof(XWeather);
		for(int nPos = 0; nPos < eTypeNum; ++nPos)
			m_astSprPart[nPos].SafeRelease();
		for(int nPos = 0; nPos < eBGNum; ++nPos)
			m_astSprBG[nPos].SafeRelease();
	}
public:
	inline void Reload() {
		m_byReload = 0xFF;
		m_byLtInited = 0x00;
		Lightning(m_wLtSpace);
		memset(m_achInitMark, 0, sizeof(char) * eTypeMarkLen);
		unsigned char byType = m_byType;
		m_byType = eNone;
		SetType(byType);
		m_byReload = 0x00;
	}
	// 是否启用闪电【0否|否则代表每隔多少帧闪一次】
	inline void Lightning(unsigned short wFSpace) {
		m_wLtSpace = wFSpace;
		if(!m_wLtSpace)
			m_byLtFrame = 0;
		if(!m_byLtInited) {
			m_byLtInited = 0xFF;
			unsigned char* szData = NULL;
			unsigned long dwDataLen = 0;
			szData = GetDataSnow(dwDataLen);
			cocos2d::CCImage stImg;
			stImg.initWithImageData(szData, (int)dwDataLen, cocos2d::CCImage::kFmtPng);
			m_astTexBG[eBGLight - eTypeNum].initWithImage(&stImg);
			if(!m_byReload)
			{
				m_astTexBG[eBGLight - eTypeNum].retain();
				m_astSprBG[eBGLight - eTypeNum].initWithTexture(&m_astTexBG[eBGLight - eTypeNum], XLHT_PART_MAX);
				m_astSprBG[eBGLight - eTypeNum].retain();
				for(int nPos = XWTH_PART_MAX; nPos < XWTH_PART_MAX + XLHT_PART_MAX; ++nPos) {
					XWthPart& stP = m_astPart[nPos];
					stP.byType = eNone;
				}
			}
		}
	}
	inline unsigned char* GetDataSnow(unsigned long& dwDataLen) {
		static unsigned char szDataSnow[] = {
			0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 
			0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 
			0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 
			0x08, 0x06, 0x00, 0x00, 0x01, 0xb3, 0x08, 0x8e, 
			0x1d, 0x00, 0x00, 0x00, 0x86, 0x49, 0x44, 0x41, 
			0x54, 0x18, 0x95, 0x65, 0xce, 0x2b, 0x6e, 0x42, 
			0x61, 0x18, 0x84, 0xe1, 0xe7, 0x3b, 0x1c, 0xd3, 
			0x86, 0x15, 0x80, 0xa5, 0x1b, 0x00, 0xd3, 0x84, 
			0x2d, 0xe0, 0xd8, 0x13, 0xfb, 0x60, 0x0f, 0xb5, 
			0xf5, 0x18, 0x10, 0xc5, 0x54, 0x16, 0x83, 0x46, 
			0x55, 0x54, 0x30, 0x98, 0xff, 0xa4, 0x10, 0x5e, 
			0x39, 0x99, 0x1b, 0x90, 0xe4, 0x45, 0x92, 0x71, 
			0x92, 0x18, 0xa8, 0x24, 0x1b, 0xa8, 0xe6, 0x59, 
			0x63, 0x81, 0x7d, 0xe7, 0x91, 0xbf, 0xa1, 0x65, 
			0x94, 0xa4, 0x1b, 0xb2, 0x33, 0xac, 0x9b, 0x63, 
			0xdb, 0xe3, 0x0d, 0xab, 0x26, 0x7c, 0xf5, 0xf8, 
			0xc6, 0x47, 0x13, 0x3e, 0x6b, 0x68, 0x6b, 0xd1, 
			0x29, 0x5e, 0xf1, 0x8b, 0x9f, 0xaa, 0x3a, 0x55, 
			0x92, 0x1e, 0x4b, 0xbc, 0x63, 0x8e, 0x09, 0xce, 
			0x38, 0x60, 0xd7, 0xe1, 0xff, 0xfe, 0x33, 0x97, 
			0xfb, 0x89, 0x29, 0x66, 0x6d, 0xe2, 0x8c, 0x63, 
			0x55, 0x5d, 0x6f, 0xdd, 0xf3, 0x34, 0xb6, 0xfc, 
			0x9d, 0x27, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x49, 
			0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
		};
		dwDataLen = (unsigned long)(sizeof(szDataSnow) / sizeof(char));
		return szDataSnow;
	}
	inline void SetType(unsigned char byType) {
		if(byType == m_byType)
			return;
		if(eNone != byType && !m_wActiveNum)
			m_wActiveNum = 1;
		m_byType = byType;
		if(!XBIT_VALUE(m_achInitMark, byType))
		{
			XBIT_MARK(m_achInitMark, byType);
			unsigned char* szData = NULL;
			unsigned long dwDataLen = 0;
			if(eSnow == byType || eBlizzard == byType) {
				szData = GetDataSnow(dwDataLen);
			} else if(eRain == byType) {
				static unsigned char szDataBGRain[] = {
					0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 
					0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 
					0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 
					0x08, 0x06, 0x00, 0x00, 0x01, 0x87, 0x71, 0x4f, 
					0x01, 0x00, 0x00, 0x00, 0xcc, 0x49, 0x44, 0x41, 
					0x54, 0x28, 0x91, 0x8d, 0x90, 0xbf, 0x4a, 0xc2, 
					0x61, 0x14, 0x86, 0x9f, 0x23, 0xb8, 0x85, 0xbf, 
					0xcd, 0xc1, 0xa5, 0x8b, 0x08, 0x84, 0x26, 0x2f, 
					0x44, 0xc5, 0xad, 0xc5, 0x2d, 0xd0, 0x46, 0x17, 
					0xc7, 0x96, 0x86, 0x36, 0x05, 0x71, 0x6b, 0x68, 
					0xf6, 0x26, 0x74, 0xa9, 0xa0, 0xb9, 0x02, 0xc5, 
					0x45, 0x08, 0xac, 0xf5, 0x71, 0xe8, 0x13, 0x3e, 
					0x0c, 0xc5, 0x03, 0x07, 0x0e, 0xe7, 0xfd, 0x73, 
					0x5e, 0x0e, 0x00, 0xea, 0x04, 0xf5, 0x5e, 0x6d, 
					0xa0, 0xbe, 0x93, 0x97, 0x7a, 0xa1, 0x8e, 0xd5, 
					0x31, 0x87, 0xa5, 0x16, 0xa8, 0x37, 0x6a, 0xe5, 
					0x1f, 0xfa, 0x47, 0x98, 0xed, 0x87, 0x37, 0xb5, 
					0x9d, 0x2b, 0xd5, 0x91, 0x7a, 0x1b, 0x69, 0xd1, 
					0x00, 0x9a, 0x40, 0x39, 0x71, 0x36, 0xc0, 0x20, 
					0x22, 0xb6, 0xa8, 0xcf, 0x47, 0xec, 0x2b, 0x6a, 
					0x97, 0x63, 0xf7, 0x13, 0x69, 0x89, 0xda, 0x55, 
					0x1f, 0xd5, 0x22, 0x03, 0x3a, 0xea, 0x07, 0xc0, 
					0x3e, 0xc3, 0x15, 0xd0, 0x03, 0x9e, 0x80, 0x15, 
					0x50, 0x4a, 0x58, 0xe4, 0x86, 0xa9, 0x6b, 0x40, 
					0x0b, 0x18, 0x46, 0xc4, 0x1c, 0xf5, 0xf5, 0x54, 
					0x8c, 0x13, 0xf1, 0x0a, 0xf5, 0x33, 0xd4, 0x3e, 
					0x50, 0x05, 0x1e, 0x22, 0xe2, 0xeb, 0x4c, 0xf1, 
					0x25, 0x70, 0x07, 0xac, 0x0f, 0x81, 0x7a, 0xfa, 
					0xef, 0x42, 0xfd, 0x56, 0xb7, 0xea, 0xaf, 0xfa, 
					0xa3, 0xbe, 0xa8, 0x53, 0xf5, 0x3a, 0xd7, 0xec, 
					0x00, 0x28, 0xc9, 0xa4, 0x6f, 0xde, 0x65, 0x9b, 
					0x23, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 
					0x44, 0xae, 0x42, 0x60, 0x82
				};
				szData = szDataBGRain;
				dwDataLen = (unsigned long)(sizeof(szDataBGRain) / sizeof(char));
				if(szData && dwDataLen) {
					cocos2d::CCImage stImg;
					stImg.initWithImageData(szData, (int)dwDataLen, cocos2d::CCImage::kFmtPng);
					m_astTexBG[eBGRain - eTypeNum].initWithImage(&stImg);
					if(!m_byReload)
					{
						m_astTexBG[eBGRain - eTypeNum].retain();
						m_astSprBG[eBGRain - eTypeNum].initWithTexture(&m_astTexBG[eBGRain - eTypeNum], XWTH_PART_MAX);
						m_astSprBG[eBGRain - eTypeNum].retain();
					}
				}
				static unsigned char szDataRain[] = {
					0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 
					0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 
					0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 
					0x08, 0x06, 0x00, 0x00, 0x01, 0x5c, 0x8d, 0x0e, 
					0xeb, 0x00, 0x00, 0x00, 0xa4, 0x49, 0x44, 0x41, 
					0x54, 0x28, 0x91, 0x7d, 0xcf, 0x21, 0x52, 0xc2, 
					0x41, 0x18, 0xc6, 0xe1, 0x77, 0xe9, 0x44, 0xa3, 
					0x81, 0x42, 0x72, 0x18, 0x67, 0x98, 0x91, 0x60, 
					0x60, 0xac, 0x16, 0x0f, 0x41, 0xb2, 0x71, 0x00, 
					0x4f, 0x60, 0x24, 0x50, 0x08, 0x70, 0x03, 0x0a, 
					0x17, 0xe0, 0x10, 0x14, 0x8a, 0xc1, 0xe4, 0x58, 
					0x68, 0xc4, 0xc7, 0xc8, 0x7f, 0x16, 0xdc, 0x2f, 
					0xfe, 0xf6, 0xd9, 0xdd, 0xf9, 0x82, 0x71, 0x2e, 
					0x83, 0x5d, 0x1a, 0x83, 0xf9, 0xcd, 0x7a, 0x0e, 
					0x9e, 0x70, 0x57, 0x9f, 0xbc, 0x63, 0x58, 0xc7, 
					0x15, 0x26, 0x75, 0xfc, 0x0c, 0xfa, 0xdd, 0xd8, 
					0x4b, 0xf2, 0x52, 0x87, 0x82, 0xfb, 0xfa, 0xee, 
					0xa2, 0x2b, 0x92, 0xe4, 0x84, 0x51, 0xad, 0x0e, 
					0x5d, 0x91, 0x24, 0x5f, 0x98, 0xd6, 0x6a, 0x13, 
					0x3c, 0xe3, 0xf1, 0x7a, 0xd5, 0xcb, 0x27, 0xc7, 
					0x24, 0x0f, 0x78, 0xbd, 0x09, 0x4a, 0x29, 0xbf, 
					0x49, 0xf6, 0x49, 0xc6, 0x98, 0xfd, 0xf7, 0x52, 
					0x30, 0xc4, 0x1a, 0x1f, 0x2d, 0x34, 0xc2, 0x1e, 
					0xcb, 0x16, 0x9a, 0xe0, 0x1b, 0xdb, 0x16, 0x9a, 
					0xe2, 0x07, 0xab, 0xd2, 0x40, 0x83, 0x24, 0x6f, 
					0x7f, 0x99, 0x27, 0x7c, 0x9a, 0xa4, 0x3c, 0x41, 
					0x7e, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 
					0x44, 0xae, 0x42, 0x60, 0x82
				};
				szData = szDataRain;
				dwDataLen = (unsigned long)(sizeof(szDataRain) / sizeof(char));
			}
			if(szData && dwDataLen) {
				cocos2d::CCImage stImg;
				stImg.initWithImageData(szData, (int)dwDataLen, cocos2d::CCImage::kFmtPng);
				m_astTexPart[byType].initWithImage(&stImg);
				if(!m_byReload)
				{
					m_astTexPart[byType].retain();
					m_astSprPart[byType].initWithTexture(&m_astTexPart[byType], XWTH_PART_MAX);
					m_astSprPart[byType].retain();
				}
			}
		}
	}
	inline void Run(short shOffsetX, float fT = 0.03f) {
		if(m_wLtSpace && !m_byLtFrame)
		{
			if(0x00 == rand() % m_wLtSpace) {
				m_byLtFrame = eLtFrame;
				m_wLtPosX = eLtXLen + rand() % (SCR_W - (eLtXLen << 1));
				m_wLtPosY = rand() % (SCR_H >> 1);
			}
		}
		// 闪电落地碎光 : 计算
		if(m_byLtActvNum) {
			m_byLtActvNum = 0;
			unsigned char byCA, byCAOld;
			float fX, fY, fScale;
			for(int nPos = XWTH_PART_MAX; nPos < XWTH_PART_MAX + XLHT_PART_MAX; ++nPos) {
				XWthPart& stP = m_astPart[nPos];
				if(eNone == stP.byType)
					continue;
				byCA = (unsigned char)stP.stSpr.getOpacity();
				byCAOld = byCA;
				if(byCA > 0x08)
					byCA -= 0x08;
				else {
					byCA = 0x00;
					stP.stSpr.removeFromParentAndCleanup(false);
					stP.byType = eNone;
				}
				if(!byCA)
					continue;
				++m_byLtActvNum;
				if(stP.wPosZ) {
					stP.stSpr.getPosition(&fX, &fY);
					fX += (stP.fSpdX * fT);
					fX += shOffsetX;
					if(fX <= 0.f)
						fX = SCR_W;
					if(fX > SCR_W)
						fX = 0.f;
					stP.fSpdY += (-90.f);
					fY += (stP.fSpdY * fT);
					fScale = stP.stSpr.getScaleY();
					fScale *= 0.98f;
					stP.stSpr.setScale(fScale);
					stP.stSpr.setPosition(ccp(fX, fY));
					stP.stSpr.setOpacity(byCA);
				} else {
					stP.stSpr.getPosition(&fX, &fY);
					fX += shOffsetX;
					if(fX <= 0.f)
						fX = SCR_W;
					if(fX > SCR_W)
						fX = 0.f;
					fScale = stP.stSpr.getScaleY();
					fScale *= 0.8f;
					stP.stSpr.setScale(fScale);
					stP.stSpr.setPosition(ccp(fX, fY));
					stP.stSpr.setOpacity(byCA);
				}
			}
		}
		// 闪电落地碎光 : 初始化
		if(m_byLtFrame) {
			m_byLtActvNum = XLHT_PART_MAX;
			unsigned char byParticle = 0;
			for(int nPos = XWTH_PART_MAX; nPos < XWTH_PART_MAX + XLHT_PART_MAX; ++nPos) {
				XWthPart& stP = m_astPart[nPos];
				if(eNone != stP.byType)
					continue;
				// 贴图初始化
				stP.byType = eBGLight;
				stP.stSpr.initWithTexture(m_astSprBG[stP.byType - eTypeNum].getTexture());
				if(stP.stSpr.getParent())
					stP.stSpr.removeFromParentAndCleanup(false);
				m_astSprBG[stP.byType - eTypeNum].addChild(&stP.stSpr);
				// 坐标及速度
				float fScale;
				if(!byParticle)
				{
					stP.wPosZ = 0;
					stP.fSpdX = 0.f;
					stP.fSpdY = 0.f;
					fScale = 10.f + (rand() % 41) * 1.f;
					stP.stSpr.setScale(fScale);
					stP.stSpr.setPosition(ccp(m_wLtXEd, m_wLtYEd));
					stP.stSpr.setOpacity(0x0F + rand() % 0x0F);
					stP.stSpr.setColor(cocos2d::ccc3(0xFF, 0xFF, 0xFF));
				}
				else
				{
					stP.wPosZ = m_wLtYEd;
					stP.fSpdX = 200.f - rand() % 401;
					stP.fSpdY = rand() % 501 + 500.f;
					fScale = 0.5 + (rand() % 6) * 0.1f;
					stP.stSpr.setScale(fScale);
					stP.stSpr.setPosition(ccp(m_wLtXEd, m_wLtYEd));
					stP.stSpr.setOpacity(0xBF + rand() % 0x40);
					stP.stSpr.setColor(cocos2d::ccc3(0xB0, 0xD0, 0xFF));
				}
				++byParticle;
				if(byParticle >= eLtParticle)
					break;
			}
		}
		if(!m_wActiveNum)
			return;
		unsigned char byCA, byCAOld;
		float fX, fY, fScale;
		bool bBG = false;
		if(eNone == m_byType)
			m_wActiveNum = 0;
		for(int nPos = 0; nPos < XWTH_PART_MAX; ++nPos) {
			XWthPart& stP = m_astPart[nPos];
			stP.stSpr.getPosition(&fX, &fY);
			fY -= stP.wPosZ;
			if(eNone == m_byType && eNone == stP.byType)
				continue;
			byCA = (unsigned char)stP.stSpr.getOpacity();
			fScale = stP.stSpr.getScaleY();
			if(fY < 0.f) {
				bBG = false;
				if(eRain == stP.byType) {
					// 20%的概率创建水花
					if(rand() % 10 < 2) {
						bBG = true;
					}
				}
				if(stP.byType != m_byType || bBG) {
					if(eNone != stP.byType) {
						if(stP.stSpr.getParent())
							stP.stSpr.removeFromParentAndCleanup(false);
					}
					if(bBG) {
						if(eRain == stP.byType) {
							stP.byType = eBGRain;
						}
						stP.stSpr.initWithTexture(m_astSprBG[stP.byType - eTypeNum].getTexture());
						if(stP.stSpr.getParent())
							stP.stSpr.removeFromParentAndCleanup(false);
						m_astSprBG[stP.byType - eTypeNum].addChild(&stP.stSpr);
					} else {
						if(eNone != m_byType) {
							stP.byType = m_byType;
							stP.stSpr.initWithTexture(m_astSprPart[stP.byType].getTexture());
							if(stP.stSpr.getParent())
								stP.stSpr.removeFromParentAndCleanup(false);
							m_astSprPart[stP.byType].addChild(&stP.stSpr);
						}
					}
				}
				if(eSnow == m_byType) {
					fX = rand() % SCR_W;
					fY = SCR_H;
					stP.wPosZ = rand() % (SCR_H >> 1);
					stP.fSpdX = 50.f - rand() % 100;
					stP.fSpdY = -10.f - rand() % 90;
					fScale = (1 + rand() % 20) * 0.1f;
					byCA = 0;
					stP.stSpr.setScale(fScale);
				} else if(eBlizzard == m_byType) {
					fX = rand() % SCR_W;
					fY = SCR_H;
					stP.wPosZ = rand() % (SCR_H >> 1);
					stP.fSpdX = 800.f - rand() % 400;
					stP.fSpdY = -50.f - rand() % 50;
					fScale = (1 + rand() % 12) * 0.1f;
					byCA = 0;
					stP.stSpr.setScale(fScale);
				} else if(eRain == m_byType) {
					if(bBG) {
						// XZ为雨水落地位置不变
						fY = 0.1f;
						stP.fSpdX = 0.f;
						stP.fSpdY = 0.f;
						fScale = (5 + rand() % 11) * 0.1f;
						byCA = 0x7F;
						stP.stSpr.setScale(fScale);
					} else {
						fX = rand() % SCR_W;
						fY = SCR_H;
						stP.wPosZ = rand() % (SCR_H >> 1);
						stP.fSpdX = rand() % 100;
						stP.fSpdY = 0.f - rand() % 100;
						fScale = (10 + rand() % 11) * 0.1f;
						byCA = 0;
						stP.stSpr.setScaleX(1.f);
						stP.stSpr.setScaleY(fScale);
						//stP.stSpr.setColor(cocos2d::ccc3(0xFF, 0x80, 0x00));
					}
				}
			} else {
				if(eNone == m_byType)
					++m_wActiveNum;
				if(eSnow == stP.byType || eBlizzard == stP.byType) {
					if(eSnow == stP.byType && eBlizzard == m_byType) {
						stP.fSpdX = 800.f - rand() % 400;
						stP.fSpdY = -50.f - rand() % 50;
						stP.byType = eBlizzard;
					}
					fX += (stP.fSpdX * fT);
					fY += (stP.fSpdY * fT);
					if(byCA < 0xFF) {
						byCAOld = byCA;
						byCA += 40;
						if(byCA < byCAOld)
							byCA = 0xFF;
					}
				} else if(eRain == stP.byType) {
					fX += (stP.fSpdX * fT);
					stP.fSpdY += (-90.f);
					fY += (stP.fSpdY * fT);
					if(byCA < 0xFF)
						byCA += 5;
				} else if(eBGRain == stP.byType) {
					if(byCA) {
						byCAOld = byCA;
						byCA -= 0x0F;
						if(byCA >= byCAOld)
							byCA = 0x00;
						fScale += 0.1f;
						stP.stSpr.setScale(fScale);
					}
					if(!byCA)
						fY = -1.f;
				}
				fX += shOffsetX;
				if(fX <= 0.f)
					fX = SCR_W;
				if(fX > SCR_W)
					fX = 0.f;
			}
			stP.stSpr.setPosition(ccp(fX, fY + stP.wPosZ));
			stP.stSpr.setOpacity(byCA);
		}
	}
	inline void DrawBack() {
		if(!m_wActiveNum)
			return;
		if(eNone == m_byType)
			return;
		if(eRain == m_byType) {
			if(m_astSprBG[eBGRain - eTypeNum].getChildrenCount())
				m_astSprBG[eBGRain - eTypeNum].visit();
		}
	}
	inline void DrawFront() {
		if(m_byLtActvNum || (m_wLtSpace && m_byLtFrame)) {
			// 闪电，饱和叠加
			cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE);
			if(m_wLtSpace && m_byLtFrame) {
				--m_byLtFrame;
				unsigned short wDisplace = rand() % 100 + 100;
				LightningTo(m_wLtPosX, SCR_H, m_wLtPosX, m_wLtPosY, wDisplace);
				// 透明度
				unsigned char byAlpha = (eLtFrame >> 1);
				if(byAlpha)
					byAlpha = (unsigned char)((float)DIFF(byAlpha, m_byLtFrame) / byAlpha * 32.f);
				// 场景变亮
				const float fColor = 1.f / 255.f;
				cocos2d::ccColor4F c4 = {0x80 * fColor, 0xC0 * fColor, 0xFF * fColor, byAlpha * fColor};
				cocos2d::ccDrawSolidRect(ccp(0, 0), ccp(SCR_W, SCR_H), c4);
			}
			if(m_byLtActvNum) {
				if(m_astSprBG[eBGLight - eTypeNum].getChildrenCount())
					m_astSprBG[eBGLight - eTypeNum].visit();
			}
			// 还原
			cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		if(!m_wActiveNum)
			return;
		if(eNone == m_byType)
			return;
		for(int nPos = 0; nPos < eTypeNum; ++nPos) {
			if(m_astSprPart[nPos].getChildrenCount())
				m_astSprPart[nPos].visit();
		}
	}
public:
	// 从哪里到哪里，闪电由多少个小线段组成
	inline void LightningTo(unsigned short wXSt, unsigned short wYSt, unsigned short wXEd, unsigned short wYEd, unsigned short wDisplace) {
		m_byBranch = rand() % 0x4F;
		float fThick = (float)(rand() % 5 + 4);
		m_wLtXEd = wXEd;
		m_wLtYEd = wYEd;
		glEnable(0x0B20);
		glHint(0x0C52, 0x1102);
		DrawLightning(wXSt, wYSt, wXEd, wYEd, wDisplace, fThick, 0.f);
	}
protected:
	inline void DrawLightning(unsigned short wXSt, unsigned short wYSt, unsigned short wXEd, unsigned short wYEd, unsigned short wDisplace, float fThickSt, float fThickEd) {
		// 宽度中心值
		float fThickMid = (fThickSt + fThickEd) * 0.5f;
		if (wDisplace < 4) {
			glLineWidth(fThickMid * 4.f);
			cocos2d::ccDrawColor4B(0x00, 0x00, 0xFF, 0x20);
			cocos2d::ccDrawLine(ccp(wXSt, wYSt), ccp(wXEd, wYEd));
			glLineWidth(fThickMid * 2.f);
			cocos2d::ccDrawColor4B(0x80, 0xC0, 0xFF, 0x40);
			cocos2d::ccDrawLine(ccp(wXSt, wYSt), ccp(wXEd, wYEd));
			glLineWidth(fThickMid);
			cocos2d::ccDrawColor4B(0x80, 0xC0, 0xFF, 0x60);
			cocos2d::ccDrawLine(ccp(wXSt, wYSt), ccp(wXEd, wYEd));
			glLineWidth(fThickMid * 0.5f);
			cocos2d::ccDrawColor4B(0xFF, 0xFF, 0xFF, 0xFF);
			cocos2d::ccDrawLine(ccp(wXSt, wYSt), ccp(wXEd, wYEd));
		}
		else {
			// 取中心点
			short nXMid = ((wXSt + wXEd) >> 1);
			short nYMid = ((wYSt + wYEd) >> 1);
			// 中心点随机偏移
			unsigned char byOdds = 0;
			byOdds = rand() % 21 + 10;
			if(rand() % 2)
				nXMid += (short)((float)byOdds * 0.01f * wDisplace);
			else
				nXMid -= (short)((float)byOdds * 0.01f * wDisplace);
			if(nXMid < 0)
				nXMid = rand() % wDisplace;
			unsigned short wXMid = (unsigned short)nXMid;
			byOdds = rand() % 21 + 10;
			if(rand() % 2)
				nYMid += (short)((float)byOdds * 0.01f * wDisplace);
			else
				nYMid -= (short)((float)byOdds * 0.01f * wDisplace);
			if(nYMid < 0)
				nYMid = rand() % wDisplace;
			unsigned short wYMid = (unsigned short)nYMid;
			// 线段数减半
			wDisplace >>= 1;
			// 从起点到中心点
			DrawLightning(wXSt, wYSt, wXMid, wYMid, wDisplace, fThickSt, fThickMid);
			// 从中心点到结束点
			DrawLightning(wXMid, wYMid, wXEd, wYEd, wDisplace, fThickMid, fThickEd);
			// 从中心点分叉
			if(m_byBranch && 0 == rand() % 0x08) {
				--m_byBranch;
				byOdds = rand() % 21 + 10;
				if(wXMid >= wXEd) {
					int nXEd = (int)wXMid - (int)((float)byOdds * 0.1f * wDisplace);
					if(nXEd < 0)
						nXEd = 0;
					wXEd = (unsigned short)nXEd;
				} else
					wXEd = wXMid + (unsigned short)((float)byOdds * 0.1f * wDisplace);
				byOdds = rand() % 21 + 10;
				if(wYMid >= wYEd) {
					int nYEd = (int)wYMid - (int)((float)byOdds * 0.1f * wDisplace);
					if(nYEd < 0)
						nYEd = 0;
					wYEd = (unsigned short)nYEd;
				} else
					wYEd = wYMid + (unsigned short)((float)byOdds * 0.1f * wDisplace);
				wDisplace += rand() % wDisplace;
				DrawLightning(wXMid, wYMid, wXEd, wYEd, wDisplace, fThickMid, 0.f);
			}
		}
	}
protected:
	unsigned char				m_byType;
	enum{ eTypeMarkLen = ((eTypeNum + 7) >> 3) };
	char						m_achInitMark[eTypeMarkLen];
	cocos2d::CCTexture2D		m_astTexPart[eTypeNum];
	cocos2d::CCSpriteBatchNode	m_astSprPart[eTypeNum];
	cocos2d::CCTexture2D		m_astTexBG[eBGNum];
	cocos2d::CCSpriteBatchNode	m_astSprBG[eBGNum];
	struct XWthPart {
		unsigned char			byType;
		unsigned short			wPosZ;
		float					fSpdX, fSpdY;
		cocos2d::CCSprite		stSpr;
		XWthPart() : byType(0), wPosZ(0), fSpdX(0.f), fSpdY(0.f) {
			stSpr.setPositionY(-1.f);
		}
	}							m_astPart[XWTH_PART_MAX + XLHT_PART_MAX];
	unsigned short				m_wActiveNum;
	// for Lightning
	unsigned short				m_wLtSpace;		// 闪电间隔帧数【多少帧一次】
	unsigned short				m_wLtPosX;
	unsigned short				m_wLtPosY;
	unsigned char				m_byLtFrame;
	unsigned char				m_byBranch;
	unsigned short				m_wLtXEd;
	unsigned short				m_wLtYEd;
	unsigned char				m_byLtInited;
	unsigned char				m_byLtActvNum;
	// for reload
	unsigned char				m_byReload;
};

#endif // __XWEATHER_H__
