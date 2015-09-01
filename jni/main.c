#include <jni.h>
#include <stdio.h>
#include "Art/art.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	return art_jni_onload(vm, reserved);
}
