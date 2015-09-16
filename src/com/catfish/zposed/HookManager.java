package com.catfish.zposed;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class HookManager {
    final static String TAG = "catfish";
    private static ThreadLocal<Integer> sEntryTag = new ThreadLocal<Integer>();
    private static Map<String, HookCallback> sMethodInfo = new HashMap<String, HookCallback>();
    static {
        System.loadLibrary("hook");
        sEntryTag.set(0);
    }

    private native static int hookZposedMethod(Method method);

    public static void hookMethod(Method method, HookCallback callback) {
        callback.ptr = hookZposedMethod(method);
        sMethodInfo.put(method.toString(), callback);
    }

    private static int getEntryTag() {
        int result = sEntryTag.get();
        sEntryTag.set(0);
        return result;
    }

    static Object onHooked(Object artmethod, Object receiver, Object[] args) {
        Method method = instanceMethod(artmethod);
        HookCallback c = sMethodInfo.get(method.toString());
        sEntryTag.set(c.ptr);
        boolean isStatic = Modifier.isStatic(method.getModifiers());
        Object result = null;
        try {
            result = c.onHook(method, isStatic ? null : receiver, args);
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
        sEntryTag.set(0);
        return result;
    }

    private static Method instanceMethod(Object artmethod) {
        try {
            Constructor<?> c = Method.class.getDeclaredConstructor(artmethod.getClass());
            return (Method) c.newInstance(artmethod);
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        } catch (InstantiationException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.toString());
        }
        return null;
    }

}
