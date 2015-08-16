#include <jni.h>

void hook_init();
void hook_zposed_method(JNIEnv* env, jobject thiz, jobject original, jobject proxy);
