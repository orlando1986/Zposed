#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "art.h"
#include "asm_support.h"
#include <stdio.h>

extern "C" void art_quick_dispatcher(void*);
extern "C" void art_quick_proxy(void*);
static JavaVM* gJVM;
static jclass methodClass = NULL;
static jclass objectClass = NULL;
static jclass hookClass = NULL;
static jmethodID hookMethod = NULL;
static jmethodID getTag = NULL;
static jobject (*addWeakGloablReferece)(JavaVM*, void*, void*);

static JNINativeMethod gMethods[] =
		{ { "hookZposedMethod", "(Ljava/lang/reflect/Method;)I", (void*) hook_zposed_method },
		  { "obtainHookPtr", "()I", (void*) obtain_hook_ptr }};

static jobjectArray BoxArgs(void* method, int* args, void* self) {
	LOGD("BoxArgs");
	return NULL;
}

static uint64_t artQuickToDispatcher(void* method, jobjectArray argsArray) {
	LOGD("artQuickToDispatcher");
	JNIEnv* env = NULL;
	if (gJVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("HOOK FAILED, DIDN'T GET env");
		return 0;
	}
	jobject meth_method = env->ToReflectedMethod(methodClass, (jmethodID) method, (jboolean) false);
	env->CallStaticObjectMethod(hookClass, hookMethod, meth_method, NULL, NULL);
	return 0;
}

extern "C" int switchEntry(int flag) {
	if (flag == 0) {
		JNIEnv* env = NULL;
		if (gJVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
			LOGE("HOOK FAILED, DIDN'T GET env");
			return 0;
		}
		return (int)env->CallStaticIntMethod(hookClass, getTag);
	} else if (flag == 1) {
		return (int)(&BoxArgs);
	} else if (flag == 2) {
		return (int)(&artQuickToDispatcher);
	}
}

static jint hook_zposed_method(JNIEnv* env, jobject thiz, jobject method) {
	jmethodID methid = env->FromReflectedMethod(method);
	int artmeth = int(methid);
	int* quick_entry_32 = (int*) (artmeth + METHOD_QUICK_CODE_OFFSET_32);
	jint ptr = (jint) *quick_entry_32;
	*quick_entry_32 = (int)(&art_quick_dispatcher);
	return ptr;
}

static jint obtain_hook_ptr(JNIEnv* env, jobject thiz) {
	return (int)(&art_quick_proxy);
}

static void init_member(JNIEnv* env) {
	void* handle = dlopen("libart.so", RTLD_LAZY | RTLD_GLOBAL);
	addWeakGloablReferece = (jobject (*)(JavaVM*, void*, void*)) dlsym(handle, "_ZN3art9JavaVMExt22AddWeakGlobalReferenceEPNS_6ThreadEPNS_6mirror6ObjectE");

	methodClass = (jclass) env->NewGlobalRef((jobject) env->FindClass("java/lang/reflect/Method"));
	objectClass = (jclass) env->NewGlobalRef((jobject) env->FindClass("java/lang/Object"));

	jclass cls_hook = env->FindClass("com/catfish/zposed/HookManager");
	hookClass = (jclass) env->NewGlobalRef((jobject) cls_hook);
	getTag = env->GetStaticMethodID(cls_hook, "getTag", "()I");
	hookMethod = env->GetStaticMethodID(cls_hook, "onHooked",
			"(Ljava/lang/reflect/Method;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
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
