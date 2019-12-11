package com.skymunge.flappybird;

import android.app.NativeActivity;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import org.fmod.FMOD;

public class DummyActivity extends Activity {
    static {
        System.loadLibrary("fmod");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = new Intent(this, NativeActivity.class);
        startActivity(intent);

        //FMOD.init(this);
    }
}
