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
		CCLOG("[MaskWord::InitMaskWord]���棺[FILE]���ļ���%s��ʧ��", szPath);
		return;
	}
	if(!dwFileSize || INVALID_ID == dwFileSize)
	{
		printf("[MaskWord::InitMaskWord]���棺[FILE]��ȡ�ļ���%s����Сʧ��", szPath);
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
	byIndex �� 0��3 ����Ϊ mapIndex�� ��󳤶ȡ���̳��ȡ�����nNum 
	4 �� nNum + 4  ����Ϊ��ϣֵ
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
		//������
		else if(a[middle] > goal)
			high = middle - 1;
		//���Ұ��
		else
			low = middle + 1;
	}
	//û�ҵ�
	return -1;
	XFUNC_END_RETURN(int, res, -1);
}
void MaskWord::ReplaceMaskWord(signed char *strBegin,int nLen)
{
	XFUNC_START();
	//��ⳤ�ȵĺϷ���
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
	unsigned short* pStrScr = (unsigned short *)str;		// Դ�ַ���
	while (*pStrUni)
	{
		//�鿴*pStr �Ƿ��� ���дʵĵ�һ���ַ�
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
			//Ѱ����pStr��ͷ�� ������ nMaxLen nMinLen֮������д�
			for (int nLen = nMaxLen; nLen >= nMinLen;--nLen)
			{
				unsigned short *strBegin = pStrUni;
				unsigned short *strBeginScr = pStrScr;
				unsigned short *strEnd = strBegin;
				//Ѱ����pStr��ͷ�� ������ Ϊ nLen���ַ��� ����Ϊ2�� һ������ ����Ҫ����
				bool bIsFindStr = true;
				for (int nCurlen = 1; nCurlen < nLen;++nCurlen)
				{
					++strEnd;
					if(!(*strEnd))//��û�����Ϳ��� ������һ������
					{
						bIsFindStr =false;
						break;
					}
				}
				//�ҵ�����ΪnLen���ַ���
				if(bIsFindStr)
				{
					//CCLOG(" CheckMaskWord Currrent First ByteValue Is Exist CurLen = %d",nLen);
					signed char* pStr = (signed char*)strBegin;
					unsigned int key = GetHashKey(pStr,nLen*2);
					//CCLOG(" CheckMaskWord key is %u", key);
					//cout<<"Get Key = "<<key<<endl;
					//���ֲ���
					int nPos = binary_searchF(pMaskWordHashValue[nIndex],pMaskWordBeginKeyNum[nIndex],key);
					if(-1 != nPos) //�ؼ��� �滻
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
		//�鿴*pStr �Ƿ��� ���дʵĵ�һ���ַ�
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
			//Ѱ����pStr��ͷ�� ������ nMaxLen nMinLen֮������д�
			for (int nLen = nMaxLen; nLen >= nMinLen;--nLen)
			{
				unsigned short *strBegin = pStrUni;
				unsigned short *strEnd = strBegin;
				//Ѱ����pStr��ͷ�� ������ Ϊ nLen���ַ��� ����Ϊ2�� һ������ ����Ҫ����
				bool bIsFindStr = true;
				for (int nCurlen = 1; nCurlen < nLen;++nCurlen)
				{
					++strEnd;
					if(!(*strEnd))//��û�����Ϳ��� ������һ������
					{
						bIsFindStr =false;
						break;
					}
				}
				//�ҵ�����ΪnLen���ַ���
				if(bIsFindStr)
				{
					signed char* pStr = (signed char*)strBegin;
					unsigned int key = GetHashKey(pStr,nLen*2);

					//cout<<"Get Key = "<<key<<endl;
					//���ֲ���
					int nPos = binary_searchF(pMaskWordHashValue[nIndex],pMaskWordBeginKeyNum[nIndex],key);
					if(-1 != nPos) //�ؼ��� �滻
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