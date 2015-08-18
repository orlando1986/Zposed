#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "art.h"
#include "common.h"

static void (*art_quick_to_interpreter_bridge)(void*);
static JNINativeMethod gMethods[] =
		{ { "hookZposedMethod", "(Ljava/lang/reflect/Method;Ljava/lang/reflect/Method;)V", (void*) hook_zposed_method } };

static void hook_zposed_method(JNIEnv* env, jobject thiz, jobject original, jobject proxy) {
	LOGI("native hook starts");

	uint32_t proxy_entry;
	{
	jmethodID methid = env->FromReflectedMethod(proxy);
	uint32_t artmeth = uint32_t(methid);
	uint32_t* interpreter_entry_32 = (uint32_t*) (artmeth + 32);
	proxy_entry = *interpreter_entry_32;
	}

	{
	jmethodID methid = env->FromReflectedMethod(original);
	uint32_t artmeth = uint32_t(methid);
	uint32_t* interpreter_entry_32 = (uint32_t*) (artmeth + 32);
	*interpreter_entry_32 = proxy_entry;

	uint32_t* quick_entry_32 = (uint32_t*) (artmeth + 40);
	*quick_entry_32 = (uint32_t)(art_quick_to_interpreter_bridge);
	}
	LOGI("native hook ends");
}

static int registerNativeMethods(JNIEnv* env, const char* className,
		JNINativeMethod* gMethods, int numMethods) {
	jclass clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

jint art_jni_onload(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0]))) {
		return -1;
	}
	result = JNI_VERSION_1_4;

	void* handle = dlopen("libart.so", RTLD_LAZY | RTLD_GLOBAL);
	art_quick_to_interpreter_bridge = (void (*)(void*)) dlsym(handle, "art_quick_to_interpreter_bridge");

	return result;
}
