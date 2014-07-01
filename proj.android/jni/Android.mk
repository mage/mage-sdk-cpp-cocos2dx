LOCAL_PATH := $(call my-dir)

#
# cURL
#
include $(CLEAR_VARS)

LOCAL_MODULE := curl
LOCAL_SRC_FILES := ../../Vendors/mage-sdk-cpp/platforms/externals/curl/prebuilt/android/armeabi/libcurl.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/platforms/externals/curl/include/android

include $(PREBUILT_STATIC_LIBRARY)

#
# Define MAGE-SDK
#
include $(CLEAR_VARS)

LOCAL_MODULE := mage-sdk
LOCAL_SRC_FILES := ../../Vendors/mage-sdk-cpp/platforms/android/obj/local/armeabi/libmage.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/src
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/vendor/libjson-rpc-cpp/src

include $(PREBUILT_STATIC_LIBRARY)

#
# The application itself
#
include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp

LOCAL_CPPFLAGS := -fexceptions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += curl
LOCAL_WHOLE_STATIC_LIBRARIES += mage-sdk

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
