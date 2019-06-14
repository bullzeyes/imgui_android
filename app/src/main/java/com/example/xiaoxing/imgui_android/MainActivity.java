package com.example.xiaoxing.imgui_android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    private MainGLView glView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);
        glView = findViewById(R.id.myGLView);
        glView.initPostCreate();
        //glView = new MainGLView(this, getApplication());
        //setContentView(glView);
    }

    @Override protected void onPause() {
        super.onPause();
        glView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        glView.onResume();
    }

    @Override protected void onDestroy() {
        super.onDestroy();
        glView.onDestroy();
    }
}
