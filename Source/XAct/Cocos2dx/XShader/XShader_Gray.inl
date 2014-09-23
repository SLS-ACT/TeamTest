NS_CC_BEGIN

class XShaderGray :
	public XShader,
	public XMemPool<XShaderGray>
{
public:
	XShaderGray() {
		XMemLog::m_dwNew += sizeof(XShaderGray);
	}
	virtual ~XShaderGray() {
		XMemLog::m_dwNew -= sizeof(XShaderGray);
	}
public:
	virtual const GLchar* GetShaderStr() {
		static GLchar szShader[] =
			"#ifdef GL_ES \n"
			"precision mediump float; \n"
			"#endif \n"
			"uniform sampler2D u_texture; \n"
			"varying vec2 v_texCoord; \n"
			"varying vec4 v_fragmentColor; \n"
			"void main(void) \n"
			"{ \n"
			" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
			" float grey = dot(texColor.rgb, vec3(0.299, 0.587, 0.114)); \n "
			" gl_FragColor = vec4(grey, grey ,grey ,texColor.a); \n "
			"}";
		return szShader;
	}

	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP)
	{
		if (!pGLP)
		{
			return;
		}
		m_pNode = pNode;
		m_pGLP	= pGLP;
	}
	virtual void RunBeforeDraw()
	{

	}
	virtual void Release()
	{
		XShaderGray* pShader = this;
		XMP_RELEASE(XShaderGray, pShader);
		if (m_pNode)
		{
			if (m_pOldShader)
			{
				m_pNode->setShaderProgram(m_pOldShader);
				m_pOldShader->release();
				m_pOldShader = NULL;
			}
			else
			{
				m_pNode->setShaderProgram(NULL);
			}
			m_pNode = NULL;
		}
		if (m_pGLP)
		{
			XMP_RELEASE(XGLProgram, m_pGLP);
			m_pGLP = NULL;
		}
	}
	virtual void Reload() {}
public:
	static const unsigned short _XMP_Alloc_Num = 4;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XShaderGray::OnException]\n%s(%d)", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		return true;
	}

	virtual void OnRelease()
	{

	}
};

class XShaderGrayChange :
	public XShader,
	public XMemPool<XShaderGrayChange> // memory pool
{
public:
	XShaderGrayChange() {
		XMemLog::m_dwNew += sizeof(XShaderGrayChange);
	}
	virtual ~XShaderGrayChange() {
		XMemLog::m_dwNew -= sizeof(XShaderGrayChange);
	}
public:
	// 参数说明
	// GLfloat m_fParam1; // 灰度图变化， 0.f[全彩]~1.f [全灰度]
	// GLfloat m_fParam2; // 透明度变化， 0.f[全透]~1.f [不透]
public:
	virtual const GLchar* GetShaderStr()
	{
		static GLchar szShader[] =
			"#ifdef GL_ES \n"
			"precision mediump float; \n"
			"#endif \n"
			"uniform sampler2D u_texture; \n"
			"uniform float f_gray; \n "
			"uniform float f_alpha; \n "
			"varying vec2 v_texCoord; \n"
			"varying vec4 v_fragmentColor; \n"
			"void main(void) \n"
			"{ \n"
			"	vec4 texColor = texture2D(u_texture, v_texCoord); \n "
			"	float grey = dot(texColor.rgb, vec3(0.299, 0.587, 0.114)); \n "
			"	grey = grey*f_gray; \n	"
			"	float col = 1.0 - f_gray; \n"
			"	texColor[0] = texColor[0]*col + grey; \n"
			"	texColor[1] = texColor[0]*col + grey; \n"
			"	texColor[2] = texColor[0]*col + grey; \n"
			"	texColor = texColor * f_alpha; \n"
			"	gl_FragColor = vec4(texColor[0], texColor[1] ,texColor[2] ,texColor.a); \n "
			"}";
		return szShader; 
	}

	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP)
	{
		if (!pGLP)
		{
			return;
		}

		m_pNode = pNode;
		m_pGLP	= pGLP;

		m_nUniformGray = glGetUniformLocation(pGLP->getProgram(), "f_gray");
		m_fParam1 = 0.f;
		m_nUniformAlpha = glGetUniformLocation(pGLP->getProgram(), "f_alpha");
		m_nUniformAlpha = 1.f;
	}
	virtual void RunBeforeDraw()
	{
		if (m_pGLP)
		{
			m_pGLP->setUniformLocationWith1f(m_nUniformGray, m_fParam1);
			m_pGLP->setUniformLocationWith1f(m_nUniformAlpha, m_fParam2);
		}
	}
	virtual void Release()
	{
		XShaderGrayChange* pShader = this;
		XMP_RELEASE(XShaderGrayChange, pShader);
		if (m_pNode)
		{
			if (m_pOldShader)
			{
				m_pNode->setShaderProgram(m_pOldShader);
				m_pOldShader->release();
				m_pOldShader = NULL;
			}
			else
			{
				m_pNode->setShaderProgram(NULL);
			}
			m_pNode = NULL;
		}
		if (m_pGLP)
		{
			XMP_RELEASE(XGLProgram, m_pGLP);
			m_pGLP = NULL;
		}
	}
	virtual void Reload() {}
protected:
	int		m_nUniformGray;
	int		m_nUniformAlpha;
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XShaderGray::OnException]\n%s(%d)", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		return true;
	}

	virtual void OnRelease()
	{

	}
};

class XShaderWhiteChange :
	public XShader,
	public XMemPool<XShaderWhiteChange> // memory pool
{
public:
	XShaderWhiteChange() {
		XMemLog::m_dwNew += sizeof(XShaderWhiteChange);
	}
	virtual ~XShaderWhiteChange() {
		XMemLog::m_dwNew -= sizeof(XShaderWhiteChange);
	}
public:
	// 参数说明
	// GLfloat m_fParam1; // 自动白色变化，不需要设置 0.f[全彩色]~1.f [全白]
public:
	virtual const GLchar* GetShaderStr()
	{
		static GLchar szShader[] =
			"#ifdef GL_ES \n"
			"precision mediump float; \n"
			"#endif \n"
			"uniform sampler2D u_texture; \n"
			"varying vec2 v_texCoord; \n"
			"varying vec4 v_fragmentColor; \n"
			"void main(void) \n"
			"{ \n"
			"	vec4 texColor = texture2D(u_texture, v_texCoord); \n "
			"	if(texColor.a <= 0.05) \n"
			"	{ \n"
			"		gl_FragColor = vec4(0.0, 0.0, 0.0); \n "
			"		return; \n"
			"	} \n "
			"	float fcolor = texColor[0] + texColor[1] + texColor[2]; \n "
			"	if(fcolor <= 0.1) \n "
			"	{ \n "
			"		gl_FragColor = vec4(0.0, 0.0, 0.0); \n "
			"		return; \n"
			"	} \n"
			"	texColor[0] = 1.0; \n"
			"	texColor[1] = 1.0; \n"
			"	texColor[2] = 1.0; \n"
			"	texColor *= texColor.a; \n"
			"	gl_FragColor = vec4(texColor[0], texColor[1] ,texColor[2] ,texColor.a); \n "
			"}";
		return szShader; 
	}

	virtual void InitForNode(CCNode* pNode, XGLProgram* pGLP)
	{
		if (!pGLP)
		{
			return;
		}

		m_pNode = pNode;
		m_pGLP	= pGLP;
	}
	virtual void RunBeforeDraw()
	{

	}
	virtual void Release()
	{
		XShaderWhiteChange* pShader = this;
		XMP_RELEASE(XShaderWhiteChange, pShader);
		if (m_pNode)
		{
			if (m_pOldShader)
			{
				m_pNode->setShaderProgram(m_pOldShader);
				m_pOldShader->release();
				m_pOldShader = NULL;
			}
			else
			{
				m_pNode->setShaderProgram(NULL);
			}
			m_pNode = NULL;
		}
		if (m_pGLP)
		{
			XMP_RELEASE(XGLProgram, m_pGLP);
			m_pGLP = NULL;
		}
	}
	virtual void Reload() {}
public:
	static const unsigned short _XMP_Alloc_Num = 128;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XShaderGray::OnException]\n%s(%d)", szFile, nLine, szInfo);
	}
	virtual bool OnCreate()
	{
		return true;
	}

	virtual void OnRelease()
	{

	}
};

NS_CC_END