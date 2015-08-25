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
        victim(new Activity(), new Object());
    }
    
    private void victim(Object a, Object b) {
        Log.d(TAG, "victim called");
    }

    private void hookVictim() {
        try {
            Method victim = getClass().getDeclaredMethod("victim", Object.class, Object.class);
            HookManager.hookMethod(victim);
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        }
    }
}
