//
// Created by 梁子 on 4/19/23.
//

#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>

// 引入的自定义静态库的头文件
#include "math_add.h"
// 引入skia头文件
#include "include/core/SkBitmap.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRect.h"
#include "include/core/SkColor.h"
#include "include/core/SkGraphics.h"

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

/*
 * Class:     com_example_skia_application_MainActivity
 * Method:    stringFromJNI
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
extern "C" JNIEXPORT jstring JNICALL Java_com_example_skia_1application_MainActivity_stringFromJNI
        (JNIEnv *env, jobject jobj, jstring str) {
    const char* c_name = env->GetStringUTFChars(str, NULL);

    std::string hello = "Hello from C++, ";
    std::string name(c_name);

    int res = MathAdd::add(1, 3);
    auto tt = hello + name + "：add=" + std::to_string(res);

    return env->NewStringUTF((tt).c_str());
}

// skia相关导出函数
extern "C"
JNIEXPORT void JNICALL
native_render(JNIEnv *env, jobject thiz, jobject jSurface,jint width,jint height){
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env,jSurface);

    ANativeWindow_setBuffersGeometry(nativeWindow,  width, height, WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer *buffer = new ANativeWindow_Buffer();

    ANativeWindow_lock(nativeWindow,buffer,0);

    int bpr = buffer->stride * 4;
    SkBitmap bitmap;
    SkImageInfo image_info = SkImageInfo
    ::MakeS32(buffer->width, buffer->height,SkAlphaType::kPremul_SkAlphaType);

    bitmap.setInfo(image_info, bpr);
    bitmap.setPixels(buffer->bits);

    SkCanvas *background = new SkCanvas(bitmap);
    SkPaint paint;

    paint.setColor(SK_ColorBLUE);
    SkRect rect;
    rect.set(SkIRect::MakeWH(width,height));

    background->drawRect(rect,paint);

    SkPaint paint2;
    paint2.setColor(SK_ColorWHITE);
    const char *str = "Hello Surface Skia";

    SkFont skfont(SkTypeface::MakeDefault(),100);

    background->drawString(str,100,100,skfont,paint2);

    SkImageInfo imageInfo = background->imageInfo();


    LOGE("row size:%d,buffer stride:%d",imageInfo.minRowBytes(),bpr);

    LOGE("before native_window stride:%d,width:%d,height:%d,format:%d",
         buffer->stride,buffer->width,buffer->height,buffer->format);

    LOGE("after native_window stride:%d,width:%d,height:%d,format:%d",
         buffer->stride,buffer->width,buffer->height,buffer->format);

    ANativeWindow_unlockAndPost(nativeWindow);
}

extern "C"
JNIEXPORT void JNICALL
native_renderCanvas(JNIEnv *env, jobject thiz, jobject bitmap) {
    // TODO: implement native_renderCanvas()
    LOGE("native render");

    AndroidBitmapInfo info;
    int *pixel;
    int ret;

    ret = AndroidBitmap_getInfo(env,bitmap,&info);
    ret = AndroidBitmap_lockPixels(env,bitmap,(void**)&pixel);

    int width = info.width;
    int height = info.height;

    SkBitmap bm = SkBitmap();
    SkImageInfo image_info = SkImageInfo
    ::MakeS32(width,height,SkAlphaType::kOpaque_SkAlphaType);
    bm.setInfo(image_info,image_info.minRowBytes());
    bm.setPixels(pixel);

    SkCanvas background(bm);
    SkPaint paint;

    paint.setColor(SK_ColorBLACK);
    SkRect rect;
    rect.set(SkIRect::MakeWH(width,height));

    background.drawRect(rect,paint);

    SkPaint paint2;
    paint2.setColor(SK_ColorBLUE);
    const char *str = "Hello Skia";

    SkFont skfont(SkTypeface::MakeDefault(),100);

    LOGE("row size:%d",image_info.minRowBytes());

    background.drawString(str,100,100,skfont,paint2);

    AndroidBitmap_unlockPixels(env,bitmap);
}


static const char* const className = "com/example/skia_application/SkiaJNI";
static const JNINativeMethod gMethods[] = {
        {"native_renderCanvas","(Landroid/graphics/Bitmap;)V", (void *)native_renderCanvas},
        {"native_render","(Landroid/view/Surface;II)V", (void *)native_render}
};


jint JNI_OnLoad(JavaVM *vm, void* reserved){
    JNIEnv *env = NULL;
    jint result;

    if(vm->GetEnv((void**)&env,JNI_VERSION_1_4)!=JNI_OK){
        return -1;
    }

    jclass clazz = env->FindClass(className);
    if(!clazz){
        LOGE("can not find class");
        return -1;
    }

    if(env->RegisterNatives(clazz,gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0){
        LOGE("can not register method");
        return -1;
    }

    return JNI_VERSION_1_4;
}