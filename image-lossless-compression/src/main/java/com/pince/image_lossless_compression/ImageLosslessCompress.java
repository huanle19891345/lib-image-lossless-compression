package com.pince.image_lossless_compression;

import android.graphics.Bitmap;
import android.os.Environment;

import java.io.File;

/**
 * Created by hp on 2017/8/4.
 */

public class ImageLosslessCompress {

    static {
        System.loadLibrary("jpegbither");
        System.loadLibrary("com_baseproject_ndk_ImageLosslessCompress");
    }

    public static String outFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "mytaobao_new.jpeg";

    private static int DETAULT_QUALITY = 20;

    //使用demo
//    Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.splash);
//        ImageLosslessCompress.compress(bitmap, ImageLosslessCompress.outFilePath, true);

    public static String compress(Bitmap bitmap, String filePath, boolean optimize) {
        return compressImage(bitmap, bitmap.getWidth(), bitmap.getHeight(), DETAULT_QUALITY, filePath.getBytes(), optimize);
    }

    public static native String compressImage(Bitmap bitmap, int w, int h, int quality, byte[] filePath, boolean optimize);
}
