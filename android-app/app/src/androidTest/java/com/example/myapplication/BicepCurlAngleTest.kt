package com.example.myapplication

import android.graphics.Bitmap
import android.util.Log
import android.media.MediaMetadataRetriever
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import com.google.mediapipe.framework.image.BitmapImageBuilder
import com.google.mediapipe.tasks.core.BaseOptions
import com.google.mediapipe.tasks.vision.core.RunningMode
import com.google.mediapipe.tasks.vision.poselandmarker.PoseLandmarker
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith
import java.nio.ByteBuffer
import java.nio.ByteOrder
import kotlin.collections.contentToString
import kotlin.text.get

@RunWith(AndroidJUnit4::class)
class BicepCurlAngleTest {
    @Test
    fun testBicepCurlAngle() {
        SampleVideoTest.Companion.SampleVideoTestFunction("wristsupination1.mp4",
            SampleVideoTest.SampleHandler({x,y->
                x.getData().add(floatArrayOf(y.extra[3], y.extra[4], y.extra[6], y.extra[7], y.extra[9], y.extra[10]))
                "State OK" to true
                                          }, {x->
                                              Log.d("BicepCurlTest", x.getData().joinToString(
                                                  separator = ",",
                                                  prefix = "[",
                                                  postfix = "]"
                                              ) { array ->
                                                  array.slice(0..1).joinToString(separator = ",", prefix = "[", postfix = "]")
                                              })
                Log.d("BicepCurlTest", x.getData().joinToString(
                    separator = ",",
                    prefix = "[",
                    postfix = "]"
                ) { array ->
                    array.slice(2..3).joinToString(separator = ",", prefix = "[", postfix = "]")
                })
                Log.d("BicepCurlTest", x.getData().joinToString(
                    separator = ",",
                    prefix = "[",
                    postfix = "]"
                ) { array ->
                    array.slice(4..5).joinToString(separator = ",", prefix = "[", postfix = "]")
                })
            })
        )
    }
}