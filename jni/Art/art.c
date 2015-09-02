#include "art.h"
#include <stdarg.h>

extern void art_quick_proxy(void*);
extern void init_entries(JNIEnv* env);

static jint hook_zposed_method(JNIEnv* env, jobject thiz, jobject method) {
	jmethodID methid = (*env)->FromReflectedMethod(env, method);
	int artmeth = (int) methid;
	int* quick_entry_32 = (int*) (artmeth + METHOD_QUICK_CODE_OFFSET_32);
	jint ptr = (jint)* quick_entry_32;
	*quick_entry_32 = (int) (&art_quick_proxy);

	int* access_flag = (int*) (artmeth + METHOD_ACCESS_FLAG);
	*access_flag = *access_flag | kAccNative;

	int* mapping_table = (int*) (artmeth + METHOD_MAPPING_TABLE);
	*mapping_table = 0;
	return ptr;
}

static JNINativeMethod gMethods[] = {
		{ "hookZposedMethod", "(Ljava/lang/reflect/Method;)I", (void*) hook_zposed_method },
		};

static void init_member(JNIEnv* env) {
	void* handle = dlopen("libart.so", RTLD_LAZY | RTLD_GLOBAL);
	addWeakGloablReference = (jobject (*)(JavaVM*, void*, void*)) dlsym(handle, "_ZN3art9JavaVMExt22AddWeakGlobalReferenceEPNS_6ThreadEPNS_6mirror6ObjectE");

	jclass cls_method = (*env)->FindClass(env, "java/lang/reflect/Method");
	methodClass  = (jclass) (*env)->NewGlobalRef(env, (jobject) cls_method);

	jclass cls_hook = (*env)->FindClass(env, JNIHOOK_CLASS);
	hookClass = (jclass) (*env)->NewGlobalRef(env, (jobject) cls_hook);

	hookMethod = (*env)->GetStaticMethodID(env, cls_hook, "onHooked",
			"(Ljava/lang/Object;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
}

static int registerNativeMethods(JNIEnv* env, const char* className,
		JNINativeMethod* gMethods, int numMethods) {
	jclass clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

jint art_jni_onload(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;
	gJVM = vm;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	if (!registerNativeMethods(env, JNIHOOK_CLASS, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0]))) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	init_member(env);
	init_entries(env);

	return result;
}
