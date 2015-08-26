package com.catfish.zposed;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class HookManager {
    private final static String TAG = "catfish";
    private static ThreadLocal<Integer> sTag = new ThreadLocal<Integer>();
    private static Map<Method, Integer> sMethodInfo = new HashMap<Method, Integer>();
    private static int sHookPtr = 0;

    static {
        System.loadLibrary("hook");
        sHookPtr = obtainHookPtr();
        sTag.set(sHookPtr);
    }

    public static Object onHooked(Method method, Object receiver, Object[] args) {
        int ptr = sMethodInfo.get(method);
        if (ptr <= 0) {
            Log.e(TAG, "get wrong ptr for " + method + ", exit");
            return null;
        }
        sTag.set(ptr);
        Log.d(TAG, "onHooked", new Exception());
/*        try {
            return method.invoke(receiver, new Object[]{null, null});
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.toString());
        }*/
        sTag.set(sHookPtr);
        return null;
    }

    public static void hookMethod(Method method) {
        int ptr = hookZposedMethod(method);
        sMethodInfo.put(method, ptr);
    }

    private native static int hookZposedMethod(Method method);

    public native static int obtainHookPtr();

    public static int getTag() {
        int result = sTag.get();
        if (result == 0) {
            result = sHookPtr;
        }
        sTag.set(sHookPtr);
        return result;
    }
}
