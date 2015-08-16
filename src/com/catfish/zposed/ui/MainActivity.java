package com.catfish.zposed.ui;

import java.lang.reflect.Method;

import android.app.Activity;
import android.content.Intent;
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
        victim();
//        Intent it = new Intent(this, MainService.class);
//        startService(it);
    }
    
    private void victim() {
        Log.d(TAG, "victim called");
    }

    private void proxy() {
        Log.d(TAG, "proxy called");
    }

    private void hookVictim() {
        try {
            Method proxy = getClass().getDeclaredMethod("proxy", (Class[]) null);
            Method victim = getClass().getDeclaredMethod("victim", (Class[]) null);
            HookManager.hookZposedMethod(victim, proxy);
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.toString());
        }
    }
}
