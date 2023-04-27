package com.example.skia_application;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class SkiaView extends SurfaceView implements SurfaceHolder.Callback2 {
    private SurfaceHolder mHolder;
    private HandlerThread mHandlerThread;
    private Handler mHandler;
    private static final int DRAW = 1;

    public SkiaView(Context context) {
        this(context,null);
    }

    public SkiaView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public SkiaView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        _init();
    }

    private class SkiaHandler extends Handler {
        public SkiaHandler(Looper looper){
            super(looper);
        }

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case DRAW:
                    synchronized (SkiaView.class){
                        SkiaJNI.native_render((Surface) msg.obj, msg.arg1, msg.arg2);
                    }
                    break;
            }
        }
    }

    private void _init(){
        mHandlerThread = new HandlerThread("Skia");
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHandlerThread.start();
        mHandler = new SkiaHandler(mHandlerThread.getLooper());
    }

    @Override
    public void surfaceRedrawNeeded(@NonNull SurfaceHolder surfaceHolder) {
        // todo
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        Message message = new Message();
        message.what = DRAW;
        message.obj = surfaceHolder.getSurface();
        message.arg1 = getWidth();
        message.arg2 = getHeight();
        mHandler.sendMessage(message);
        Log.e("create","width:" + getWidth());
        Log.e("create","height" + getHeight());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        Message message = new Message();
        message.what = DRAW;
        message.obj = surfaceHolder.getSurface();
        message.arg1 = getWidth();
        message.arg2 = getHeight();
        mHandler.sendMessage(message);
        Log.e("change","width:" + getWidth());
        Log.e("change","height" + getHeight());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        mHandlerThread.quit();
    }
}
