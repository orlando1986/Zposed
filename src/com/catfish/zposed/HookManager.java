package com.catfish.zposed;

import java.lang.reflect.Method;

import android.util.Log;

import com.catfish.zposed.ui.MainService;

public class HookManager {
    static {
        System.loadLibrary("hook");
    }

    public static Object onHooked(Method method, Object receiver, Object[] args) {
        Class clazz = MainService.class;
        Log.d("catfish", "onHooked: " + clazz);
        return null;
    }

    public native static void hookZposedMethod(Method method);
}
