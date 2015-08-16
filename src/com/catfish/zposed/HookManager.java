package com.catfish.zposed;

import java.lang.reflect.Method;

public class HookManager {
    public native static void hookZposedMethod(Method original, Method proxy);
}
