#include "art.h"

#define BASE_TOKEN 4
#define ENTRY_SIZE 9 + BASE_TOKEN

extern int get_object_addr(int*);

static jclass gEntryClass;
static jmethodID gMethods[ENTRY_SIZE];
static jclass gObjectClass;

int* switch_entry(int flag) {
	return (int*) gMethods[flag];
}

void init_entries(JNIEnv* env) {
	jclass cls_entry = (*env)->FindClass(env, JNIENTRY_CLASS);
	gEntryClass  = (jclass) (*env)->NewGlobalRef(env, (jobject) cls_entry);
	jclass cls_object = (*env)->FindClass(env, "java/lang/Object");
	gObjectClass  = (jclass) (*env)->NewGlobalRef(env, (jobject) cls_object);

	gMethods[0] = ((*env)->GetStaticMethodID(env, hookClass, "getEntryTag", "()I"));
	gMethods[1] = ((*env)->GetStaticMethodID(env, gEntryClass, "getParamList", "(Ljava/lang/reflect/Method;)[I"));
	gMethods[2] = ((*env)->GetStaticMethodID(env, gEntryClass, "getObjectAddr", "(Ljava/lang/Object;)I"));
	gMethods[3] = hookMethod;

	gMethods[0 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;ILjava/lang/Object;)I"));
	gMethods[1 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;ID)I"));
	gMethods[2 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IJ)I"));
	gMethods[3 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IB)I"));
	gMethods[4 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IC)I"));
	gMethods[5 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IZ)I"));
	gMethods[6 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IF)I"));
	gMethods[7 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;IS)I"));
	gMethods[8 + BASE_TOKEN] = ((*env)->GetStaticMethodID(env, gEntryClass, "boxArgs", "([Ljava/lang/Object;II)I"));

	int artmeth = (int) gMethods[2];
	int* quick_entry_32 = (int*) (artmeth + METHOD_QUICK_CODE_OFFSET_32);
	*quick_entry_32 = (int) (&get_object_addr);
}

int box_args(void* artmethod, void* self, int* args) {
	JNIEnv* env = NULL;
	if ((*gJVM)->GetEnv(gJVM, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	jobject method = (*env)->ToReflectedMethod(env, methodClass, (jmethodID) artmethod, 0);
	jintArray params = (jintArray) (*env)->CallStaticObjectMethod(env, gEntryClass, gMethods[1], method);
	jsize size = (*env)->GetArrayLength(env, params);
	jobjectArray argbox = (jobjectArray)(*env)->NewObjectArray(env, size, gObjectClass, NULL);

	int* access_flag = (int*) (artmethod + METHOD_ACCESS_FLAG);
	if ((*access_flag & kAccStatic) != kAccStatic) {
		args++;
	}

	int index = 0;
	jint* elements = (*env)->GetIntArrayElements(env, params, (jboolean) 0);
	for (; index < size; index++) {
		jint offset = 1;
		int token = elements[index];
		switch(token) {
		case 0 + BASE_TOKEN: {
			jobject arg = (jobject) addWeakGloablReference(gJVM, self, (int*) (args[0]));
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 1 + BASE_TOKEN: {
			jdouble arg = *((jdouble*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 2 + BASE_TOKEN: {
			jlong arg = *((jlong*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 3 + BASE_TOKEN: {
			jbyte arg = *((jbyte*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 4 + BASE_TOKEN: {
			jchar arg = *((jchar*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 5 + BASE_TOKEN: {
			jboolean arg = *((jboolean*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 6 + BASE_TOKEN: {
			jfloat arg = *((jfloat*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 7 + BASE_TOKEN: {
			jshort arg = *((jshort*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		case 8 + BASE_TOKEN: {
			jint arg = *((jint*) args);
			offset = (*env)->CallStaticIntMethod(env, gEntryClass, gMethods[token], argbox, index, arg);
			break;}
		}
		args += offset;
	}
	return (int)(*env)->CallStaticIntMethod(env, gEntryClass, gMethods[2], argbox);
}
