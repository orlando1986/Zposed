#include <jni.h>

#define JNIREG_CLASS "com/catfish/zposed/HookManager"

jint art_jni_onload(JavaVM* vm, void* reserved);
static jint hook_zposed_method(JNIEnv* env, jobject thiz, jobject method);
static jint obtain_hook_ptr(JNIEnv* env, jobject thiz);
