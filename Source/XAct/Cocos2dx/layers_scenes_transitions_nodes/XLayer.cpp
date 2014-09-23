#include "../../Common.h"

XCPP_DEFINED();

NS_CC_BEGIN

XLayer::XLayer()
{
	XMemLog::m_dwNew += sizeof(XLayer);
}

XLayer::~XLayer()
{
	XMemLog::m_dwNew -= sizeof(XLayer);
}

bool XLayer::init()
{
	XFUNC_START();
    bool bRet = false;
    do 
    {        
        CCDirector * pDirector;
        CC_BREAK_IF(!(pDirector = XDirector::sharedDirector()));
        this->setContentSize(pDirector->getWinSize());
        m_bTouchEnabled = false;
        m_bAccelerometerEnabled = false;
        // success
        bRet = true;
    } while(0);
	return bRet;
	XFUNC_END_RETURN(bool, res, false);
}

unsigned long XLayer::OnFrame()
{
	XFUNC_START();
	return 0;
	XFUNC_END_RETURN(unsigned long, res, 0xFFFFFFFF);
}

void XLayer::onEnter()
{
	XScene* pScene = dynamic_cast<XScene*>(m_pParent);
	if(pScene)
	{
		pScene->XLM_AddLinkItem(this);
	}
	CCLayer::onEnter();
}

void XLayer::onExit()
{
	CCLayer::onExit();
	if(_pXLI_Parent)
	{
		XScene* pScene = dynamic_cast<XScene*>(m_pParent);
		if(pScene)
		{
			pScene->XLM_DelLinkItem(this);
		}
	}
}

NS_CC_END
