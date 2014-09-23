#ifndef __XFILEUTILS_H__
#define __XFILEUTILS_H__
/*
	文件管理类
	by 刘潇 @ 2014-05-12
*/

#include "cocos2d.h"
#include "../XHelper/XHelper.h"

NS_CC_BEGIN

class XFileUtils :
	public CCFileUtils,
	public XSingleton<XFileUtils>
{
public:
    XFileUtils(void)
	{
	}
    virtual ~XFileUtils(void)
	{
	}
public:
	virtual std::string getWritablePath() { return ""; }
	virtual bool isFileExist(const std::string& strFilePath) { return false; }
	virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
	virtual unsigned char* getFileDataThread(const char* pszFileName, const char* pszMode, unsigned long * pSize, unsigned char byThId);
	virtual void delFileData(unsigned char* &pData);
	virtual bool getFileInfoInXPF(const char* pszFileName, char* szXPFPath, unsigned long &dwOffset, unsigned long &dwSize);
};

NS_CC_END

#endif // __XDIRECTOR_H__
