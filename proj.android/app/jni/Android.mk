LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
$(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
$(LOCAL_PATH)/../../../Classes/StageManager.cpp \
$(LOCAL_PATH)/../../../Classes/Account.cpp \
$(LOCAL_PATH)/../../../Classes/maze/Animal.cpp \
$(LOCAL_PATH)/../../../Classes/maze/MazeLayer.cpp \
$(LOCAL_PATH)/../../../Classes/maze/MazeAlphaLayer.cpp \
$(LOCAL_PATH)/../../../Classes/maze/PathLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/LogoLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/TitleLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/CategoryLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/StageSelectLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/MoreLayer.cpp \
$(LOCAL_PATH)/../../../Classes/menu/ShopLayer.cpp \
$(LOCAL_PATH)/../../../Classes/popup/ExitPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/WaitPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/ShopPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/TimeupPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/PausePopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/HintPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/ClearPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/ViewPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/DailyPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/CollectPopup.cpp \
$(LOCAL_PATH)/../../../Classes/popup/PopupManager.cpp \
$(LOCAL_PATH)/../../../Classes/ads/AppnextAds.cpp

LOCAL_CPPFLAGS := -DSDKBOX_ENABLED \
-DSDKBOX_COCOS_CREATOR
LOCAL_LDLIBS := -landroid \
-llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
$(LOCAL_PATH)/../../../Classes/maze \
$(LOCAL_PATH)/../../../Classes/menu \
$(LOCAL_PATH)/../../../Classes/popup \
$(LOCAL_PATH)/../../../Classes/ads

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_WHOLE_STATIC_LIBRARIES += PluginFirebase
LOCAL_WHOLE_STATIC_LIBRARIES += sdkbox
LOCAL_WHOLE_STATIC_LIBRARIES += PluginGoogleAnalytics
LOCAL_WHOLE_STATIC_LIBRARIES += PluginSdkboxAds
LOCAL_WHOLE_STATIC_LIBRARIES += PluginAdMob
LOCAL_WHOLE_STATIC_LIBRARIES += PluginAppnext

LOCAL_STATIC_LIBRARIES := cc_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path,$(LOCAL_PATH))

$(call import-module, cocos)
$(call import-module, ./sdkbox)
$(call import-module, ./PluginFirebase)
$(call import-module, ./PluginGoogleAnalytics)
$(call import-module, ./PluginSdkboxAds)
$(call import-module, ./PluginAdmob)
$(call import-module, ./PluginAppnext)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
