NS_CC_BEGIN

class XShaderETC1 :
	public XShader,
	public XMemPool<XShaderETC1>
{
public:
	XShaderETC1() {
		XMemLog::m_dwNew += sizeof(XShaderETC1);
	}
	virtual ~XShaderETC1() {
		XMemLog::m_dwNew -= sizeof(XShaderETC1);
	}
public:
	virtual const GLchar* GetShaderStr() {
		static GLchar szShader[] =
			"#ifdef GL_ES \n"
			"precision mediump float; \n"
			"#endif \n"
			"uniform sampler2D u_texture; \n"
			"uniform sampler2D u_alphatexture; \n"
			"varying vec2 v_texCoord; \n"
			"varying vec4 v_fragmentColor; \n"
			"void main(void) \n"
			"{ \n"
			" vec4 texColor = texture2D(u_texture, v_texCoord); \n "
			" texColor.a = texture2D(u_alphatexture, v_texCoord); \n "
			" gl_FragColor = v_fragmentColor * texColor; \n "
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
		m_nUniformTex = glGetUniformLocation(pGLP->getProgram(), "u_texture");
		m_nUniformAlpha = glGetUniformLocation(pGLP->getProgram(), "u_alphatexture");
	}
	virtual void RunBeforeDraw()
	{
		if (m_pGLP)
		{
			m_pGLP->setUniformLocationWith1i(m_nUniformTex, 0);
			m_pGLP->setUniformLocationWith1i(m_nUniformAlpha, 1);
		}
	}
	virtual void Release()
	{
		XShaderETC1* pShader = this;
		XMP_RELEASE(XShaderETC1, pShader);
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
	int		m_nUniformTex;
	int		m_nUniformAlpha;
public:
	static const unsigned short _XMP_Alloc_Num = 4;
public:
	virtual void OnException(const char* szFile, const int nLine, const char* szInfo)
	{
		CCLOG("[XShaderETC1::OnException]\n%s(%d)", szFile, nLine, szInfo);
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