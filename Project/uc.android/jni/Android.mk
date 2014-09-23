LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -lGLESv2 \
                -lEGL

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp  \
                   ../../../Source/XAct/AppDelegate.cpp \
                   ../../../Source/XAct/Cocos2dx/layers_scenes_transitions_nodes/XLayer.cpp \
                   ../../../Source/XAct/Cocos2dx/layers_scenes_transitions_nodes/XScene.cpp \
                   ../../../Source/XAct/Cocos2dx/misc_nodes/XRenderTexture.cpp \
                   ../../../Source/XAct/Cocos2dx/sprite_nodes/XScale9Sprite.cpp \
                   ../../../Source/XAct/Cocos2dx/sprite_nodes/XSprite.cpp \
                   ../../../Source/XAct/Cocos2dx/sprite_nodes/XSpriteBatchNode.cpp \
                   ../../../Source/XAct/Cocos2dx/sprite_nodes/XSpriteFrameCache.cpp \
                   ../../../Source/XAct/Cocos2dx/textures/DDS/S3TC.cpp \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/checkheader.c \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/errstr.c \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/etcdec.cpp \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/etcunpack.cpp \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/hashtable.c \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/loader.c \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/swap.c \
                   ../../../Source/XAct/Cocos2dx/textures/ETC/writer.c \
                   ../../../Source/XAct/Cocos2dx/textures/XTexture2D.cpp \
                   ../../../Source/XAct/Cocos2dx/textures/XTextureCache.cpp \
                   ../../../Source/XAct/Cocos2dx/XDirector.cpp \
                   ../../../Source/XAct/Cocos2dx/XFileUtils.cpp \
                   ../../../Source/XAct/Cocos2dx/XLoader/XLoader.cpp \
                   ../../../Source/XAct/Cocos2dx/XLoader/XLoader_PNG.cpp \
                   ../../../Source/XAct/Cocos2dx/XLoader/XLoader_Tex.cpp \
                   ../../../Source/XAct/Cocos2dx/XShader/XShader.cpp \
                   ../../../Source/XAct/Game/GameScript.cpp \
                   ../../../Source/XAct/Game/MaskWord.cpp \
                   ../../../Source/XAct/Game/MaskWordScript.cpp \
                   ../../../Source/XAct/Game/XCityStateScript.cpp \
                   ../../../Source/XAct/Game/XGameState.cpp \
                   ../../../Source/XAct/Game/XGameStateBaseScript.cpp \
                   ../../../Source/XAct/Game/XGameStateMangerScript.cpp \
                   ../../../Source/XAct/Game/XInstZoneStateScript.cpp \
                   ../../../Source/XAct/Game/XSelRoleStateScript.cpp \
                   ../../../Source/XAct/Game/XTimeManager.cpp \
                   ../../../Source/XAct/HelloWorldScene.cpp \
                   ../../../Source/XAct/Role/Animation/XAnimation.cpp \
                   ../../../Source/XAct/Role/XEffect.cpp \
                   ../../../Source/XAct/Role/XRole.cpp \
                   ../../../Source/XAct/Role/XRoleAI.cpp \
                   ../../../Source/XAct/Role/XRoleManager.cpp \
                   ../../../Source/XAct/Role/XRoleScript.cpp \
                   ../../../Source/XAct/SDK/XSdkManager.cpp \
                   ../../../Source/XAct/UpdateScene.cpp \
                   ../../../Source/XAct/XCamera/XCamera.cpp \
                   ../../../Source/XAct/XConfig.cpp \
                   ../../../Source/XAct/XControl/XJoystick.cpp \
                   ../../../Source/XAct/XControl/XJSCtrl.cpp \
                   ../../../Source/XAct/XControl/XJSGesture.cpp \
                   ../../../Source/XAct/XEvent/XEvent.cpp \
                   ../../../Source/XAct/XEvent/XEventManger.cpp \
                   ../../../Source/XAct/XHelper/XStrConvert.cpp \
                   ../../../Source/XAct/XHelper/XTable/XTable.cpp \
                   ../../../Source/XAct/XHelper/XTable/XTableFile.cpp \
                   ../../../Source/XAct/XMap/XCreateRoleMap.cpp \
                   ../../../Source/XAct/XMap/XIZMap.cpp \
                   ../../../Source/XAct/XMap/XIZMapScript.cpp \
                   ../../../Source/XAct/XMap/XMap.cpp \
                   ../../../Source/XAct/XMap/XMapCollide.cpp \
                   ../../../Source/XAct/XMap/XMapScript.cpp \
                   ../../../Source/XAct/XNet/XNetWork.cpp \
                   ../../../Source/XAct/XNet/XProtocol.cpp \
                   ../../../Source/XAct/XNet/XSocket.cpp \
                   ../../../Source/XAct/XNumEffect/XComboEffect.cpp \
                   ../../../Source/XAct/XNumEffect/XNumEffect.cpp \
                   ../../../Source/XAct/XSound/XSoundManager.cpp \
                   ../../../Source/XAct/XSound/XSoundManagerScript.cpp \
                   ../../../Source/XAct/XUI/Control/XButton.cpp \
                   ../../../Source/XAct/XUI/Control/XButtonScript.cpp \
                   ../../../Source/XAct/XUI/Control/XComplexLabel.cpp \
                   ../../../Source/XAct/XUI/Control/XDataGrid.cpp \
                   ../../../Source/XAct/XUI/Control/XDataGridScript.cpp \
                   ../../../Source/XAct/XUI/Control/XFuncPanel.cpp \
                   ../../../Source/XAct/XUI/Control/XFuncPanelScript.cpp \
                   ../../../Source/XAct/XUI/Control/XHeadEffect.cpp \
                   ../../../Source/XAct/XUI/Control/XHeadEffectScript.cpp \
                   ../../../Source/XAct/XUI/Control/XImageNum.cpp \
                   ../../../Source/XAct/XUI/Control/XImageNumScript.cpp \
                   ../../../Source/XAct/XUI/Control/XInputText.cpp \
                   ../../../Source/XAct/XUI/Control/XInputTextScript.cpp \
                   ../../../Source/XAct/XUI/Control/XItemWindow.cpp \
                   ../../../Source/XAct/XUI/Control/XItemWindowScript.cpp \
                   ../../../Source/XAct/XUI/Control/XLabel.cpp \
                   ../../../Source/XAct/XUI/Control/XLabelScript.cpp \
                   ../../../Source/XAct/XUI/Control/XLinker.cpp \
                   ../../../Source/XAct/XUI/Control/XLinkerScript.cpp \
                   ../../../Source/XAct/XUI/Control/XList.cpp \
                   ../../../Source/XAct/XUI/Control/XListScript.cpp \
                   ../../../Source/XAct/XUI/Control/XMagicWindow.cpp \
                   ../../../Source/XAct/XUI/Control/XMagicWindowScript.cpp \
                   ../../../Source/XAct/XUI/Control/XNoticeLabel.cpp \
                   ../../../Source/XAct/XUI/Control/XNoticeLabelScript.cpp \
                   ../../../Source/XAct/XUI/Control/XPanel.cpp \
                   ../../../Source/XAct/XUI/Control/XPanelScript.cpp \
                   ../../../Source/XAct/XUI/Control/XProssBar.cpp \
                   ../../../Source/XAct/XUI/Control/XProssBarScript.cpp \
                   ../../../Source/XAct/XUI/Control/XRadioButton.cpp \
                   ../../../Source/XAct/XUI/Control/XRadioButtonGroup.cpp \
                   ../../../Source/XAct/XUI/Control/XRadioButtonGroupScript.cpp \
                   ../../../Source/XAct/XUI/Control/XRadioButtonScript.cpp \
                   ../../../Source/XAct/XUI/Control/XScrollBar.cpp \
                   ../../../Source/XAct/XUI/Control/XScrollBarScript.cpp \
                   ../../../Source/XAct/XUI/Control/XSlider.cpp \
                   ../../../Source/XAct/XUI/Control/XSliderScript.cpp \
                   ../../../Source/XAct/XUI/Control/XTimerLabel.cpp \
                   ../../../Source/XAct/XUI/Control/XTimerLabelScript.cpp \
                   ../../../Source/XAct/XUI/Control/XUIRole.cpp \
                   ../../../Source/XAct/XUI/Control/XUIRoleScript.cpp \
                   ../../../Source/XAct/XUI/Control/XWheelWindow.cpp \
                   ../../../Source/XAct/XUI/Control/XWheelWindowScript.cpp \
                   ../../../Source/XAct/XUI/XBaseWindow.cpp \
                   ../../../Source/XAct/XUI/XBaseWindowScript.cpp \
                   ../../../Source/XAct/XUI/XFont/XFont.cpp \
                   ../../../Source/XAct/XUI/XUIManger.cpp \
                   ../../../Source/XAct/XUI/XUIMangerScript.cpp \
                   ../../../Source/XAct/XUI/XWindowType.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Source/XAct
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Source/cocos2dx/platform/third_party/android/prebuilt/libcurl/include

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, C:/XAct/Source)
$(call import-add-path, C:/XAct/Source/cocos2dx/platform/third_party/android/prebuilt)
$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,lua/proj.android/jni)
