package com.catfish.zposed;

import java.lang.reflect.Constructor;
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

    public static Object onHooked(Object artmethod, Object receiver, Object[] args) {
//        Log.d(TAG, "onHooked receiver=" + receiver + ", args0=" + args[0] + ", args1=" + args[1] + ", args2=" + args[2] /*, new Exception()*/);
//        Log.d(TAG, "onHooked receiver=" + receiver + ", len=" + args.length);
        Method method = instanceMethod(artmethod);
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
