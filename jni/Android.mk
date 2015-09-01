LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    main.c \
    Art/art.c\
    Art/art_quick_proxy.S \
    Art/art_entries.c \


LOCAL_MODULE := libhook
LOCAL_MODULE_TAGS := optional

LOCAL_ARM_MODE := thumb

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

#LOCAL_CFLAGS := -DDEBUG -O0 -fPIC 
#LOCAL_C_INCLUDES := 
include $(BUILD_SHARED_LIBRARY)
