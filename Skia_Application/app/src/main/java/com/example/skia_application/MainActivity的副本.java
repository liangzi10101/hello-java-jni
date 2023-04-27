//package com.example.skia_application;
//
//import androidx.appcompat.app.AppCompatActivity;
//
//import android.os.Bundle;
//import android.util.Log;
//import android.view.View;
//import android.widget.TextView;
//
//public class MainActivity extends AppCompatActivity {
//    private static void loadLibrary() {
//        try {
//            System.loadLibrary("native-lib");
//        } catch (Throwable e) {
//            Log.d("error", "加载库异常 ："+ e.toString());
//        }
//    }
//
//    static {
//        loadLibrary();
//    }
//
//    TextView textview_welcome;
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
//
//        // 使用findViewById得到textView对象
//        textview_welcome = (TextView)findViewById(R.id.liangzi);
//
//        //添加点击监听器
//        textview_welcome.setOnClickListener(new TextView.OnClickListener(){
//            @Override
//            public void onClick(View v) {
//                String ret = stringFromJNI("world");
//                //控制台打印
//                Log.i("jnitest:", ret + "");
//                //设置text
//                textview_welcome.setText(ret);
//            }
//        });
//    }
//
//    public native String stringFromJNI(String str);
//}