LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    main.cpp \
    Art/art.cpp \

LOCAL_MODULE := libhook
LOCAL_MODULE_TAGS := optional

LOCAL_ARM_MODE := thumb

LOCAL_DEPENDENCY := $(LOCAL_PATH)/Dependency
LOCAL_C_INCLUDES := $(LOCAL_DEPENDENCY)/include/
LOCAL_LDLIBS := -L$(LOCAL_DEPENDENCY)/lib/ -llog -landroid_runtime -lutils -lcutils -lart

LOCAL_CFLAGS := -std=gnu++11 -fpermissive -DDEBUG -O0 -fPIC 
#LOCAL_C_INCLUDES := 
include $(BUILD_SHARED_LIBRARY)
