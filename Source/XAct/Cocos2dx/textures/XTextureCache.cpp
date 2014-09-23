#include "../../Common.h"
#include "XTextureCache.h"

XCPP_DEFINED();

NS_CC_BEGIN

CCTexture2D* XTextureCache::addImage( const char* fileimage )
{
	XFUNC_START();
	CCAssert(fileimage != NULL, "TextureCache: fileImage must not be nil");

	CCTexture2D* texture = NULL;
	CCImage* pImage = NULL;
	
	std::string pathKey = fileimage;

	pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
	if (pathKey.size() == 0)
	{
		return NULL;
	}

	texture = (CCTexture2D*)m_pTextures->objectForKey(pathKey.c_str()); 

	std::string fullpath = pathKey;

	if (!texture)
	{
		std::string lowerCase(pathKey);
		for(unsigned int i = 0; i < lowerCase.length(); ++i)
		{
			lowerCase[i] = tolower(lowerCase[i]);
		}
		
		do 
		{
			if (std::string::npos != lowerCase.find(".pvr"))
			{
				texture = this->addPVRImage(fullpath.c_str());
			}
			else if (std::string::npos != lowerCase.find(".ktx"))
			{
				texture = this->addETC1Image(fullpath.c_str());
			}
			else if (std::string::npos != lowerCase.find(".dds"))
			{
				texture = this->addS3TCImage(fullpath.c_str());
			}
			else
			{
				CCImage::EImageFormat eImageFormat = CCImage::kFmtUnKnown;
				if (std::string::npos != lowerCase.find(".png"))
				{
					eImageFormat = CCImage::kFmtPng;
				}
				else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
				{
					eImageFormat = CCImage::kFmtJpg;
				}
				else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
				{
					eImageFormat = CCImage::kFmtTiff;
				}
				else if (std::string::npos != lowerCase.find(".webp"))
				{
					eImageFormat = CCImage::kFmtWebp;
				}

				pImage = new CCImage();
				CC_BREAK_IF(NULL == pImage);

				unsigned long nSize = 0;
				unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fileimage, "rb", &nSize);
				if(!pBuffer)
					pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", &nSize);
				bool bRet = pImage->initWithImageData((void*)pBuffer, nSize, eImageFormat);
				CCFileUtils::sharedFileUtils()->delFileData(pBuffer);
				CC_BREAK_IF(!bRet);

				texture = new CCTexture2D();

				if( texture &&
					texture->initWithImage(pImage) )
				{
#if CC_ENABLE_CACHE_TEXTURE_DATA
					// cache the texture file name
					VolatileTexture::addImageTexture(texture, fullpath.c_str(), eImageFormat);
#endif
					m_pTextures->setObject(texture, pathKey.c_str());
					texture->release();
				}
				else
				{
					CCLOG("cocos2d: Couldn't create texture for file:%s in CCTextureCache", fileimage);
				}
			}
			
		} while (false);		
	}
	
	
	XFUNC_END_RETURN(CCTexture2D*, ptr, NULL);
}

CCTexture2D* XTextureCache::addETC1Image( const char* fileimage )
{
	XFUNC_START();
	CCAssert(fileimage != NULL, "TextureCache: fileImage must not be nil");

	XTexture2D* texture = NULL;
	std::string key(fileimage);

	if ((texture = (XTexture2D*)m_pTextures->objectForKey(key.c_str())))
	{
		return texture;
	}

	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(key.c_str());
	texture = new XTexture2D();
	if (texture != NULL && texture->initWithETC1File(fullpath.c_str()))
	{
#if CC_ENABLE_CACHE_TEXTURE_DATA
		VolatileTexture::addImageTexture(texture, fullpath.c_str(), CCImage::kFmtRawData);
#endif
		m_pTextures->setObject(texture, key.c_str());
		texture->autorelease();
	}
	else
	{
		CCLOG("cocos2d: Couldn't add ETC1Image:%s in CCTextureCache", key.c_str());
		CC_SAFE_DELETE(texture);
	}

	return texture;
	XFUNC_END_RETURN(CCTexture2D*, ptr, NULL);
}

CCTexture2D* XTextureCache::addS3TCImage( const char* fileimage )
{
	XFUNC_START();
	CCAssert(fileimage != NULL, "TextureCache: fileImage must not be nil");

	XTexture2D* texture = NULL;
	std::string key(fileimage);

	if ((texture = (XTexture2D*)m_pTextures->objectForKey(key.c_str())))
	{
		return texture;
	}

	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(key.c_str());
	texture = new XTexture2D();
	if (texture != NULL && texture->initWithS3TCFile(fullpath.c_str()))
	{
#if CC_ENABLE_CACHE_TEXTURE_DATA
		VolatileTexture::addImageTexture(texture, fullpath.c_str(), CCImage::kFmtRawData);
#endif
		m_pTextures->setObject(texture, key.c_str());
		texture->autorelease();
	}
	else
	{
		CCLOG("cocos2d: Couldn't add S3TCImage:%s in CCTextureCache", key.c_str());
		CC_SAFE_DELETE(texture);
	}

	return texture;
	XFUNC_END_RETURN(CCTexture2D*, ptr, NULL);
}

bool XTextureCache::InitTexture( const char* fileName, CCImage* pImg, CCTexture2D* pTex )
{
	XFUNC_START();
	if (!fileName || !pImg || !pTex)
	{
		return false;
	}
	//printf("[ref:%d]%s\n", pTex->retainCount(), fileName);
	bool bRes = pTex->initWithImage(pImg);
	if (!bRes)
	{
		return false;
	}
	//printf("[ref:%d]%s\n", pTex->retainCount(), fileName);
	m_pTextures->setObject(pTex, fileName);
	//printf("[ref:%d]%s\n", pTex->retainCount(), fileName);
	pTex->autorelease();
	//printf("[ref:%d]%s\n", pTex->retainCount(), fileName);
	return true;	
	XFUNC_END_RETURN(bool, res, NULL);
}

bool XTextureCache::InitTexture( const char* fileName, XTexture2D* pTex, XTexture2D* &pTexKtxAlpha )
{
	XFUNC_START();
	if (!fileName || !pTex)
	{
		return false;
	}
	
	std::string pathKey = fileName;
	std::string lowerCase(pathKey);
	for(unsigned int i = 0; i < lowerCase.length(); ++i)
	{
		lowerCase[i] = tolower(lowerCase[i]);
	}
	bool bRes = false;
	if (std::string::npos != lowerCase.find(".pvr"))
	{
		bRes = pTex->initWithPVRFile(pathKey.c_str());
	}
	else if (std::string::npos != lowerCase.find(".ktx"))
	{
		bRes = pTex->initWithETC1File(pathKey.c_str());
		if (bRes)
		{
			pTexKtxAlpha = new cocos2d::XTexture2D();
			if (pTexKtxAlpha)
			{
				std::string tempPath(pathKey);
				size_t startPos = tempPath.find_last_of(".");
				tempPath = tempPath.erase(startPos);
				tempPath = tempPath.append("_alpha");
				tempPath = tempPath.append(".ktx");
				bRes = pTexKtxAlpha->initWithETC1File(tempPath.c_str());
				if (bRes)
				{
					m_pTextures->setObject(pTexKtxAlpha, tempPath.c_str());
					pTexKtxAlpha->autorelease();
				}				
			}			
		}		
	}
	else if (std::string::npos != lowerCase.find(".dds"))
	{
		bRes = pTex->initWithS3TCFile(pathKey.c_str());
	}
	if (!bRes)
	{
		CCLOG("cocos2d: cann't init texture for file:%s in CCTextureCache", fileName);
		return false;
	}
	m_pTextures->setObject(pTex, pathKey.c_str());
	pTex->autorelease();
	return true;
	
	XFUNC_END_RETURN(bool, res, NULL);
}

void XTextureCache::dumpCachedTextureInfo()
{
	unsigned int count = 0;
	unsigned int totalBytes = 0;

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(m_pTextures, pElement)
	{
		CCTexture2D* tex = (CCTexture2D*)pElement->getObject();
		unsigned int bpp = tex->bitsPerPixelForFormat();
		// Each texture takes up width * height * bytesPerPixel bytes.
		unsigned int bytes = tex->getPixelsWide() * tex->getPixelsHigh() * bpp / 8;
		totalBytes += bytes;
		count++;
		CCLOG("cocos2d: \"%s\" rc=%lu id=%lu %lu x %lu @ %ld bpp => %lu KB",
			pElement->getStrKey(),
			(long)tex->retainCount(),
			(long)tex->getName(),
			(long)tex->getPixelsWide(),
			(long)tex->getPixelsHigh(),
			(long)bpp,
			(long)bytes / 1024);
	}

	CCLOG("cocos2d: CCTextureCache dumpDebugInfo: %ld textures, for %lu KB (%.2f MB)", (long)count, (long)totalBytes / 1024, totalBytes / (1024.0f*1024.0f));
}
NS_CC_END
