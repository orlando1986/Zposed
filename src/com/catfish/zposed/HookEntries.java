package com.catfish.zposed;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import android.util.Log;

public class HookEntries {
    private final static String TAG = "catfish";
    private final static int BASE_TOKEN = 4;

    private static int getObjectAddr(Object obj) {
        return 0;
    }

    private static int getParamByteCount(Method method) {
        int count = 0;
        Class<?>[] types = method.getParameterTypes();
        for (int i = 0; i < types.length; i++) {
            String type = types[i].toString();
            if (type.contains("long")) {
                count++;
            } else if (type.contains("double")) {
                count++;
            }
            count++;
        }
        if (Modifier.isStatic(method.getModifiers())) {
            count++;
        }
        return count * 4;
    }

    private static int[] getParamList(Method method) {
        Class<?>[] types = method.getParameterTypes();
        int index = 0;
        int[] result = new int[types.length];

        for (int i = 0; i < types.length; i++) {
            String type = types[i].toString();
            if (type.contains("int")) {
                result[index] = 8 + BASE_TOKEN;
            } else if (type.contains("short")) {
                result[index] = 7 + BASE_TOKEN;
            } else if (type.contains("float")) {
                result[index] = 6 + BASE_TOKEN;
            } else if (type.contains("boolean")) {
                result[index] = 5 + BASE_TOKEN;
            } else if (type.contains("char")) {
                result[index] = 4 + BASE_TOKEN;
            } else if (type.contains("byte")) {
                result[index] = 3 + BASE_TOKEN;
            } else if (type.contains("long")) {
                result[index] = 2 + BASE_TOKEN;
            } else if (type.contains("double")) {
                result[index] = 1 + BASE_TOKEN;
            } else { // Object
                result[index] = 0 + BASE_TOKEN;
            }
            index++;
        }
        return result;
    }

    private static int boxArgs(Object[] box, int index, int arg) {
        Log.d(TAG, "boxArgs: " + arg);
        box[index] = Integer.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, long arg) {
        Log.d(TAG, "boxArgs: " + arg);
        box[index] = Long.valueOf(arg);
        return 2;
    }

    private static int boxArgs(Object[] box, int index, byte arg) {
        box[index] = Byte.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, char arg) {
        Log.d(TAG, "boxArgs: " + arg);
        box[index] = Character.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, double arg) {
        box[index] = Double.valueOf(arg);
        return 2;
    }

    private static int boxArgs(Object[] box, int index, float arg) {
        box[index] = Float.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, Object arg) {
        Log.d(TAG, "boxArgs: " + arg);
        box[index] = arg;
        return 1;
    }

    private static int boxArgs(Object[] box, int index, short arg) {
        box[index] = Short.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, boolean arg) {
        box[index] = Boolean.valueOf(arg);
        return 1;
    }
}