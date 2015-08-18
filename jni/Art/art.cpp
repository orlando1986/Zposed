#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "art.h"
#include "common.h"

extern "C" void art_quick_dispatcher(void*);
static JavaVM* gJVM;
static jclass methodClass = NULL;
static jclass hookClass = NULL;
static jmethodID hookMethod = NULL;

static JNINativeMethod gMethods[] =
		{ { "hookZposedMethod", "(Ljava/lang/reflect/Method;)V", (void*) hook_zposed_method } };

extern "C" uint64_t artQuickToDispatcher(void* method, void *self, u4 **args, u4 **old_sp) {
	LOGI("hook arrived");
	JNIEnv* env = NULL;
	if (gJVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("HOOK FAILED, DIDN'T GET env");
		return 0;
	}
	jobject meth_method = env->ToReflectedMethod(methodClass, (jmethodID) method, (jboolean) false);
	env->CallStaticObjectMethod(hookClass, hookMethod, meth_method, NULL, NULL);
	return 0;
}

static void hook_zposed_method(JNIEnv* env, jobject thiz, jobject method) {
	LOGI("native hook starts");

	jmethodID methid = env->FromReflectedMethod(method);
	uint32_t artmeth = uint32_t(methid);
	uint32_t* quick_entry_32 = (uint32_t*) (artmeth + 40);
	*quick_entry_32 = (uint32_t)(&art_quick_dispatcher);

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

static void init_member(JNIEnv* env) {
	void* handle = dlopen("libart.so", RTLD_LAZY | RTLD_GLOBAL);
	void* art_quick_to_interpreter_bridge =  (void*) dlsym(handle, "art_quick_to_interpreter_bridge");

	methodClass = (jclass) env->NewGlobalRef((jobject) env->FindClass("java/lang/reflect/Method"));

	jclass cls_hook = env->FindClass("com/catfish/zposed/HookManager");
	hookClass = (jclass) env->NewGlobalRef((jobject) cls_hook);
	hookMethod = env->GetStaticMethodID(cls_hook, "onHooked",
			"(Ljava/lang/reflect/Method;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
	uint32_t artmeth = uint32_t(hookMethod);
	uint32_t* quick_entry_32 = (uint32_t*) (artmeth + 40);
	*quick_entry_32 = (uint32_t)(art_quick_to_interpreter_bridge);
}

jint art_jni_onload(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;
	gJVM = vm;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0]))) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	init_member(env);

	return result;
}
