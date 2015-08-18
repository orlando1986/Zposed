#include <jni.h>

#define JNIREG_CLASS "com/catfish/zposed/HookManager"

jint art_jni_onload(JavaVM* vm, void* reserved);
static void hook_zposed_method(JNIEnv* env, jobject thiz, jobject original, jobject proxy);
