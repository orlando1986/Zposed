#ifndef LOG_H_
#define LOG_H_

#include <android/log.h>

#define TAG    "catfish"

#define DEBUG 1
#define LOGV(fmt, args...) if (DEBUG) __android_log_print(ANDROID_LOG_VERBOSE, TAG, fmt, ##args)
#define LOGI(fmt, args...) if (DEBUG) __android_log_print(ANDROID_LOG_INFO, TAG, fmt, ##args)
#define LOGD(fmt, args...) if (DEBUG) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) if (DEBUG) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
#define LOGW(fmt, args...) if (DEBUG) __android_log_print(ANDROID_LOG_WARN, TAG, fmt, ##args)

#endif
