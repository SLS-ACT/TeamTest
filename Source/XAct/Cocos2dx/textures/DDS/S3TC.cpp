#include "../../../Common.h"
#include "S3TC.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#	define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#	define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT	0x83F3
#endif

#ifndef ATC_RGB_AMD
#	define ATC_RGB_AMD	0x8C92
#endif

#ifndef ATC_RGBA_EXPLICIT_ALPHA_AMD
#	define	ATC_RGBA_EXPLICIT_ALPHA_AMD	0x8C93
#endif

#ifndef ATC_RGBA_INTERPOLATED_ALPHA_AMD
#	define	ATC_RGBA_INTERPOLATED_ALPHA_AMD	0x87EE
#endif

#ifndef MAX
#	define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

bool XS3TC::s3tcLoadTexture( const void* bytes, GLsizei size, XDDS_Info& stDInfo )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if ((size < 128) || (*reinterpret_cast<const unsigned long*>(bytes) != cDDSFileSignature))
	{
		return false;
	}
	const DDSURFACEDESC2& desc = *reinterpret_cast<const DDSURFACEDESC2*>((*reinterpret_cast<const unsigned char*>(bytes) + sizeof(unsigned long)));
	if (desc.dwSize != sizeof(DDSURFACEDESC2))
	{
		return false;
	}
	unsigned char* pData = (unsigned char*)bytes + sizeof(unsigned long) + sizeof(DDSURFACEDESC2);
	switch (desc.ddpfPixelFormat.dwFourCC)
	{
	case PIXEL_FMT_DXT1:
		stDInfo.glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case PIXEL_FMT_DXT3:
		stDInfo.glFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case PIXEL_FMT_DXT5:
		stDInfo.glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	case PIXEL_FMT_ATC:
		stDInfo.glFormat = ATC_RGB_AMD;
		break;
	case PIXEL_FMT_ATCA:
		stDInfo.glFormat = ATC_RGBA_EXPLICIT_ALPHA_AMD;
		break;
	case PIXEL_FMT_ATCI:
		stDInfo.glFormat = ATC_RGBA_INTERPOLATED_ALPHA_AMD;
		break;
	default:
		CCLOG("s3tcLoadTexture file failed, doesn't appear to be compressed using DXT1, DXT3, DXT5\n");
		return false;
	}
	if (desc.dwMipMapCount)
	{
		glGenTextures(1, &stDInfo.dwTexture);
		glBindTexture(GL_TEXTURE_2D, stDInfo.dwTexture);
		// default:Anti alias
		if (1 == desc.dwMipMapCount)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	//CHECK_GL_ERROR_DEBUG();
	int nBlockSize;
	if (GL_COMPRESSED_RGBA_S3TC_DXT1_EXT == stDInfo.glFormat)
	{
		nBlockSize = 8;
	}
	else
	{
		nBlockSize = 16;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int nSize;
	int nOffset = 0;
	unsigned int dwWidth = desc.dwWidth;
	unsigned int dwHeight = desc.dwHeight;
	for (unsigned long dwPos = 0; dwPos < desc.dwMipMapCount; ++dwPos)
	{
		if (!dwWidth)
		{
			dwWidth = 1;
		}
		if (!dwHeight)
		{
			dwHeight = 1;
		}
		nSize = ((dwWidth + 3) >> 2) * ((dwHeight + 3) >> 2) * nBlockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, dwPos, stDInfo.glFormat, dwWidth, dwHeight, 0, nSize, pData + nOffset);
		nOffset += nSize;
		GLenum __err = glGetError();
		if (__err != GL_NO_ERROR)
		{
			CCLOG("cocos2d: textureS3TC: error uploading compressed texture level: %u . glError: 0x%04X", dwPos, __err);
			return false;
		}
		dwWidth = MAX(dwWidth >> 1, 1);
		dwHeight = MAX(dwHeight >> 1, 1);
	}
	memcpy(&stDInfo.stDDsc, &desc, sizeof(DDSURFACEDESC2));
	return true;
#endif
	return false;
}
