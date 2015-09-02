package com.catfish.zposed;

import java.lang.reflect.Method;

public abstract class HookCallback {
    int ptr;
    public abstract Object onHook(Method artmethod, Object receiver, Object[] args);
}
