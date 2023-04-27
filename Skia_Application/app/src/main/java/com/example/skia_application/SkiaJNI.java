package com.example.skia_application;

import android.graphics.Bitmap;
import android.view.Surface;

public class SkiaJNI {

    static {
        System.loadLibrary("skia");
        System.loadLibrary("native-lib");
    }

    public static native void native_renderCanvas(Bitmap bitmap);

    public static native void native_render(Surface surface, int width, int height);
}
