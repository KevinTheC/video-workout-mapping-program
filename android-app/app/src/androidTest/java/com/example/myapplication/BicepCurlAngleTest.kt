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
import kotlin.math.PI
import kotlin.text.get

@RunWith(AndroidJUnit4::class)
class BicepCurlAngleTest {
    @Test
    fun testBicepCurlAngle() {
        SampleVideoTest.sampleVideoTestFunction("bicepcurl.mp4",
            SampleVideoTest.SampleHandler({x->
                PhysicsAPI.nativeGetBatchState(intArrayOf(
                    BodyState.getIndexOf(BodyState.Side.left,
                        BodyState.Joint.elbow,
                        BodyState.Plane.flexion),
                    BodyState.getIndexOf(BodyState.Side.right,
                        BodyState.Joint.elbow,
                        BodyState.Plane.flexion)),
                    BodyState.values)

                x.getData().add(floatArrayOf(
                    BodyState.getPlanarAngle(BodyState.Side.left,
                        BodyState.Joint.elbow,
                        BodyState.Plane.flexion),
                    BodyState.getPlanarAngle(BodyState.Side.right,
                        BodyState.Joint.elbow,
                        BodyState.Plane.flexion)
                ))
                "State OK" to true
            }, {x-> //now we go through, make sure both sides have a min-max difference of at least 70 degrees
                //i should really check to make sure delta isn't greater than 180
                //AI: i copide this straight from gemini, did test it though
                val data = x.getData()


                var unwrappedLeft = 0.0f
                var unwrappedRight = 0.0f

                var leftMin = 9999.0f
                var leftMax = -9999.0f
                var rightMin = 9999.0f
                var rightMax = -9999.0f

                var prevLeftRaw = data.firstOrNull()?.get(0) ?: 0.0f
                var prevRightRaw = data.firstOrNull()?.get(1) ?: 0.0f

                val PI_F = PI.toFloat()
                val TWO_PI_F = (2.0 * PI).toFloat()
                unwrappedLeft = prevLeftRaw
                unwrappedRight = prevRightRaw

                for (entry in data) {
                    val currentLeftRaw = entry[0]
                    val currentRightRaw = entry[1]

                    var diffLeft = currentLeftRaw - prevLeftRaw
                    while (diffLeft < -PI_F) diffLeft += TWO_PI_F
                    while (diffLeft > PI_F)  diffLeft -= TWO_PI_F

                    unwrappedLeft += diffLeft
                    prevLeftRaw = currentLeftRaw

                    var diffRight = currentRightRaw - prevRightRaw
                    while (diffRight < -PI_F) diffRight += TWO_PI_F
                    while (diffRight > PI_F)  diffRight -= TWO_PI_F

                    unwrappedRight += diffRight
                    prevRightRaw = currentRightRaw

                    if (unwrappedLeft < leftMin) leftMin = unwrappedLeft
                    if (unwrappedLeft > leftMax) leftMax = unwrappedLeft

                    if (unwrappedRight < rightMin) rightMin = unwrappedRight
                    if (unwrappedRight > rightMax) rightMax = unwrappedRight
                }

                // Now you can safely check your 70-degree range gap!
                val leftRange = leftMax - leftMin
                val rightRange = rightMax - rightMin
                if ((leftMax - leftMin > (70.0f * PI) / 180.0f) and (rightMax - rightMin > (70.0f * PI) / 180.0f)) {
                    "State OK" to true
                }
                else {
                    "Model could not capture at least 70 degrees of motion in flexion along the elbow" +
                            "LMax = $leftMax LMin = $leftMin RMax = $rightMax RMin = $rightMin" to false
                }
            })
        )
    }
}