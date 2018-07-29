package com.pince.image.losslesscompressiondemo

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.pince.image_lossless_compression.ImageLosslessCompress
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher);
        val result = ImageLosslessCompress.compress(bitmap, ImageLosslessCompress.outFilePath, true);
         Log.d(TAG, "result = " + result)
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        val TAG = this::class.java.simpleName

//        // Used to load the 'native-lib' library on application startup.
//        init {
//            System.loadLibrary("native-lib")
//        }
    }
}
