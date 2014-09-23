#ifndef __S3TC_H__
#define __S3TC_H__

#define CRNLIB_PIXEL_FMT_FOURCC(a, b, c, d) ((a) | ((b) << 8U) | ((c) << 16U) | ((d) << 24U))

namespace XS3TC
{
	enum pixel_format
	{
		PIXEL_FMT_INVALID               = 0,

		PIXEL_FMT_DXT1                  = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', 'T', '1'),
		PIXEL_FMT_DXT2                  = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', 'T', '2'),
		PIXEL_FMT_DXT3                  = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', 'T', '3'),
		PIXEL_FMT_DXT4                  = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', 'T', '4'),
		PIXEL_FMT_DXT5                  = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', 'T', '5'),
		PIXEL_FMT_3DC                   = CRNLIB_PIXEL_FMT_FOURCC('A', 'T', 'I', '2'), // DXN_YX
		PIXEL_FMT_DXN                   = CRNLIB_PIXEL_FMT_FOURCC('A', '2', 'X', 'Y'), // DXN_XY
		PIXEL_FMT_DXT5A                 = CRNLIB_PIXEL_FMT_FOURCC('A', 'T', 'I', '1'), // ATI1N, http://developer.amd.com/media/gpu_assets/Radeon_X1x00_Programming_Guide.pdf
		PIXEL_FMT_ATC			        = CRNLIB_PIXEL_FMT_FOURCC('A', 'T', 'C', ' '),
		PIXEL_FMT_ATCA                  = CRNLIB_PIXEL_FMT_FOURCC('A', 'T', 'C', 'A'),
		PIXEL_FMT_ATCI                  = CRNLIB_PIXEL_FMT_FOURCC('A', 'T', 'C', 'I'),
		// Non-standard, crnlib-specific pixel formats (some of these are supported by ATI's Compressonator)
		PIXEL_FMT_DXT5_CCxY             = CRNLIB_PIXEL_FMT_FOURCC('C', 'C', 'x', 'Y'),
		PIXEL_FMT_DXT5_xGxR             = CRNLIB_PIXEL_FMT_FOURCC('x', 'G', 'x', 'R'),
		PIXEL_FMT_DXT5_xGBR             = CRNLIB_PIXEL_FMT_FOURCC('x', 'G', 'B', 'R'),
		PIXEL_FMT_DXT5_AGBR             = CRNLIB_PIXEL_FMT_FOURCC('A', 'G', 'B', 'R'),

		PIXEL_FMT_DXT1A                 = CRNLIB_PIXEL_FMT_FOURCC('D', 'X', '1', 'A'),
		PIXEL_FMT_ETC1                  = CRNLIB_PIXEL_FMT_FOURCC('E', 'T', 'C', '1'),

		PIXEL_FMT_R8G8B8                = CRNLIB_PIXEL_FMT_FOURCC('R', 'G', 'B', 'x'),
		PIXEL_FMT_L8                    = CRNLIB_PIXEL_FMT_FOURCC('L', 'x', 'x', 'x'),
		PIXEL_FMT_A8                    = CRNLIB_PIXEL_FMT_FOURCC('x', 'x', 'x', 'A'),
		PIXEL_FMT_A8L8                  = CRNLIB_PIXEL_FMT_FOURCC('L', 'x', 'x', 'A'),
		PIXEL_FMT_A8R8G8B8              = CRNLIB_PIXEL_FMT_FOURCC('R', 'G', 'B', 'A')
	};
	
	const unsigned long cDDSMaxImageDimensions = 8192U;

	// Total size of header is sizeof(uint32)+cDDSSizeofDDSurfaceDesc2;
	const unsigned long cDDSSizeofDDSurfaceDesc2 = 124;

	// "DDS "
	const unsigned long cDDSFileSignature = 0x20534444;

	struct DDCOLORKEY
	{
		unsigned long dwUnused0;
		unsigned long dwUnused1;
	};

	struct DDPIXELFORMAT
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwFourCC;
		unsigned long dwRGBBitCount;     // ATI compressonator and crnlib will place a FOURCC code here for swizzled/cooked DXTn formats
		unsigned long dwRBitMask;
		unsigned long dwGBitMask;
		unsigned long dwBBitMask;
		unsigned long dwRGBAlphaBitMask;
	};

	struct DDSCAPS2
	{
		unsigned long dwCaps;
		unsigned long dwCaps2;
		unsigned long dwCaps3;
		unsigned long dwCaps4;
	};

	struct DDSURFACEDESC2
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwHeight;
		unsigned long dwWidth;
		union
		{
			long lPitch;
			unsigned long dwLinearSize;
		};
		unsigned long dwBackBufferCount;
		unsigned long dwMipMapCount;
		unsigned long dwAlphaBitDepth;
		unsigned long dwUnused0;
		unsigned long lpSurface;
		DDCOLORKEY unused0;
		DDCOLORKEY unused1;
		DDCOLORKEY unused2;
		DDCOLORKEY unused3;
		DDPIXELFORMAT ddpfPixelFormat;
		DDSCAPS2 ddsCaps;
		unsigned long dwUnused1;
	};

	const unsigned long DDSD_CAPS                   = 0x00000001;
	const unsigned long DDSD_HEIGHT                 = 0x00000002;
	const unsigned long DDSD_WIDTH                  = 0x00000004;
	const unsigned long DDSD_PITCH                  = 0x00000008;

	const unsigned long DDSD_BACKBUFFERCOUNT        = 0x00000020;
	const unsigned long DDSD_ZBUFFERBITDEPTH        = 0x00000040;
	const unsigned long DDSD_ALPHABITDEPTH          = 0x00000080;

	const unsigned long DDSD_LPSURFACE              = 0x00000800;

	const unsigned long DDSD_PIXELFORMAT            = 0x00001000;
	const unsigned long DDSD_CKDESTOVERLAY          = 0x00002000;
	const unsigned long DDSD_CKDESTBLT              = 0x00004000;
	const unsigned long DDSD_CKSRCOVERLAY           = 0x00008000;

	const unsigned long DDSD_CKSRCBLT               = 0x00010000;
	const unsigned long DDSD_MIPMAPCOUNT            = 0x00020000;
	const unsigned long DDSD_REFRESHRATE            = 0x00040000;
	const unsigned long DDSD_LINEARSIZE             = 0x00080000;

	const unsigned long DDSD_TEXTURESTAGE           = 0x00100000;
	const unsigned long DDSD_FVF                    = 0x00200000;
	const unsigned long DDSD_SRCVBHANDLE            = 0x00400000;
	const unsigned long DDSD_DEPTH                  = 0x00800000;

	const unsigned long DDSD_ALL                    = 0x00fff9ee;

	const unsigned long DDPF_ALPHAPIXELS            = 0x00000001;
	const unsigned long DDPF_ALPHA                  = 0x00000002;
	const unsigned long DDPF_FOURCC                 = 0x00000004;
	const unsigned long DDPF_PALETTEINDEXED8        = 0x00000020;
	const unsigned long DDPF_RGB                    = 0x00000040;
	const unsigned long DDPF_LUMINANCE              = 0x00020000;

	const unsigned long DDSCAPS_COMPLEX             = 0x00000008;
	const unsigned long DDSCAPS_TEXTURE             = 0x00001000;
	const unsigned long DDSCAPS_MIPMAP              = 0x00400000;

	const unsigned long DDSCAPS2_CUBEMAP            = 0x00000200;
	const unsigned long DDSCAPS2_CUBEMAP_POSITIVEX  = 0x00000400;
	const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEX  = 0x00000800;

	const unsigned long DDSCAPS2_CUBEMAP_POSITIVEY  = 0x00001000;
	const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEY  = 0x00002000;
	const unsigned long DDSCAPS2_CUBEMAP_POSITIVEZ  = 0x00004000;
	const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEZ  = 0x00008000;

	const unsigned long DDSCAPS2_VOLUME             = 0x00200000;

	typedef struct XDDS_Info
	{
		GLuint	dwTexture;
		GLenum	glFormat;
		DDSURFACEDESC2	stDDsc;
	} XDDS_Info;
	bool s3tcLoadTexture(const void* bytes, GLsizei size, XDDS_Info& stDInfo);
} // namespace XS3TC

#endif // __S3TC_H__
