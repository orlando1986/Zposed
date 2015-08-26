package com.catfish.zposed.ui;

import java.lang.reflect.Method;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

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
        victim(1, 2, 3, 4, 5);
    }

    private void victim(int a, int b, int c, int d, int e) {
        Log.d(TAG, "victim called");
    }

    private void hookVictim() {
        Method[] ms = getClass().getDeclaredMethods();
        for(Method m : ms) {
            if (m.getName().contains("victim")) {
                HookManager.hookMethod(m);
                return;
            }
        }
    }
}
