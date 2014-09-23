#include "MaskWord.h"

XCPP_DEFINED();

signed char MaskWord::pMaskWordBeginKey[FIRST_CHAR_MAX_INDEX] = {0};
short MaskWord::pMaskWordBeginKeyMaxLen[FIRST_CHAR_MAX_INDEX] = {0};
short MaskWord::pMaskWordBeginKeyMinLen[FIRST_CHAR_MAX_INDEX] = {0};
int MaskWord::pMaskWordBeginKeyNum[FIRST_CHAR_MAX_INDEX] = {0};
unsigned int** MaskWord::pMaskWordHashValue = NULL;

MaskWord::MaskWord()
{

}
MaskWord::~MaskWord()
{

}

void MaskWord::InitMaskWord(char *szPath)
{
	XFUNC_START();
	unsigned long dwFileSize;
	unsigned char* pReadBuf = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(szPath, "rb", &dwFileSize);
	unsigned char* pStr = (unsigned char*)pReadBuf;
	if(NULL == pStr)
	{
		CCLOG("[MaskWord::InitMaskWord]警告：[FILE]打开文件“%s”失败", szPath);
		return;
	}
	if(!dwFileSize || INVALID_ID == dwFileSize)
	{
		printf("[MaskWord::InitMaskWord]警告：[FILE]获取文件“%s”大小失败", szPath);
		return;
	}
	memset(pMaskWordBeginKey,0,sizeof(signed char)*FIRST_CHAR_MAX_INDEX);
	memset(pMaskWordBeginKeyMaxLen,0,sizeof(short)*FIRST_CHAR_MAX_INDEX);
	memset(pMaskWordBeginKeyMinLen,0,sizeof(short)*FIRST_CHAR_MAX_INDEX);
	memset(pMaskWordBeginKeyNum,0,sizeof(int)*FIRST_CHAR_MAX_INDEX);
	pMaskWordHashValue = new unsigned int*[FIRST_CHAR_MAX_INDEX];
	if(!pMaskWordHashValue)
		return;
	for (int i = 0;i<FIRST_CHAR_MAX_INDEX;++i)
	{
		pMaskWordHashValue[i] = NULL;
	}
	/*
	byIndex 的 0～3 依次为 mapIndex、 最大长度、最短长度、个数nNum 
	4 ～ nNum + 4  依次为哈希值
	*/
	int byIndex = 0;
	signed char szNum[100] = {'\0'};
	unsigned char byNumIndex = 0;
	bool bIsStruct = true;
	int curmapIndex = 0;
	int curmapIndexHashNum = 0;
	while('\0'!= *pStr)
	{
		signed char charNum = *pStr;
		if('0'<= charNum && '9' >= charNum)	
		{
			szNum[byNumIndex++] = *pStr;
		}
		else
		{
			if('\0' != szNum[0])
			{
				szNum[byNumIndex++] = '\0';
				unsigned int nNum = 0;
				nNum = atoi((const char*)szNum);
				switch(byIndex)
				{
				case 0:
					{
						if(nNum >= FIRST_CHAR_MAX_INDEX)
						{
							//CCLOG("nNum is %d", nNum);
							nNum = FIRST_CHAR_MAX_INDEX - 1;
						}
						pMaskWordBeginKey[nNum] = 1;
						curmapIndex = nNum;
					}
					break;
				case 1:
					{
						if(curmapIndex >= FIRST_CHAR_MAX_INDEX)
						{
							//CCLOG("curmapIndex is %d", curmapIndex);
							curmapIndex = FIRST_CHAR_MAX_INDEX - 1;
						}
						pMaskWordBeginKeyMaxLen[curmapIndex] = nNum;
					}
					break;
				case 2:
					{
						if(curmapIndex >= FIRST_CHAR_MAX_INDEX)
						{
							//CCLOG("curmapIndex is %d", curmapIndex);
							curmapIndex = FIRST_CHAR_MAX_INDEX - 1;
						}
						pMaskWordBeginKeyMinLen[curmapIndex] = nNum;
					}
					break;
				case 3:
					{
						if(curmapIndex >= FIRST_CHAR_MAX_INDEX)
						{
							//CCLOG("curmapIndex is %d", curmapIndex);
							curmapIndex = FIRST_CHAR_MAX_INDEX - 1;
						}
						curmapIndexHashNum = nNum;
						pMaskWordBeginKeyNum[curmapIndex] = nNum;
						pMaskWordHashValue[curmapIndex] = new unsigned int [curmapIndexHashNum];
						if(!pMaskWordHashValue[curmapIndex])
						{
							//CCLOG("Error pMaskWordHashValue[mapIndex] is NULL");
							return;
						}
					}
					break;
				default:
					{
						int nNumIndex = byIndex - 4;
						if(0 > nNumIndex)
						{
							CCLOG("Error nNumIndex");
							return;
						}
						if( 0 == nNum)
						{
							CCLOG("Key Value is Error ");
						}
						pMaskWordHashValue[curmapIndex][nNumIndex] = nNum;
					}
					break;
				}
				szNum[0] = '\0';
				byNumIndex = 0;
				++byIndex;
				if ((byIndex - 4) >= 0 && (byIndex - 4) >= curmapIndexHashNum)
				{
					byIndex = 0;
				}
			}
		}
		++pStr;
	}
	XFUNC_END();
}
unsigned long MaskWord::GetHashKey(signed char *str,int nLen)
{
	XFUNC_START();
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	for (int nIndex = 0; nIndex < nLen; nIndex++)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
	XFUNC_END_RETURN(unsigned long, res, 0);
}
int MaskWord::binary_searchF(unsigned int* a, int len, unsigned int goal)
{
	XFUNC_START();
	int low = 0;
	int high = len - 1;
	while(low <= high)
	{
		int middle = (low + high)/2;
		if(a[middle] == goal)
			return middle;
		//在左半边
		else if(a[middle] > goal)
			high = middle - 1;
		//在右半边
		else
			low = middle + 1;
	}
	//没找到
	return -1;
	XFUNC_END_RETURN(int, res, -1);
}
void MaskWord::ReplaceMaskWord(signed char *strBegin,int nLen)
{
	XFUNC_START();
	//检测长度的合法性
	if(0 != nLen%2)
	{
		CCLOG("ReplaceMaskWord Error nLen");
		return;
	}
	signed char *p = strBegin;
	for (int i = 0; i < nLen; ++i)
	{
		if(0 == i%2)
			p[i] = 42;
		else
			p[i] = 0;
	}
	XFUNC_END();
}
signed char* MaskWord::CheckMaskWord(signed char *str)
{
	XFUNC_START();
	unsigned short* pStrUni = (unsigned short *)str;
	if(!pStrUni || !pStrUni[0])
		return (signed char*)"";
	static const int nLenth = 128;
	signed char szStr[FIRST_CHAR_MAX_INDEX + 1] = {0};
	memset(szStr, 0, sizeof(signed char)*(FIRST_CHAR_MAX_INDEX + 1));
	memcpy(&szStr, str, nLenth*sizeof(wchar_x));
	pStrUni = (unsigned short *)szStr;
	while (*pStrUni)
	{
		if((*pStrUni) <= 90 && (*pStrUni) >= 65)
			(*pStrUni) = (*pStrUni) + 32;
		++pStrUni;
	}
	pStrUni = (unsigned short *)szStr;
	unsigned short* pStrScr = (unsigned short *)str;		// 源字符串
	while (*pStrUni)
	{
		//查看*pStr 是否是 敏感词的第一个字符
		signed char* pStrChar = (signed char*)pStrUni;
		int nIndex = *pStrChar;
		//CCLOG(" CheckMaskWord Currrent First ByteValue =  is %d", nIndex);
		nIndex += 128;
		if(nIndex >= FIRST_CHAR_MAX_INDEX)
		{
			//CCLOG(" CheckMaskWord nIndex is %d", nIndex);
			nIndex = FIRST_CHAR_MAX_INDEX - 1;
		}
		if(1 == pMaskWordBeginKey[nIndex])
		{
			//CCLOG(" CheckMaskWord Currrent First ByteValue Is Exist");
			int nMaxLen = pMaskWordBeginKeyMaxLen[nIndex];
			int nMinLen = pMaskWordBeginKeyMinLen[nIndex];
			//CCLOG(" CheckMaskWord Currrent First ByteValue Is Exist nMaxLen = %d",nMaxLen);
			//CCLOG(" CheckMaskWord Currrent First ByteValue Is Exist nMinLen = %d",nMinLen);
			bool bIsMaskWord = false;
			//寻找以pStr开头的 长度在 nMaxLen nMinLen之间的敏感词
			for (int nLen = nMaxLen; nLen >= nMinLen;--nLen)
			{
				unsigned short *strBegin = pStrUni;
				unsigned short *strBeginScr = pStrScr;
				unsigned short *strEnd = strBegin;
				//寻找以pStr开头的 长度在 为 nLen的字符串 长度为2的 一定存在 不需要查找
				bool bIsFindStr = true;
				for (int nCurlen = 1; nCurlen < nLen;++nCurlen)
				{
					++strEnd;
					if(!(*strEnd))//还没结束就空了 查找下一个长度
					{
						bIsFindStr =false;
						break;
					}
				}
				//找到长度为nLen的字符串
				if(bIsFindStr)
				{
					//CCLOG(" CheckMaskWord Currrent First ByteValue Is Exist CurLen = %d",nLen);
					signed char* pStr = (signed char*)strBegin;
					unsigned int key = GetHashKey(pStr,nLen*2);
					//CCLOG(" CheckMaskWord key is %u", key);
					//cout<<"Get Key = "<<key<<endl;
					//二分查找
					int nPos = binary_searchF(pMaskWordHashValue[nIndex],pMaskWordBeginKeyNum[nIndex],key);
					if(-1 != nPos) //关键词 替换
					{
						//CCLOG(" CheckMaskWord ReplaceMaskWord");
						signed char* pStr = (signed char*)strBeginScr;
						ReplaceMaskWord(pStr,nLen*2);
						pStrUni = strEnd;
						break;
					}
				}
			}
			++pStrUni;
			++pStrScr;
		}
		else
		{
			++pStrUni;
			++pStrScr;
		}
	}
	return str;
	XFUNC_END_RETURN(signed char*, res, NULL);
}

unsigned char MaskWord::OnlyCheck(signed char *str)
{
	XFUNC_START();
	unsigned short* pStrUni = (unsigned short *)str;
	if(!pStrUni || !pStrUni[0])
		return 0x00;
	static const int nLenth = 128;
	signed char szStr[FIRST_CHAR_MAX_INDEX + 1] = {0};
	memset(szStr, 0, sizeof(signed char)*(FIRST_CHAR_MAX_INDEX + 1));
	memcpy(&szStr, str, nLenth*sizeof(wchar_x));
	pStrUni = (unsigned short *)szStr;
	while (*pStrUni)
	{
		if((*pStrUni) <= 90 && (*pStrUni) >= 65)
			(*pStrUni) = (*pStrUni) + 32;
		++pStrUni;
	}
	pStrUni = (unsigned short *)szStr;
	while (*pStrUni)
	{
		//查看*pStr 是否是 敏感词的第一个字符
		signed char* pStrChar = (signed char*)pStrUni;
		int nIndex = *pStrChar;
		nIndex += 128;
		if(nIndex >= FIRST_CHAR_MAX_INDEX)
		{
			//CCLOG(" CheckMaskWord nIndex is %d", nIndex);
			nIndex = FIRST_CHAR_MAX_INDEX - 1;
		}
		if(1 == pMaskWordBeginKey[nIndex])
		{
			int nMaxLen = pMaskWordBeginKeyMaxLen[nIndex];
			int nMinLen = pMaskWordBeginKeyMinLen[nIndex];
			bool bIsMaskWord = false;
			//寻找以pStr开头的 长度在 nMaxLen nMinLen之间的敏感词
			for (int nLen = nMaxLen; nLen >= nMinLen;--nLen)
			{
				unsigned short *strBegin = pStrUni;
				unsigned short *strEnd = strBegin;
				//寻找以pStr开头的 长度在 为 nLen的字符串 长度为2的 一定存在 不需要查找
				bool bIsFindStr = true;
				for (int nCurlen = 1; nCurlen < nLen;++nCurlen)
				{
					++strEnd;
					if(!(*strEnd))//还没结束就空了 查找下一个长度
					{
						bIsFindStr =false;
						break;
					}
				}
				//找到长度为nLen的字符串
				if(bIsFindStr)
				{
					signed char* pStr = (signed char*)strBegin;
					unsigned int key = GetHashKey(pStr,nLen*2);

					//cout<<"Get Key = "<<key<<endl;
					//二分查找
					int nPos = binary_searchF(pMaskWordHashValue[nIndex],pMaskWordBeginKeyNum[nIndex],key);
					if(-1 != nPos) //关键词 替换
					{
						return 0xFF;
					}
				}
			}
			++pStrUni;
		}
		else
		{
			++pStrUni;
		}
	}
	return 0x00;
	XFUNC_END_RETURN(unsigned char, res, 0xFF);
}