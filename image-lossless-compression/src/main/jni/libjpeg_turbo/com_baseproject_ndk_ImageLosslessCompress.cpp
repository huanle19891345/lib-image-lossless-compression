#include <jni.h>
#include <android/bitmap.h>
#include "com_baseproject_ndk_ImageLosslessCompress.h"
#include "libjpeg/jinclude.h"
#include <string.h>

#include <setjmp.h>
extern "C" {
#include "libjpeg/jpeglib.h"
}

#include <android/log.h>


#ifndef LOG_TAG

#define LOG_TAG "ANDROID_LAB"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#endif


struct my_error_mgr {
    struct jpeg_error_mgr pub;  /* "public" fields */

    jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

char *error;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo) {
/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

/* Always display the message. */
/* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

    error = (char*) myerr->pub.jpeg_message_table[myerr->pub.msg_code];

    LOGE("jpeg_meeage_table[&d]: %s", myerr->pub.msg_code, error);

/* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

typedef uint8_t BYTE;

int generateJPEG(BYTE *tmpdata, jint w, jint h, jint quality, char *path, jboolean optimize);

jstring JNICALL Java_com_pince_image_1lossless_1compression_ImageLosslessCompress_compressImage
  (JNIEnv * env, jclass jclz, jobject bitmap, jint w, jint h, jint quality, jbyteArray filePathBytes, jboolean optimize) {

    //解析bitmap的像素，生成字节数组
    BYTE *pixelsColor;//byte array
    AndroidBitmap_lockPixels(env, bitmap, (void**)&pixelsColor);
    BYTE *data;//data数组，用来存3原色
    data = (BYTE*)malloc(w * h * 3);
    BYTE *tmpdata;//缓存数组
    tmpdata = data;

    int i = 0, j = 0;
    BYTE r,g,b;
    int color;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            color = *(int*) pixelsColor;
            r = (color&0x00FF0000)>>16;
            g = (color&0x0000FF00)>>8;
            b = (color&0x000000FF);
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data = data + 3;
            pixelsColor+=4;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);

    //获取输出图片路径的char指针
    char *filePath = jstringToChar(env, filePathBytes);
    //图片数据交给jpeg库进行哈夫曼重新编码，生成新的压缩图片
    int resultCode = generateJPEG(tmpdata, w, h, quality, filePath, optimize);
    if (resultCode == 0) {
        jstring  result = env->NewStringUTF("压缩失败");
        return result;
    }
    return env->NewStringUTF("压缩成功");
}

int generateJPEG(BYTE *tmpdata, jint w, jint h, jint quality, char *path, jboolean optimize) {
    struct jpeg_compress_struct jcs;
    //error处理
    struct my_error_mgr jem;
    jcs.err = jpeg_std_error(&jem.pub);
    jem.pub.error_exit = my_error_exit;
    if(setjmp(jem.setjmp_buffer)) {
        return 0;
    }

    jpeg_create_compress(&jcs);//初始化
    //打开文件
    FILE *f = fopen(path , "wb");
    if(f == NULL) {
        return 0;
    }
    //设置jpeg的输出目录
    jpeg_stdio_dest(&jcs, f);
    jcs.image_width = w;
    jcs.image_height = h;
    jcs.arith_code = false;
    int mComponent = 3;//原色数量
    jcs.input_components = mComponent;
    jcs.in_color_space = JCS_RGB;
    jpeg_set_defaults(&jcs);
    jcs.optimize_coding = optimize;//优化压缩的boolean值
    jpeg_set_quality(&jcs, quality, true);
    //开始压缩
    jpeg_start_compress(&jcs, true);
    JSAMPROW row_pointer[1];//行标
    int row_stride;
    row_stride = jcs.image_width * mComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = (JSAMPROW)&tmpdata[jcs.next_scanline*row_stride];
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);
    fclose(f);
    return 0;

}

char *jstringToChar(JNIEnv *pEnv, jbyteArray pArray) {
    char *rtn = NULL;
    jsize alen = pEnv->GetArrayLength(pArray);
    jbyte *ba = pEnv->GetByteArrayElements(pArray, 0);
    if(alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    pEnv->ReleaseByteArrayElements(pArray, ba, 0);
    return rtn;
}

