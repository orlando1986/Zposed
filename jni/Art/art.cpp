#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "art.h"
#include "common.h"

static void (*art_quick_to_interpreter_bridge)(void*);

void hook_zposed_method(JNIEnv* env, jobject thiz, jobject original, jobject proxy) {
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

void hook_init() {
	void* handle = dlopen("libart.so", RTLD_LAZY | RTLD_GLOBAL);
	art_quick_to_interpreter_bridge = (void (*)(void*)) dlsym(handle, "art_quick_to_interpreter_bridge");
}
