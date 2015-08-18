LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    main.cpp \
    Art/art.cpp\
    Art/art_quick_proxy.S

LOCAL_MODULE := libhook
LOCAL_MODULE_TAGS := optional

LOCAL_ARM_MODE := thumb

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_DEPENDENCY := $(LOCAL_PATH)/Dependency
LOCAL_C_INCLUDES := $(LOCAL_DEPENDENCY)/include/

LOCAL_CFLAGS := -std=gnu++11 -fpermissive -DDEBUG -O0 -fPIC 
#LOCAL_C_INCLUDES := 
include $(BUILD_SHARED_LIBRARY)
