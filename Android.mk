LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/cocos)

LOCAL_MODULE := ml_static

LOCAL_MODULE_FILENAME := libml

LOCAL_SRC_FILES := \
Animation.cpp \
AStar.cpp \
Audio/AudioEngine.cpp \
Audio/AudioMenu.cpp \
common.cpp \
Events.cpp \
FiniteStateMachine.cpp \
ImageManager.cpp \
IntrusivePtr.cpp \
Language.cpp \
loadxml/xmlLoader.cpp \
loadxml/xmlProperties.cpp \
loadxml/Crypto.cpp \
xtea/xtea.cpp \
MenuItem.cpp \
NodeExt.cpp \
ObjectFactory.cpp \
ParamCollection.cpp \
pugixml/pugixml.cpp \
ScrollMenu.cpp \
SmartScene.cpp \
SplashScene.cpp \
Panel.cpp \
Text.cpp \
mlUserData.cpp \
DataBase.cpp \
services/Advertisers.cpp \
services/mlStatistic.cpp \
services/inapp/Purchase.cpp \
services/inapp/Purchase_android.cpp \
services/chartboost/chartboost_android.cpp \
services/playservises/playservises_android.cpp \
services/admob/AdMob.cpp \
services/appgratis/appgratis.cpp \
services/appgratis/appgratis_android.cpp \
services/appsflyer/appsflyer_android.cpp \
services/flurry/flurry_android.cpp \
services/vungle/vungle_android.cpp \
services/tapjoy/tapjoy_android.cpp \
services/plugins/ServicePlugin.cpp \
services/adcolony/adcolony_android.cpp \
services/unityads/unityads_android.cpp \
services/appodeal/appodeal_android.cpp \
services/fyber/fyber_android.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/..

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)