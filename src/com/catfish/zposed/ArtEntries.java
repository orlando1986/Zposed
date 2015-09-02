package com.catfish.zposed;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import android.util.Log;

class ArtEntries {
    private final static String TAG = "catfish";

    private static int getObjectAddr(Object obj) {
        return 0;
    }

    private static int[] getParamList(Method method) {
        Class<?>[] types = method.getParameterTypes();
        int index = 0;
        int[] result = new int[types.length];

        for (int i = 0; i < types.length; i++) {
            String type = types[i].toString();
            if (type.contains("int")) {
                result[index] = 8;
            } else if (type.contains("short")) {
                result[index] = 7;
            } else if (type.contains("float")) {
                result[index] = 6;
            } else if (type.contains("boolean")) {
                result[index] = 5;
            } else if (type.contains("char")) {
                result[index] = 4;
            } else if (type.contains("byte")) {
                result[index] = 3;
            } else if (type.contains("long")) {
                result[index] = 2;
            } else if (type.contains("double")) {
                result[index] = 1;
            } else { // Object
                result[index] = 0;
            }
            index++;
        }
        return result;
    }

    private static int boxArgs(Object[] box, int index, int arg) {
        box[index] = Integer.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, long arg) {
        box[index] = Long.valueOf(arg);
        return 2;
    }

    private static int boxArgs(Object[] box, int index, byte arg) {
        box[index] = Byte.valueOf(arg);
        return 1;
    }

    private static int boxArgs(Object[] box, int index, char arg) {
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