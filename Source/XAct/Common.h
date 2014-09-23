#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
#include "Cocos2dx/XDirector.h"
#include "Cocos2dx/layers_scenes_transitions_nodes/XScene.h"
#include "Cocos2dx/layers_scenes_transitions_nodes/XLayer.h"
#include "Cocos2dx/sprite_nodes/XSprite.h"
#include "Cocos2dx/sprite_nodes/XSpriteBatchNode.h"
#include "Cocos2dx/sprite_nodes/XSpriteFrameCache.h"
#include "Cocos2dx/textures/XTexture2D.h"
#include "Cocos2dx/textures/XTextureCache.h"
#include "Cocos2dx/misc_nodes/XRenderTexture.h"
#include "XHelper/XHelper.h"

extern "C" const char* GetRealPath(const char* szPath);

#ifdef CCLOG
#	undef CCLOG
#	define CCLOG	cocos2d::XDirector::DebugLog
#endif

extern XStrConvert _stXStrCnvMainTh;

//#include "XHelper/XMemParse.h"

#endif // __COMMON_H__
