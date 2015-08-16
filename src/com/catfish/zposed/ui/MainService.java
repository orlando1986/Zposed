package com.catfish.zposed.ui;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Application;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import com.catfish.zposed.HookManager;

public class MainService extends Service {
    private static final String TAG = "catfish";
    private Application mApp = null;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        try {
            Class<?> activityThreadClass = Class.forName("android.app.ActivityThread");
            Method currentActivityThread = activityThreadClass.getDeclaredMethod("currentActivityThread",
                    (Class[]) null);
            currentActivityThread.setAccessible(true);
            Object activityThread = currentActivityThread.invoke(null, (Object[]) null);

            Field[] fs = activityThreadClass.getDeclaredFields();
            for (Field f : fs) {
                if (f.getType().getName().equals("android.app.Application")) {
                    f.setAccessible(true);
                    mApp = (Application) f.get(activityThread);
                    break;
                }
            }
        } catch (ClassNotFoundException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            Log.e(TAG, e.toString());
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.toString());
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (mApp != null) {
            hookVictim(mApp);
        } else {
            Log.e(TAG, "didn't find application");
        }
        return super.onStartCommand(intent, flags, startId);
    }

    private void hookVictim(Application app) {
        if (app == null) {
            return;
        }
        try {
            Class<?> activityclass = app.getClass().getClassLoader().loadClass("com.example.test.MainActivity");
            Method victim = activityclass.getDeclaredMethod("victim", (Class[]) null);

            Class<?> zposedactivityclass = Class.forName("com.catfish.zposed.ui.MainActivity");
            Method proxy = zposedactivityclass.getDeclaredMethod("proxy", (Class[]) null);
            HookManager.hookZposedMethod(victim, proxy);
            Log.i(TAG, "hook method done");
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        } catch (ClassNotFoundException e) {
            Log.e(TAG, e.toString());
        }
    }
}
