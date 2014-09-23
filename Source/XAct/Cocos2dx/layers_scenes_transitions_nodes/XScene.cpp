#include "../../Common.h"

XCPP_DEFINED();

NS_CC_BEGIN

XScene::XScene()
{
	m_bIgnoreAnchorPointForPosition = true;
	setAnchorPoint(ccp(0.5f, 0.5f));
	XMemLog::m_dwNew += sizeof(XScene);
}

XScene::~XScene()
{
	XMemLog::m_dwNew -= sizeof(XScene);
}

XScene *XScene::create()
{
	XFUNC_START();
	XScene *pRet = new XScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	XFUNC_END_RETURN(XScene*, ptr, NULL);
}

bool XScene::init()
{
	XFUNC_START();
	bool bRet = false;
	do 
	{
		XDirector * pDirector;
		CC_BREAK_IF( ! (pDirector = XDirector::sharedDirector()) );
		this->setContentSize(pDirector->getWinSize());
		// success
		bRet = true;
	} while (0);
	return bRet;
	XFUNC_END_RETURN(bool, res, false);
}

unsigned long XScene::OnFrame()
{
	XFUNC_START();
	unsigned long dwRun = ChildrenOnFrame();
	dwRun += 1;
	return dwRun;
	XFUNC_END_RETURN(unsigned long, res, 0xFFFFFFFF);
}

XLayer* XScene::getRunLayer()
{
	XFUNC_START();
	if(XLinkManager<XLayer>::_pXLM_ItemListH && XLinkManager<XLayer>::_dwXLM_Num > 0)
	{
		XLayer* pLItem = XLinkManager<XLayer>::_pXLM_ItemListH;
		CCLOG("lihaofeng__:XScene::getRunLayer__4325___%d", pLItem);
		return pLItem;
	}
	XFUNC_END_RETURN(XLayer*, ptr, NULL);
}


NS_CC_END
