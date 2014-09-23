#include "../Common.h"
#include "XFileUtils.h"
#include "../Update/XPackage.h"

XCPP_DEFINED();

NS_CC_BEGIN

unsigned char* XFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
	XFUNC_START();
	static XPackage& stInst = XPackage::Instance();
	if(stInst._byValid)
	{
		unsigned char* abyBuf;
		unsigned long dwSize;
		bool bRes = stInst.Read(pszFileName, abyBuf, dwSize);
		if(pSize)
			*pSize = dwSize;
		if(bRes)
			return abyBuf;
	}
	XFUNC_END();
	return NULL;
}

unsigned char* XFileUtils::getFileDataThread(const char* pszFileName, const char* pszMode, unsigned long * pSize, unsigned char byThId)
{
	XFUNC_START();
	return getFileData(pszFileName, pszMode, pSize);
	XFUNC_END();
	return NULL;
}

void XFileUtils::delFileData(unsigned char* &pData)
{
	XFUNC_START();
	static XPackage& stInst = XPackage::Instance();
	if(stInst._byValid)
		stInst.Del(pData);
	XFUNC_END();
}

bool XFileUtils::getFileInfoInXPF(const char* pszFileName, char* szXPFPath, unsigned long &dwOffset, unsigned long &dwSize)
{
	XFUNC_START();
	static XPackage& stInst = XPackage::Instance();
	if(!stInst._byValid)
		return false;
	XPF_FileInfo stFInfo;
	bool bRes = stInst.FileInfo(pszFileName, stFInfo);
	if(!bRes)
		return false;
	if(!stFInfo.pVerInfo)
		return false;
	strncpy(szXPFPath, stFInfo.pVerInfo->szPath, sizeof(char) * MAX_PATH);
	dwOffset = stFInfo.dwSeek;
	dwSize = stFInfo.dwLength;
	return true;
	XFUNC_END();
	return false;
}

NS_CC_END

