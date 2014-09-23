#ifndef __XSHADER_H__
#define __XSHADER_H__
#include "cocos2d.h"
#include "../../XHelper/XHelper.h"
// shader 类
NS_CC_BEGIN

#define MAX_SHADER	(16)

class XGLProgram;
class XShader
{
public:
	enum
	{
		eNone = 0,
		eETC,
		eGray,
		eGrayChange,
		ewhiteChange,

		eNum
	};
public:
	virtual void	RunBeforeDraw()		= 0;
	virtual void	Release()			= 0;
	virtual void	Reload()			= 0;
protected:
	virtual const GLchar* GetShaderStr() = 0;
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP)	= 0;
public:
	CCGLProgram* GetProgram();
	inline void SetParamf1(GLfloat fParam1) {
		m_fParam1 = fParam1;
	}
	inline void SetParamf2(GLfloat fParam2) {
		m_fParam2 = fParam2;
	}
protected:
	CCNode*			m_pNode;
	XGLProgram*		m_pGLP;
	CCGLProgram*	m_pOldShader;
protected:
	GLfloat			m_fParam1;
	GLfloat			m_fParam2;
public:
	XShader() : m_pNode(NULL), m_pGLP(NULL), m_pOldShader(NULL) {}
	virtual ~XShader() {}
protected:
	static XShader*	m_apShader[MAX_SHADER];
	static unsigned char m_byNum;
public:
	static void ReloadAll();
};

class XGLProgram :
	public CCGLProgram,
	public XMemPool<XGLProgram>
{
public:
	XGLProgram() {
		XMemLog::m_dwNew += sizeof(XGLProgram);
	}
	virtual ~XGLProgram() {
		XMemLog::m_dwNew -= sizeof(XGLProgram);
	}
public:
	static const unsigned short _XMP_Alloc_Num = 4;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo) {
		CCLOG("[XGLProgram::OnException]\n%s(%d) : %s", szFile, nLine, szInfo);
	}

	virtual bool OnCreate() {
		m_pShader = NULL;
		return true;
	}

	virtual void OnRelease(){
	}

public:
	XShader*	m_pShader;
};

// 灵魂效果，偏白蓝
class XShaderSoul :
	public XShader
{
public:
	XShaderSoul();
	virtual ~XShaderSoul() {
		XMemLog::m_dwNew -= sizeof(XShaderSoul);
	}
	inline void SetAlpha(unsigned char byAlpha) {
		SetParamf1(byAlpha / 255.f);
	}
	inline void SetTexRect(float fW, float fH) {
		m_fTexW = fW;
		m_fTexH = fH;
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nUniformAlpha;
	int				m_nUniformHurt;
	int				m_nUniformTW;
	GLfloat			m_fTexW;
	int				m_nUniformTH;
	GLfloat			m_fTexH;
public:
	inline XGLProgram* GetXProgram() {
		return m_pGLP;
	}
};

// Bloom全屏泛光
class XShaderBloom :
	public XShader
{
public:
	XShaderBloom();
	virtual ~XShaderBloom() {
		XMemLog::m_dwNew -= sizeof(XShaderBloom);
	}
	inline void SetTexSize(float fW, float fH, float fWeight) {
		m_fParam1 = fW;
		m_fParam2 = fH;
		m_fWeight = fWeight;
		RunBeforeDraw();
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
	virtual void	UseShader();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nUniformTW;
	int				m_nUniformTH;
	float			m_fWeight;
	int				m_nUniformWt;
};

// 热扭曲
class XShaderHeat :
	public XShader
{
public:
	XShaderHeat();
	virtual ~XShaderHeat() {
		XMemLog::m_dwNew -= sizeof(XShaderHeat);
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
	virtual void	UseShader();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nTexOffsetX;
	int				m_nTexOffsetY;
};

// Shader描边
class XShaderStroke :
	public XShader
{
public:
	XShaderStroke();
	virtual ~XShaderStroke() {
		XMemLog::m_dwNew -= sizeof(XShaderStroke);
	}
	inline void SetTexRect(float fW, float fH) {
		m_fTexW = fW;
		m_fTexH = fH;
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nUniformTW;
	GLfloat			m_fTexW;
	int				m_nUniformTH;
	GLfloat			m_fTexH;
	int				m_nUniformA;
	int				m_nUniformH;
public:
	inline XGLProgram* GetXProgram() {
		return m_pGLP;
	}
};

// Shader白色Alpha混合
class XShaderWhite :
	public XShader
{
public:
	XShaderWhite();
	virtual ~XShaderWhite() {
		XMemLog::m_dwNew -= sizeof(XShaderWhite);
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nUniformA;
public:
	inline XGLProgram* GetXProgram() {
		return m_pGLP;
	}
};

// Shader纯色Alpha混合
class XShaderColor :
	public XShader
{
public:
	XShaderColor();
	virtual ~XShaderColor() {
		XMemLog::m_dwNew -= sizeof(XShaderColor);
	}
	inline void SetColor(unsigned char byR, unsigned char byG, unsigned char byB) {
		const float fC = 1.f / 255.f;
		m_fCR = (float)byR * fC;
		m_fCG = (float)byG * fC;
		m_fCB = (float)byB * fC;
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int				m_nUniformA;
	GLfloat			m_fCR;
	GLfloat			m_fCG;
	GLfloat			m_fCB;
	int				m_nUniformR;
	int				m_nUniformG;
	int				m_nUniformB;
public:
	inline XGLProgram* GetXProgram() {
		return m_pGLP;
	}
};

// Shader Pal Color
class XShaderPal :
	public XShader
{
public:
	XShaderPal();
	virtual ~XShaderPal() {
		XMemLog::m_dwNew -= sizeof(XShaderPal);
	}
public:
	virtual void	RunBeforeDraw();
	virtual void	Release() {}
	virtual void	Reload();
	virtual void	UseShader();
protected:
	virtual const GLchar* GetShaderStr() {
		return NULL;
	}
	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP) {}
protected:
	XGLProgram*		m_pGLP;
	int		m_nUniformAlpha;
	int		m_nUniformPal;
public:
	inline XGLProgram* GetXProgram() {
		return m_pGLP;
	}
};

NS_CC_END

#endif /* __XSHADER_H__ */
