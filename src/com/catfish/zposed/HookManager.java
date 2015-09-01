package com.catfish.zposed;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class HookManager {
    final static String TAG = "catfish";
    private static ThreadLocal<Integer> sEntryTag = new ThreadLocal<Integer>();
    private static Map<String, Integer> sMethodInfo = new HashMap<String, Integer>();
    static {
        System.loadLibrary("hook");
        sEntryTag.set(0);
    }

    public native static int hookZposedMethod(Method method);

    public static void hookMethod(Method method) {
        int ptr = hookZposedMethod(method);
        sMethodInfo.put(method.toString(), ptr);
    }

    private static int getEntryTag() {
        int result = sEntryTag.get();
        sEntryTag.set(0);
        return result;
    }

    public static Object onHooked(Method method, Object receiver, Object[] args) {
        Log.d(TAG, "onHooked"/*, new Exception()*/);
        int ptr = sMethodInfo.get(method.toString());
        sEntryTag.set(ptr);
        Object result = notifyOnHooked(method, receiver, args);
        sEntryTag.set(0);
        return result;
    }

    private static Object notifyOnHooked(Method method, Object receiver, Object[] args) {
        try {
            method.invoke(receiver, args);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }
}
