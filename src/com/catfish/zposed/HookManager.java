package com.catfish.zposed;

import java.lang.reflect.Method;

import android.util.Log;

public class HookManager {
    private final static String TAG = "catfish";
    private static ThreadLocal<Boolean> sTag = new ThreadLocal<Boolean>();

    static {
        System.loadLibrary("hook");
        sTag.set(false);
    }

    public static Object onHooked(Method method, Object receiver, Object[] args) {
        sTag.set(true);
        Log.d(TAG, "onHooked: " + method, new Exception());
        return null;
    }

    public native static void hookZposedMethod(Method method);

    public static void resetTag() {
        sTag.set(false);
    }

    public static boolean getTag() {
        boolean result = sTag.get();
        return result;
    }
}
