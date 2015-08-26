package com.catfish.zposed;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class HookManager {
    private final static String TAG = "catfish";
    private static ThreadLocal<Integer> sTag = new ThreadLocal<Integer>();
    private static Map<String, Integer> sMethodInfo = new HashMap<String, Integer>();
    private static int sHookPtr = 0;

    static {
        System.loadLibrary("hook");
        sHookPtr = obtainHookPtr();
        sTag.set(sHookPtr);
    }

    public static Object onHooked(Method method, int[] args) {
        try {
            Class<?> artmethod = Class.forName("java.lang.reflect.ArtMethod");
            Method getMethodName = artmethod.getDeclaredMethod("getMethodName", artmethod);
            String name = (String)getMethodName.invoke(null, method);
            Log.d(TAG, "onHooked: arg-" + args, new Exception());
            int ptr = sMethodInfo.get(name);
            if (ptr <= 0) {
                Log.e(TAG, "get wrong ptr for " + method + ", exit");
                return null;
            }
            sTag.set(ptr);
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        } catch (ClassNotFoundException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.toString());
        }
/*        try {
            return method.invoke(receiver, new Object[]{null, null});
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.toString());
        }*/
//        sTag.set(sHookPtr);
        return null;
    }

    public static void hookMethod(Method method) {
        int ptr = hookZposedMethod(method);
        sMethodInfo.put(method.getName(), ptr);
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
