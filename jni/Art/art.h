#include <jni.h>
#include <dlfcn.h>
#include "log.h"
#include "asm_support.h"
#include <stdio.h>
#include <stdlib.h>

#define JNIHOOK_CLASS "com/catfish/zposed/HookManager"
#define JNIENTRY_CLASS "com/catfish/zposed/ArtEntries"

jclass hookClass;
jclass methodClass;
JavaVM* gJVM;
jmethodID hookMethod;
jint art_jni_onload(JavaVM* vm, void* reserved);
jobject (*addWeakGloablReference)(JavaVM*, void*, void*);
