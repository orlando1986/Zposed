package com.catfish.zposed;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
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

    private static final int BASE = 13;

    private static Class<?> returnTypeForArt(Object artmethod) {
        try {
            Method m = artmethod.getClass().getDeclaredMethod("getReturnType", (Class[]) null);
            m.setAccessible(true);
            return (Class<?>)m.invoke(artmethod);
        } catch (NoSuchMethodException e) {
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

    private static int getReturnType(Object method) {
        Class<?> type_class = returnTypeForArt(method);
        String type = type_class.toString();
        if (type.contains("int")) {
            return 8 + BASE;
        } else if (type.contains("short")) {
            return 7 + BASE;
        } else if (type.contains("float")) {
            return 6 + BASE;
        } else if (type.contains("boolean")) {
            return 5 + BASE;
        } else if (type.contains("char")) {
            return 4 + BASE;
        } else if (type.contains("byte")) {
            return 3 + BASE;
        } else if (type.contains("long")) {
            return 2 + BASE;
        } else if (type.contains("double")) {
            return 1 + BASE;
        } else { // Object
            return 0 + BASE;
        }
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

    private static int onHookInt(Object artmethod, Object receiver, Object[] args) {
        return (Integer) HookManager.onHooked(artmethod, receiver, args);
    }

    private static long onHookLong(Object artmethod, Object receiver, Object[] args) {
        return (Long) HookManager.onHooked(artmethod, receiver, args);
    }

    private static double onHookDouble(Object artmethod, Object receiver, Object[] args) {
        return (Double) HookManager.onHooked(artmethod, receiver, args);
    }

    private static char onHookChar(Object artmethod, Object receiver, Object[] args) {
        return (Character) HookManager.onHooked(artmethod, receiver, args);
    }

    private static short onHookShort(Object artmethod, Object receiver, Object[] args) {
        return (Short) HookManager.onHooked(artmethod, receiver, args);
    }

    private static float onHookFloat(Object artmethod, Object receiver, Object[] args) {
        return (Float) HookManager.onHooked(artmethod, receiver, args);
    }

    private static Object onHookObject(Object artmethod, Object receiver, Object[] args) {
        return HookManager.onHooked(artmethod, receiver, args);
    }

    private static boolean onHookBoolean(Object artmethod, Object receiver, Object[] args) {
        return (Boolean) HookManager.onHooked(artmethod, receiver, args);
    }

    private static byte onHookByte(Object artmethod, Object receiver, Object[] args) {
        return (Byte) HookManager.onHooked(artmethod, receiver, args);
    }
}