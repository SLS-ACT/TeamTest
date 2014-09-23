#ifndef __XA8READER_H__
#define __XA8READER_H__
/*
	XANI 8Bit 文件格式定义及描述
	by 刘潇 @ 2014-05-29
*/

#define XLZW_ROOTBIT		(8)							// 根位数
#define XLZW_ROOTNUM		(1 << XLZW_ROOTBIT)			// 根元素数目
#define XLZWDIC_BITMAX		(12)						// 自定义字典ID最大Bit数
#define XLZWDIC_WORDMAX		(1 << XLZWDIC_BITMAX)		// 自定义字典ID最大数目
#define XLZW_CLR			XLZW_ROOTNUM				// 清除标记
#define XLZW_END			(XLZW_CLR + 1)				// 结束标记
#define XLZW_IDX			(XLZW_END + 1)				// 自定义字典ID起始标记
struct XLZWDicR {
	int	nData;
	int	nLen;
	XLZWDicR() : nData(0), nLen(1) {}
};
class XLZWReader
{
public:
	unsigned char*	m_abyData;
	int				m_nLen;
public:
	inline void Close() {
		if(m_abyData) {
			delete[] m_abyData;
			m_abyData = NULL;
		}
		m_nLen = 0;
	}
	inline bool Uncompress(const unsigned char* abyDataIn, int nLenIn) {
		Close();
		// 自定义字典ID的起始值
		unsigned short wCurIdx = XLZW_IDX;
		// 当前编码长度
		unsigned char byCurIdxBit = XLZW_ROOTBIT + 1;
		// 当前编码长度下的最大值
		unsigned short wCurIdxMax = (1 << byCurIdxBit);
		// 当前使用的bit数
		unsigned long dwUsedBits = 0;
		// 准备解压缩
		unsigned short wHashKey;
		unsigned short wBufUsed = 0;
		unsigned char abyBuf[XLZWDIC_WORDMAX];
		// 创建一个4倍的临时缓冲，如不足则每次再增加一倍
		int nTmpLen = nLenIn << 3;
		m_abyData = new unsigned char[nTmpLen];
		if(!m_abyData)
			return false;
		int nWrite = 0;
		// 解压数据
		do 
		{
			ReadData((unsigned char*)abyDataIn, dwUsedBits, byCurIdxBit, wHashKey);
			// 是否结束
			if(XLZW_END == wHashKey)
				return true;
			// 字典是否需要重置
			if(XLZW_CLR == wHashKey) {
				// 字典重置
				wCurIdx = XLZW_IDX;
				byCurIdxBit = XLZW_ROOTBIT + 1;
				wCurIdxMax = (1 << byCurIdxBit);
				wBufUsed = 0;
				continue;
			}
			if(wHashKey >= XLZWDIC_WORDMAX) {
				// 错误
				return false;
			}
			// 解析
			if(wHashKey < wCurIdx) {
				// 上次使用的字符串加上当前Key加入字典
				if(wBufUsed) {
					abyBuf[wBufUsed++] = (wHashKey < XLZW_ROOTNUM ? (unsigned char)wHashKey : m_abyData[m_astDic[wHashKey].nData]);
					// 加入字典
					XLZWDicR& stCurDic = m_astDic[wCurIdx++];
					stCurDic.nLen = wBufUsed;
					stCurDic.nData = nWrite;
				}
				// 还原匹配缓冲区
				wBufUsed = (unsigned short)m_astDic[wHashKey].nLen;
				if(wHashKey < XLZW_ROOTNUM)
					abyBuf[0] = (unsigned char)wHashKey;
				else
					memcpy(abyBuf, &m_abyData[m_astDic[wHashKey].nData], wBufUsed);
			} else {
				abyBuf[wBufUsed++] = abyBuf[0];
				// 加入字典
				XLZWDicR& stCurDic = m_astDic[wHashKey];
				stCurDic.nLen = wBufUsed;
				stCurDic.nData = m_nLen;
				wCurIdx = wHashKey + 1;
			}
			// 输出szBuf
			nWrite = m_nLen;
			for(int nD = 0; nD < wBufUsed; ++nD) {
				m_abyData[m_nLen++] = abyBuf[nD];
				if(m_nLen >= nTmpLen) {
					nTmpLen <<= 1;
					unsigned char* abyNewBuf = new unsigned char[nTmpLen];
					if(!abyNewBuf)
						return false;
					memcpy(abyNewBuf, m_abyData, m_nLen);
					delete[] m_abyData;
					m_abyData = abyNewBuf;
				}
			}
			// 检查字典ID使用的bit位是否需要增加
			if(wCurIdx >= wCurIdxMax && byCurIdxBit < XLZWDIC_BITMAX) {
				// 位数及上限增加
				++byCurIdxBit;
				wCurIdxMax = (1 << byCurIdxBit);
			}
		} while (true);
		return false;
	}
public:
	XLZWReader() {
		m_abyData = NULL;
		m_nLen = 0;
	}
	~XLZWReader() {
		Close();
	}
protected:
	inline void ReadData(const unsigned char* abyBuf, unsigned long& dwUsedBits, unsigned char byCurValBits, unsigned short& wVal) {
		wVal = 0;
		unsigned char byVal;
		unsigned long dwBytePos;
		unsigned char byBits, byBitsRead = 0;
		while(byCurValBits) {
			// 使用第几个Byte
			dwBytePos = (dwUsedBits >> 3);
			// 当前Byte读取的Bit数
			byBits = dwUsedBits % 8;
			// 格式化剩余的数值
			byVal = (abyBuf[dwBytePos] >> byBits);
			// 把数据写入wVal
			if(byCurValBits < 8) {
				wVal |= ((unsigned short)(((byVal << (8 - byCurValBits)) & 0xFF) >> (8 - byCurValBits)) << byBitsRead);
				// 缓冲区总共读取的Bit数
				dwUsedBits += byCurValBits;
				byCurValBits = 0;
			}
			else {
				wVal |= (((unsigned short)byVal) << byBitsRead);
				// 已写入的Bit数
				byBits = 8 - byBits;
				// 剩余的Bit数
				byCurValBits -= byBits;
				// wVal中总共已读取的Bit数
				byBitsRead += byBits;
				// 缓冲区总共读取的Bit数
				dwUsedBits += byBits;
			}
		}
	}
protected:
	XLZWDicR		m_astDic[XLZWDIC_WORDMAX];
};
struct XA8Header
{
	unsigned char	abySign[4];		// Signature : 'X''A''8''b'
	unsigned long	dwVersion;		// Version
};
#define XA8b_PAL		(256)
#define XA8b_PAL_BYTE	((XA8b_PAL << 2) - XA8b_PAL)
struct XA8ScrDesc
{
	unsigned short	wWidth;			// 像素数，定义GIF图象的宽度
	unsigned short	wHeight;		// 像素数，定义GIF图象的高度
	unsigned char	abyPal[XA8b_PAL_BYTE];	// 调色板
	unsigned short	wFrameNum;		// 帧数
	unsigned long*	adwFrameOffset;	// 每帧的起始偏移【从XAniHeader之前开始计算】
};
struct XA8Frame
{
	//@ST - 必须限定在逻辑屏幕尺寸范围内【左下角为原点】
	unsigned short	wLeft;			// X方向偏移量
	unsigned short	wTop;			// Y方向偏移量
	unsigned short	wWidth;			// 有效像素宽度
	unsigned short	wHeight;		// 有效像素高度
	//@ED - 必须限定在逻辑屏幕尺寸范围内
	unsigned long	dwData;			// 数据长度
	unsigned char*	abyData;		// 数据[每Byte代表调色板索引]
	unsigned long	dwDataA;		// Alpha数据长度
	unsigned char*	abyDataA;		// Alpha数据[每Byte代表透明度]
	XA8Frame() : abyData(NULL), abyDataA(NULL) {}
};
union X4B
{
	unsigned char abyVal[4];
	unsigned long dwVal;
};
union X2B
{
	unsigned char abyVal[2];
	unsigned short wVal;
};
class XA8Reader
{
public:
	char		szErr[64];
public:
	XA8Header	stHeader;
	XA8ScrDesc	stScrDesc;
	XA8Frame*	astFrames;
public:
	inline void Close() {
		if(stScrDesc.adwFrameOffset) {
			delete[] stScrDesc.adwFrameOffset;
			stScrDesc.adwFrameOffset = NULL;
		}
		if(astFrames) {
			for(int nPos = 0; nPos < (int)stScrDesc.wFrameNum; ++nPos) {
				if(astFrames[nPos].abyData) {
					delete[] astFrames[nPos].abyData;
					astFrames[nPos].abyData = NULL;
				}
				if(astFrames[nPos].abyDataA) {
					delete[] astFrames[nPos].abyDataA;
					astFrames[nPos].abyDataA = NULL;
				}
			}
			delete[] astFrames;
			astFrames = NULL;
		}
	}
	inline bool ReadHeader(unsigned char* abyBuf, int nLen) {
		int nUsed = 0;
		X4B u4B;
		X2B u2B;
		// XA8Header
		stHeader.abySign[0] = abyBuf[nUsed++];
		stHeader.abySign[1] = abyBuf[nUsed++];
		stHeader.abySign[2] = abyBuf[nUsed++];
		stHeader.abySign[3] = abyBuf[nUsed++];
		if('X' != stHeader.abySign[0] || 'A' != stHeader.abySign[1] || '8' != stHeader.abySign[2] || 'b' != stHeader.abySign[3]) {
			sprintf(szErr, "ReadHeader : Invalid XA8.");
			return false;
		}
		u4B.abyVal[0] = abyBuf[nUsed++];
		u4B.abyVal[1] = abyBuf[nUsed++];
		u4B.abyVal[2] = abyBuf[nUsed++];
		u4B.abyVal[3] = abyBuf[nUsed++];
		stHeader.dwVersion = u4B.dwVal;
		// XA8ScrDesc
		u2B.abyVal[0] = abyBuf[nUsed++];
		u2B.abyVal[1] = abyBuf[nUsed++];
		stScrDesc.wWidth = u2B.wVal;
		u2B.abyVal[0] = abyBuf[nUsed++];
		u2B.abyVal[1] = abyBuf[nUsed++];
		stScrDesc.wHeight = u2B.wVal;
		memcpy(stScrDesc.abyPal, &abyBuf[nUsed], sizeof(unsigned char) * XA8b_PAL_BYTE);
		nUsed += (sizeof(unsigned char) * XA8b_PAL_BYTE);
		u2B.abyVal[0] = abyBuf[nUsed++];
		u2B.abyVal[1] = abyBuf[nUsed++];
		stScrDesc.wFrameNum = u2B.wVal;
		if(!stScrDesc.wFrameNum) {
			sprintf(szErr, "ReadHeader : Frame num is 0.");
			return false;
		}
		stScrDesc.adwFrameOffset = new unsigned long[stScrDesc.wFrameNum];
		if(!stScrDesc.adwFrameOffset) {
			sprintf(szErr, "ReadHeader : Frame offset new %d failed.", stScrDesc.wFrameNum);
			return false;
		}
		for(unsigned long dwPos = 0; dwPos < stScrDesc.wFrameNum; ++dwPos) {
			u4B.abyVal[0] = abyBuf[nUsed++];
			u4B.abyVal[1] = abyBuf[nUsed++];
			u4B.abyVal[2] = abyBuf[nUsed++];
			u4B.abyVal[3] = abyBuf[nUsed++];
			stScrDesc.adwFrameOffset[dwPos] = u4B.dwVal;
		}
		// XA8Frame
		astFrames = new XA8Frame[stScrDesc.wFrameNum];
		if(!astFrames) {
			sprintf(szErr, "ReadHeader : Frame new %d failed.", stScrDesc.wFrameNum);
			return false;
		}
		return true;
	}
	inline bool ReadFrame(unsigned char* abyBuf, int nLen, unsigned short wFrame) {
		if(wFrame >= stScrDesc.wFrameNum) {
			sprintf(szErr, "ReadFrame : Frame %d more than max %d.", wFrame, stScrDesc.wFrameNum);
			return false;
		}
		X4B u4B;
		X2B u2B;
		unsigned long dwUsed = stScrDesc.adwFrameOffset[wFrame];
		XA8Frame& stFrm = astFrames[wFrame];
		u2B.abyVal[0] = abyBuf[dwUsed++];
		u2B.abyVal[1] = abyBuf[dwUsed++];
		stFrm.wLeft = u2B.wVal;
		u2B.abyVal[0] = abyBuf[dwUsed++];
		u2B.abyVal[1] = abyBuf[dwUsed++];
		stFrm.wTop = u2B.wVal;
		u2B.abyVal[0] = abyBuf[dwUsed++];
		u2B.abyVal[1] = abyBuf[dwUsed++];
		stFrm.wWidth = u2B.wVal;
		u2B.abyVal[0] = abyBuf[dwUsed++];
		u2B.abyVal[1] = abyBuf[dwUsed++];
		stFrm.wHeight = u2B.wVal;
		XLZWReader stR;
		bool bRes;
		u4B.abyVal[0] = abyBuf[dwUsed++];
		u4B.abyVal[1] = abyBuf[dwUsed++];
		u4B.abyVal[2] = abyBuf[dwUsed++];
		u4B.abyVal[3] = abyBuf[dwUsed++];
		stFrm.dwData = u4B.dwVal;
		if(stFrm.dwData)
		{
			bRes = stR.Uncompress(&abyBuf[dwUsed], (int)stFrm.dwData);
			if(!bRes) {
				sprintf(szErr, "ReadFrame[%d] : Uncompress failed", wFrame);
				return false;
			}
			dwUsed += stFrm.dwData;
			stFrm.dwData = (unsigned long)stR.m_nLen;
			stFrm.abyData = new unsigned char[stFrm.dwData];
			if(!stFrm.dwData || !stFrm.abyData) {
				sprintf(szErr, "ReadFrame[%d] : Invalid %p or %d", wFrame, stFrm.abyData, stFrm.dwData);
				return false;
			}
			if(stFrm.dwData != stFrm.wWidth * stFrm.wHeight) {
				sprintf(szErr, "ReadFrame[%d] : %d != %d * %d", stFrm.dwData, stFrm.wWidth, stFrm.wHeight);
				return false;
			}
			memcpy(stFrm.abyData, stR.m_abyData, stFrm.dwData);
		}
		else
			stFrm.abyData = NULL;
		u4B.abyVal[0] = abyBuf[dwUsed++];
		u4B.abyVal[1] = abyBuf[dwUsed++];
		u4B.abyVal[2] = abyBuf[dwUsed++];
		u4B.abyVal[3] = abyBuf[dwUsed++];
		stFrm.dwDataA = u4B.dwVal;
		if(stFrm.dwDataA)
		{
			bRes = stR.Uncompress(&abyBuf[dwUsed], (int)stFrm.dwDataA);
			if(!bRes) {
				sprintf(szErr, "ReadFrame[%dA] : Uncompress failed", wFrame);
				return false;
			}
			dwUsed += stFrm.dwDataA;
			stFrm.dwDataA = (unsigned long)stR.m_nLen;
			stFrm.abyDataA = new unsigned char[stFrm.dwData];
			if(!stFrm.dwDataA || !stFrm.abyDataA) {
				sprintf(szErr, "ReadFrame[%dA] : Invalid %p or %d", wFrame, stFrm.abyDataA, stFrm.dwDataA);
				return false;
			}
			if(stFrm.dwDataA != stFrm.wWidth * stFrm.wHeight) {
				sprintf(szErr, "ReadFrame[%dA] : %d != %d * %d", stFrm.dwDataA, stFrm.wWidth, stFrm.wHeight);
				return false;
			}
			memcpy(stFrm.abyDataA, stR.m_abyData, stFrm.dwDataA);
		}
		else
			stFrm.abyDataA = NULL;
		return true;
	}
	inline bool Read(unsigned char* abyBuf, int nLen) {
		bool bRes = ReadHeader(abyBuf, nLen);
		if(!bRes)
			return false;
		for(unsigned long dwPos = 0; dwPos < stScrDesc.wFrameNum; ++dwPos) {
			bRes = ReadFrame(abyBuf, nLen, (unsigned short)dwPos);
			if(!bRes)
				return false;
		}
		return true;
	}
public:
	XA8Reader() {
		memset(&stScrDesc, 0, sizeof(XA8ScrDesc));
		astFrames = NULL;
	}
	~XA8Reader() {
		Close();
	}
};

#endif