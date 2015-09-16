package com.catfish.zposed.ui;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.catfish.zposed.HookCallback;
import com.catfish.zposed.HookManager;
import com.catfish.zposed.R;

public class MainActivity extends Activity {
    private static final String TAG = "catfish";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        hookVictim();
    }

    public void onClick(View view) {
        Log.d(TAG, "get long: " + victim(1, 1234567890987654l, 'x'));
    }

    private long victim(int a, long b, char c) {
        Log.d(TAG, "victim called: a=" + a + ", b=" + b + ", c=" + c);
        return b;
    }

    private void hookVictim() {
        Method[] ms = getClass().getDeclaredMethods();
        for (Method m : ms) {
            if (m.getName().contains("victim")) {
                HookManager.hookMethod(m, new HookCallback() {
                    @Override
                    public Object onHook(Method method, Object receiver, Object[] args) {
                        Log.d(TAG, "onHooked");
                        try {
                            method.setAccessible(true);
                            return method.invoke(receiver, args);
                        } catch (IllegalAccessException e) {
                            e.printStackTrace();
                        } catch (IllegalArgumentException e) {
                            e.printStackTrace();
                        } catch (InvocationTargetException e) {
                            e.printStackTrace();
                        }
                        return null;
                    }
                });
                return;
            }
        }
    }
}
