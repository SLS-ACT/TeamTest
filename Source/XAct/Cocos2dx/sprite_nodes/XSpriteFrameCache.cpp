#include "../../Common.h"
#include "../../XConfig.h"
#include "../XLoader/XLoader.h"
#include <vector>

using namespace std;

XCPP_DEFINED();

NS_CC_BEGIN

void XSpriteFrameCache::Reload()
{
	XFUNC_START();
	// 重加载资源
	for(unsigned short wPos = 0; wPos < m_byUsed; ++wPos)
	{
		XSprFrmInfo& stFInfo = m_astSFInfo[wPos];
		if(stFInfo.pTex && !stFInfo._byReload)
		{
			++XResLoader::_dwReTotal;
			stFInfo._byReload = 0xFF;
			XResLoader::Instance().AddResReload((XEventObj*)&stFInfo, 
				(FUNC_HANDEL)&cocos2d::XSprFrmInfo::ResHandle, GetRealPath(stFInfo.szTexPath), NULL);
		}
	}
	XFUNC_END();
}

void XSpriteFrameCache::addSpriteFramesWithFile(const char *pszPlist)
{
	XFUNC_START();
	CCAssert(pszPlist, "plist filename should not be NULL");

	if (m_pLoadedFileNames->find(pszPlist) == m_pLoadedFileNames->end())
	{
		std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
		CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPlist);
		if(!dict)
			dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

		string texturePath("");

		CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
		if (metadataDict)
		{
			// try to read  texture file name from meta data
			texturePath = metadataDict->valueForKey("textureFileName")->getCString();
		}

		if (! texturePath.empty())
		{
			// build texture path relative to plist file
			texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), pszPlist);
		}
		else
		{
			// build texture path by replacing file extension
			texturePath = pszPlist;

			// remove .xxx
			size_t startPos = texturePath.find_last_of("."); 
			texturePath = texturePath.erase(startPos);

			// append .png
			texturePath = texturePath.append(".png");

			CCLOG("cocos2d: XSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
		}

		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());

		if (pTexture)
		{
			addSpriteFramesWithDictionary(dict, pTexture);
			m_pLoadedFileNames->insert(pszPlist);

			if(m_byUsed < XSPC_PLIST_MAX)
			{
				XSprFrmInfo& stFInfo = m_astSFInfo[m_byUsed++];
				stFInfo._byReload = 0x00;
				stFInfo.pTex = pTexture;
				strncpy(stFInfo.szTexPath, texturePath.c_str(), sizeof(char) * MAX_PATH);
			}
			//CCLOG("[Texture]XSpriteFrameCache [%d]%s used %d", m_byUsed, texturePath.c_str(), pTexture->getName());
		}
		else
		{
			CCLOG("cocos2d: XSpriteFrameCache: Couldn't load texture");
		}

		dict->release();
	}
	XFUNC_END();
}

NS_CC_END
