#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "art.h"
#include "common.h"

extern "C" void art_quick_dispatcher(void*);
extern "C" uint64_t art_quick_call_entrypoint(void* method, void *self, u4 **args, u4 **old_sp, const void *entrypoint);
static JavaVM* gJVM;
static jclass methodClass = NULL;
static jclass objectClass = NULL;
static jclass hookClass = NULL;
static jmethodID hookMethod = NULL;
static jmethodID getTag = NULL;
static jmethodID resetTag = NULL;
static jobject (*addWeakGloablReferece)(JavaVM*, void*, void*);
static void (*art_quick_to_interpreter_bridge)(void*);

static JNINativeMethod gMethods[] =
		{ { "hookZposedMethod", "(Ljava/lang/reflect/Method;)V", (void*) hook_zposed_method } };

static int getArgLength(JNIEnv* env, jobject method) {
	return 3;
}

static jobjectArray BoxArgs(JNIEnv* env, jobject method, const char* shorty, u4** args, void* self) {
	int arg_len = getArgLength(env, method);
	jobjectArray args_jobj = env->NewObjectArray(arg_len, objectClass, NULL);;
	for (size_t i = 0; i < arg_len; ++i) {
		if (shorty[i] == 'L') {
			jobject val = addWeakGloablReferece(gJVM, self, args[i]);
			env->SetObjectArrayElement(args_jobj, i, val);
		} else {
//			jobject val = BoxPrimitive(shorty[i], args[i]);
//			env->SetObjectArrayElement(args_jobj, i, val);
		}
	}

	return args_jobj;
}

extern "C" uint64_t artQuickToDispatcher(void* method, void *self, u4 **args, u4 **old_sp) {
	LOGI("hook arrived");
	JNIEnv* env = NULL;
	if (gJVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("HOOK FAILED, DIDN'T GET env");
		return 0;
	}
	jboolean if_original = env->CallStaticBooleanMethod(hookClass, getTag);
	if (if_original) {
		return art_quick_call_entrypoint(method, self, args, old_sp, (const void*) art_quick_to_interpreter_bridge);
	}
	jobject meth_method = env->ToReflectedMethod(methodClass, (jmethodID) method, (jboolean) false);
	jobjectArray arg_array = BoxArgs(env, meth_method, "LLL", args, self);
	env->CallStaticObjectMethod(hookClass, hookMethod, meth_method, NULL, arg_array);
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
	art_quick_to_interpreter_bridge =  (void (*)(void*)) dlsym(handle, "art_quick_to_interpreter_bridge");
	addWeakGloablReferece = (jobject (*)(JavaVM*, void*, void*)) dlsym(handle, "_ZN3art9JavaVMExt22AddWeakGlobalReferenceEPNS_6ThreadEPNS_6mirror6ObjectE");

	methodClass = (jclass) env->NewGlobalRef((jobject) env->FindClass("java/lang/reflect/Method"));
	objectClass = (jclass) env->NewGlobalRef((jobject) env->FindClass("java/lang/Object"));

	jclass cls_hook = env->FindClass("com/catfish/zposed/HookManager");
	hookClass = (jclass) env->NewGlobalRef((jobject) cls_hook);
	getTag = env->GetStaticMethodID(cls_hook, "getTag", "()Z");
	resetTag = env->GetStaticMethodID(cls_hook, "resetTag", "()V");
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
