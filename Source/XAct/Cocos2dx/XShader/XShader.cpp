#include "../../Common.h"
#include "XShader.h"
#include "XGLProgram.inl"
//@{ shader Instance
#include "XShader_ETC1.inl"
#include "XShader_Gray.inl"
//@{ End shader Instance

XCPP_DEFINED();

NS_CC_BEGIN

XShader* XShader::m_apShader[MAX_SHADER];
unsigned char XShader::m_byNum = 0;

void XShader::ReloadAll()
{
	for(unsigned short wPos = 0; wPos < m_byNum; ++wPos)
	{
		if(XShader::m_apShader[wPos])
			XShader::m_apShader[wPos]->Reload();
	}
}

CCGLProgram* XShader::GetProgram()
{
	if (m_pGLP)
	{
		return static_cast<CCGLProgram*>(m_pGLP);
	}
	return NULL;
}

XShaderSoul::XShaderSoul()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderSoul);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderSoul::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"precision mediump float; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"varying vec2 v_texCoord; \n"
		"varying vec4 v_fragmentColor; \n"
		"uniform float f_alpha; \n "
		"uniform float f_hurt; \n "
		"void main(void) \n"
		"{ \n"
		" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		" float colorB = texColor.r > texColor.g ? texColor.r : texColor.g; \n "
		" colorB = colorB > texColor.b ? colorB : texColor.b; \n "
		" if(colorB < 0.5) \n "
		" { \n "
		"	texColor.r = texColor.r * (1.0 + texColor.r); \n "
		"	texColor.g = texColor.g * (1.0 + texColor.g); \n "
		"	colorB = colorB * (1.0 + colorB); \n "
		" } \n "
		" else \n "
		" { \n "
		"	texColor.r = texColor.r * (2.0 - texColor.r); \n "
		"	texColor.g = texColor.g * (2.0 - texColor.g); \n "
		"	colorB = colorB * (2.0 - colorB); \n "
		" } \n "
		" if(f_hurt < 0.3) \n "			// 灵魂
		"	gl_FragColor = vec4(texColor.r*0.5,texColor.g*0.75,colorB,texColor.a*f_alpha); \n "
		" else if(f_hurt < 0.6) \n "	// 霸体
		"	gl_FragColor = vec4(texColor.r,texColor.g,0.0,texColor.a*f_alpha); \n "
		" else \n "						// 受创
		"	gl_FragColor = vec4(texColor.r,texColor.g*0.25,colorB*0.25,texColor.a*f_alpha); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformAlpha = glGetUniformLocation(m_pGLP->getProgram(), "f_alpha");
	m_fParam1 = 1.f;
	m_nUniformHurt = glGetUniformLocation(m_pGLP->getProgram(), "f_hurt");
	m_fParam2 = 0.f;
}

void XShaderSoul::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1f(m_nUniformAlpha, m_fParam1);
		m_pGLP->setUniformLocationWith1f(m_nUniformHurt, m_fParam2);
	}
}

XShaderBloom::XShaderBloom()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderBloom);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderBloom::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"precision mediump float; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"varying vec2 v_texCoord; \n"
		"varying vec4 v_fragmentColor; \n"
		"uniform float f_texWidth; \n"
		"uniform float f_texHeight; \n"
		"uniform float f_param; \n"
		"void main(void) \n"
		"{ \n"
		/*
		//" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		//" gl_FragColor = texColor; \n "
		"	int samples = 5; \n "			// pixels per axis; higher = bigger glow, worse performance
		"	float fSim = 25.0; \n "			// (float)(samples * samples)
		"	float quality = 1.5; \n "		// lower = smaller glow, better quality
		"	vec2 onePixel = vec2(1.0 / f_texWidth, 1.0 / f_texHeight); \n "
		"	vec4 source = texture2D(u_texture, v_texCoord); \n "
		"	vec4 sum = vec4(0, 0, 0, 0); \n "
		"	int diff = (samples - 1) / 2; \n "
		"	vec2 sizeFactor = onePixel * quality; \n "
		"	vec2 offset; \n "
		"	for (int x = -diff; x <= diff; x++) \n "
		"	{ \n "
		"		for (int y = -diff; y <= diff; y++) \n "
		"		{ \n "
		"			offset = vec2(x, y) * sizeFactor + v_texCoord; \n "
		"			sum += texture2D(u_texture, offset); \n "
		"		} \n "
		"	} \n "
		"	sum = sum / fSim;\n"
		"	vec4 sumAdd = sum * source;"
		"	sum = (1.0 - sum) * (1.0 - source);"
		"	sum = 1.0 - sum;"
		"	gl_FragColor = (sumAdd + sum) * 0.5; \n "
		*/
		"	#define SAMPLER_NUMBER		7	\n "
		"	#define SAMPLER_QUALITY		1.0	\n "
		"	vec2 onePixel = vec2(SAMPLER_QUALITY / f_texWidth, SAMPLER_QUALITY / f_texHeight); \n "
		"	vec4 colAdd = vec4(0, 0, 0, 0); \n "
		"	vec4 colSour; \n "
		"	vec4 colOne; \n "
		"	vec2 offset; \n "
		"	int nDiff = (SAMPLER_NUMBER - 1) / 2; \n "
		"	float fWeight = float(nDiff + 1); \n "
		"	fWeight = fWeight * fWeight * 2.0 - fWeight; \n "
		"	int nPosABS;"
		"	for (int nPos = -nDiff; nPos <= nDiff; ++nPos) \n "
		"	{ \n "
		"		nPosABS = nPos; \n "
		"		if(nPosABS < 0) \n "
		"			nPosABS = -nPosABS; \n "
		"		offset = vec2(nPos, nPos) * onePixel + v_texCoord; \n "
		"		colOne = texture2D(u_texture, offset); \n "
		"		colAdd = colAdd + colOne * float(nDiff + 1 - nPosABS) / fWeight;"
		"		if(nPos != 0) \n "
		"		{ \n "
		"			offset = vec2(nPos, -nPos) * onePixel + v_texCoord; \n "
		"			colOne = texture2D(u_texture, offset); \n "
		"			colAdd = colAdd + colOne * float(nDiff + 1 - nPosABS) / fWeight;"
		"		} \n "
		"		else \n "
		"			colSour = colOne; \n "
		"	} \n "
		"	float fBrightO = colSour.r * 0.299 + colSour.g * 0.587 + colSour.b * 0.114; \n "
		"	colOne = colAdd * colSour + 1.0 - (1.0 - colAdd) * (1.0 - colSour); \n "
		"	colOne = colOne * f_param; \n "
		"	float fBrightN = colOne.r * 0.299 + colOne.g * 0.587 + colOne.b * 0.114; \n "
		//// 全屏泛光
		//"	if(fBrightN > fBrightO) \n "
		//"		gl_FragColor = colOne; \n "
		//"	else \n "
		//"		gl_FragColor = colSour; \n "
		////// 漫画风
		////"	if(fBrightN > fBrightO) \n "
		////"		gl_FragColor = vec4(colSour.r * 0.4, colSour.g * 0.4, colSour.b * 0.4, 1.0); \n "
		////"	else \n "
		////"		gl_FragColor = 1.0 - (1.0 - colAdd) * 0.85; \n "
		//// 新全屏泛光
		//"	gl_FragColor = colSour + colOne * (fBrightN - fBrightO); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformTW = glGetUniformLocation(m_pGLP->getProgram(), "f_texWidth");
	m_nUniformTH = glGetUniformLocation(m_pGLP->getProgram(), "f_texHeight");
	m_nUniformWt = glGetUniformLocation(m_pGLP->getProgram(), "f_param");
	m_fParam1 = 1.f;
	m_fParam2 = 1.f;
	m_fWeight = 0.5f;
}

void XShaderBloom::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1f(m_nUniformTW, m_fParam1);
		m_pGLP->setUniformLocationWith1f(m_nUniformTH, m_fParam2);
		m_pGLP->setUniformLocationWith1f(m_nUniformWt, m_fWeight);
	}
}

void XShaderBloom::UseShader()
{
	if(m_pGLP)
	{
		m_pGLP->use();
		m_pGLP->setUniformsForBuiltins();
	}
}

XShaderHeat::XShaderHeat()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderHeat);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderHeat::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"precision mediump float; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"varying vec2 v_texCoord; \n"
		"uniform float f_texOffsetX; \n"
		"uniform float f_texOffsetY; \n"
		"void main(void) \n"
		"{ \n"
		// fetch bump texture, unpack from [0..1] to [-1..1]
		"	vec4 texColor = 2.0 * texture2D(u_texture, v_texCoord) - 1.0; \n "
		// displace texture coordinates : color[0..1]
		"	vec2 newUV = (sin(v_texCoord * v_texCoord * 100.0)) * 0.005 * vec2(f_texOffsetX, f_texOffsetY) + v_texCoord; \n "
		// fetch refraction map
		"	gl_FragColor = texture2D(u_texture, newUV); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nTexOffsetX = glGetUniformLocation(m_pGLP->getProgram(), "f_texOffsetX");
	m_nTexOffsetY = glGetUniformLocation(m_pGLP->getProgram(), "f_texOffsetY");
	m_fParam1 = 0.f;
	m_fParam2 = 0.f;
}

void XShaderHeat::RunBeforeDraw()
{
	if (m_pGLP)
	{
		static float fOffsetAddX = 0.03f;
		m_fParam1 += fOffsetAddX;
		if(m_fParam1 < -0.95f || m_fParam1 > 0.95f)
			fOffsetAddX = -fOffsetAddX;
		else if(0 == rand() % 20)
			fOffsetAddX = -fOffsetAddX;
		m_pGLP->setUniformLocationWith1f(m_nTexOffsetX, m_fParam1);
		static float fOffsetAddY = 0.04f;
		m_fParam2 += fOffsetAddY;
		if(m_fParam2 < -0.95f || m_fParam2 > 0.95f)
			fOffsetAddY = -fOffsetAddY;
		else if(0 == rand() % 10)
			fOffsetAddY = -fOffsetAddY;
		m_pGLP->setUniformLocationWith1f(m_nTexOffsetY, m_fParam2);
	}
}

void XShaderHeat::UseShader()
{
	if(m_pGLP)
	{
		m_pGLP->use();
		m_pGLP->setUniformsForBuiltins();
	}
}

XShaderStroke::XShaderStroke()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderStroke);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderStroke::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"varying vec4 v_fragmentColor; \n"
		"varying mediump vec2 v_texCoord; \n"
		"#else \n"
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texCoord; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"uniform float f_alpha; \n "
		"uniform float f_texWidth; \n "
		"uniform float f_texHeight; \n "
		"uniform float f_hurt; \n "
		"void main(void) \n"
		"{ \n"
		//" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		//// Test Codes : 描边
		//" if(texColor.a < 0.1) \n "
		//" { \n "
		////"	vec2 onePixelW = vec2(1.0 / f_texWidth, 1.0 / f_texHeight); \n "
		//"	vec2 onePixelW = vec2(1.0 / f_texWidth, 0.0); \n "
		//"	vec2 v_texCoordL = v_texCoord - onePixelW; \n "
		//"	vec4 texColorL = texture2D(u_texture, v_texCoordL); \n "
		//"	vec2 v_texCoordR = v_texCoord + onePixelW; \n "
		//"	vec4 texColorR = texture2D(u_texture, v_texCoordR); \n "
		//"	if(texColorL.a > 0.01 || texColorR.a > 0.01) \n "
		//"	{ \n "
		//"		gl_FragColor = vec4(1.0,1.0,1.0,1.0); \n "
		//"	} \n "
		//" } \n "
		" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		" if(texColor.a >= 0.8) \n "
		" { \n "
		"	vec2 onePixelW = vec2(3.0 / f_texWidth, 0.0); \n "
		"	vec2 v_texCoordL = v_texCoord - onePixelW; \n "
		"	vec4 texColorL = texture2D(u_texture, v_texCoordL); \n "
		"	vec2 v_texCoordR = v_texCoord + onePixelW; \n "
		"	vec4 texColorR = texture2D(u_texture, v_texCoordR); \n "
		//"	if(texColorL.a > f_alpha || texColorR.a > f_alpha) \n "
		//"	{ \n "
		//"		gl_FragColor = vec4(1.0,1.0,0.4,0.7); \n "
		//"	} \n "
		"	if(texColorL.a < 0.2 || texColorR.a < 0.2) \n "
		"	{ \n "
		"		gl_FragColor = vec4(1.0, 1.0, 0.4, texColor.a); \n "
		"	} \n "
		"	else \n "
		"	{ \n "
		"		if(f_hurt > 0.5) \n "
		"			gl_FragColor = vec4(texColor.r,texColor.g,0.0,texColor.a); \n "
		"		else \n "
		" 			gl_FragColor = texColor; \n \n "
		//" 			gl_FragColor = vec4(texColor.r,texColor.g,texColor.b*0.4,texColor.a); \n \n "
		"	} \n "
		" } \n "
		" else \n "
		" { \n "
		"	if(f_hurt > 0.5) \n "
		"		gl_FragColor = vec4(texColor.r,texColor.g,0.0,texColor.a); \n "
		"	else \n "
		" 		gl_FragColor = texColor; \n \n "
		//" 		gl_FragColor = vec4(texColor.r,texColor.g,texColor.b*0.4,texColor.a); \n \n "
		" } \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformTW = glGetUniformLocation(m_pGLP->getProgram(), "f_texWidth");
	m_nUniformTH = glGetUniformLocation(m_pGLP->getProgram(), "f_texHeight");
	m_nUniformA = glGetUniformLocation(m_pGLP->getProgram(), "f_alpha");
	m_nUniformH = glGetUniformLocation(m_pGLP->getProgram(), "f_hurt");
	m_fParam1 = 0.01f;
	m_fParam2 = 0.f;
}

void XShaderStroke::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1f(m_nUniformTW, m_fTexW);
		m_pGLP->setUniformLocationWith1f(m_nUniformTH, m_fTexH);
		m_pGLP->setUniformLocationWith1f(m_nUniformA, m_fParam1);
		m_pGLP->setUniformLocationWith1f(m_nUniformH, m_fParam2);
	}
}

XShaderWhite::XShaderWhite()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderWhite);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderWhite::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"varying vec4 v_fragmentColor; \n"
		"varying mediump vec2 v_texCoord; \n"
		"#else \n"
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texCoord; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"uniform float f_alpha; \n "
		"void main(void) \n "
		"{ \n "
		" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		" if(texColor.a <= 0.003) \n "
		" { \n"
		"	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0); \n "
		"	return; \n "
		" } \n "
		" texColor.a = texColor.a * f_alpha; \n "
		" gl_FragColor = vec4(texColor.a, texColor.a, texColor.a, texColor.a); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformA = glGetUniformLocation(m_pGLP->getProgram(), "f_alpha");
	m_fParam1 = 1.f;
}

void XShaderWhite::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1f(m_nUniformA, m_fParam1);
	}
}

XShaderColor::XShaderColor()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderWhite);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderColor::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"varying vec4 v_fragmentColor; \n"
		"varying mediump vec2 v_texCoord; \n"
		"#else \n"
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texCoord; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"uniform float f_alpha; \n "
		"uniform float f_cr; \n "
		"uniform float f_cg; \n "
		"uniform float f_cb; \n "
		"void main(void) \n "
		"{ \n "
		" if(f_alpha <= 0.003) \n "
		" { \n"
		"	vec4 texColor = texture2D(u_texture, v_texCoord); \n "
		"	gl_FragColor = vec4(f_cr, f_cg, f_cb, texColor.a); \n "
		"	return; \n "
		" } \n "
		" gl_FragColor = vec4(f_cr, f_cg, f_cb, f_alpha); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformA = glGetUniformLocation(m_pGLP->getProgram(), "f_alpha");
	m_nUniformR = glGetUniformLocation(m_pGLP->getProgram(), "f_cr");
	m_nUniformG = glGetUniformLocation(m_pGLP->getProgram(), "f_cg");
	m_nUniformB = glGetUniformLocation(m_pGLP->getProgram(), "f_cb");
	m_fParam1 = 1.f;
}

void XShaderColor::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1f(m_nUniformA, m_fParam1);
		m_pGLP->setUniformLocationWith1f(m_nUniformR, m_fCR);
		m_pGLP->setUniformLocationWith1f(m_nUniformG, m_fCG);
		m_pGLP->setUniformLocationWith1f(m_nUniformB, m_fCB);
	}
}

XShaderPal::XShaderPal()
{
	m_pGLP = XMP_CREATE(XGLProgram);
	if(!m_pGLP)
		return;
	Reload();
	XMemLog::m_dwNew += sizeof(XShaderPal);
	XShader::m_apShader[XShader::m_byNum++] = this;
}

void XShaderPal::Reload()
{
	const GLchar szShader[] =
		"#ifdef GL_ES \n"
		"varying vec4 v_fragmentColor; \n"
		"varying mediump vec2 v_texCoord; \n"
		"#else \n"
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texCoord; \n"
		"#endif \n"
		"uniform sampler2D u_texture; \n"
		"uniform sampler2D u_texAlpha; \n"
		"uniform sampler2D u_texPal; \n"
		"void main(void) \n "
		"{ \n "
		"	lowp vec4 texAlpha = texture2D(u_texAlpha, v_texCoord); \n "
		//// 模糊平滑效果
		//"	lowp vec4 texColor = vec4(0, 0, 0, 0); \n "
		//"	if(texAlpha.a > 0.003) \n "
		//"	{ \n "
		//"		int nDiff = 1; \n "
		//"		highp vec4 pixPal; \n "
		//"		highp vec2 uvPal; \n "
		//"		vec2 offset; \n "
		//"		for (int nPos = -nDiff; nPos <= nDiff; ++nPos) \n "
		//"		{ \n "
		//"			offset = vec2(nPos, nPos) + v_texCoord; \n "
		//"			pixPal = texture2D(u_texture, offset); \n "
		//"			uvPal = vec2(pixPal.a, 0.0); \n "
		//"			texColor = texColor + texture2D(u_texPal, uvPal); \n "
		//"			if(nPos != 0) \n "
		//"			{ \n "
		//"				offset = vec2(nPos, -nPos) + v_texCoord; \n "
		//"				pixPal = texture2D(u_texture, offset); \n "
		//"				uvPal = vec2(pixPal.a, 0.0); \n "
		//"				texColor = texColor + texture2D(u_texPal, uvPal); \n "
		//"			} \n "
		//"		} \n "
		////"		texColor = texColor * 0.2; \n "
		//"	} \n "
		// 普通调色板效果
		"	highp vec4 pixPal = texture2D(u_texture, v_texCoord); \n "
		"	highp vec2 uvPal = vec2(pixPal.a, 0.0); \n "
		"	lowp vec4 texColor = texture2D(u_texPal, uvPal); \n "
		"	gl_FragColor = vec4(texColor.r, texColor.g, texColor.b, texAlpha.a); \n "
		"}";
	m_pGLP->initWithVertexShaderByteArray(ccPositionTextureColor_vert, szShader);
	m_pGLP->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_pGLP->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	m_pGLP->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_pGLP->link();
	m_pGLP->updateUniforms();
	m_pGLP->m_pShader = this;
	m_nUniformAlpha = glGetUniformLocation(m_pGLP->getProgram(), "u_texAlpha");
	m_nUniformPal = glGetUniformLocation(m_pGLP->getProgram(), "u_texPal");
}

void XShaderPal::RunBeforeDraw()
{
	if (m_pGLP)
	{
		m_pGLP->setUniformLocationWith1i(m_nUniformAlpha, XSprite::eTexAlpha);
		m_pGLP->setUniformLocationWith1i(m_nUniformPal, XSprite::eTexPal);
	}
}

void XShaderPal::UseShader()
{
	if(m_pGLP)
	{
		m_pGLP->use();
		m_pGLP->setUniformsForBuiltins();
	}
}

NS_CC_END