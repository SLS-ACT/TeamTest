#ifndef _MASKWORD_H_
#define _MASKWORD_H_

#define FIRST_CHAR_MAX_INDEX  256

#include "../common.h"

class MaskWord
{
public:
	MaskWord();
	~MaskWord();

public:	
	static void InitMaskWord(char *szPath);
	static unsigned long GetHashKey(signed char *str,int nLen);
	static int binary_searchF(unsigned int* a, int len, unsigned int goal);
	static void ReplaceMaskWord(signed char *strBegin,int nLen);
	static signed char* CheckMaskWord(signed char *str);
	static unsigned char OnlyCheck(signed char *str);
private:
	static signed char pMaskWordBeginKey[FIRST_CHAR_MAX_INDEX];
	static short pMaskWordBeginKeyMaxLen[FIRST_CHAR_MAX_INDEX];
	static short pMaskWordBeginKeyMinLen[FIRST_CHAR_MAX_INDEX];
	static int pMaskWordBeginKeyNum[FIRST_CHAR_MAX_INDEX];
	static unsigned int **pMaskWordHashValue;
};

#endif