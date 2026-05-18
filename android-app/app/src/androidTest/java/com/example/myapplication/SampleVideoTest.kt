package com.example.myapplication

import android.graphics.Bitmap
import android.media.MediaMetadataRetriever
import android.util.Log
import androidx.test.platform.app.InstrumentationRegistry
import com.google.mediapipe.framework.image.BitmapImageBuilder
import com.google.mediapipe.tasks.core.BaseOptions
import com.google.mediapipe.tasks.vision.core.RunningMode
import com.google.mediapipe.tasks.vision.poselandmarker.PoseLandmarker
import java.nio.ByteBuffer
import java.nio.ByteOrder

class SampleVideoTest {
    companion object {
        init {
            try {
                System.loadLibrary("mediapipe_tasks_vision_jni")
            } catch (e: UnsatisfiedLinkError) {
                // Log it so you can see if it's still failing here
                println("Native library failed to load: $e")
            }
        }
        fun SampleVideoTestFunction(assetName: String, handler : SampleHandler){
            PhysicsAPI.Companion.CppLoggerBridge.initCppLogger()
            //use a predetermined clip to test
            val baseOptions = BaseOptions.builder()
                .setModelAssetPath("pose_landmarker_full.task")
                .build()

            val options = PoseLandmarker.PoseLandmarkerOptions.builder()
                .setBaseOptions(baseOptions)
                .setRunningMode(RunningMode.VIDEO)
                //in video mode, the landmarks are returned immediately because its a blocking function
                //livestream mode will drop frames
                .build()

            val poseLandmarker = PoseLandmarker.createFromOptions(InstrumentationRegistry.getInstrumentation().targetContext, options)

            val context = InstrumentationRegistry.getInstrumentation().context

            val retriever = MediaMetadataRetriever()
            try {//
                val assetFileDescriptor = context.assets.openFd(assetName)

                retriever.setDataSource(assetFileDescriptor.fileDescriptor, assetFileDescriptor.startOffset, assetFileDescriptor.length)

                // Now you can extract frames or metadata
                val duration = retriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION)
                Log.d("PhysicsTest", "Video duration: $duration ms")
                assetFileDescriptor.close()

                // Get the total duration of the video in microseconds
                val durationStr = retriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION)
                val durationMs = durationStr?.toLong() ?: 0L

                // 1000ms / 30fps = 33.33ms
                val frameIntervalMs = 17L
                PhysicsAPI.initializeBuffer(900)
                PhysicsAPI.registerListener { response: FrameUpdateResponse ->
                    handler.callPredicate(response)
                }
                for (timestampMs in 0 until durationMs step frameIntervalMs) {

                    // get bitmap
                    // Note: getFrameAtTime takes MICROseconds (ms * 1000)
                    val rawBitmap = retriever.getFrameAtTime(
                        timestampMs * 1000,
                        MediaMetadataRetriever.OPTION_CLOSEST_SYNC
                    )

                    // Convert to ARGB_8888 if it isn't already
                    val bitmap = rawBitmap?.copy(Bitmap.Config.ARGB_8888, false)
                        ?: throw IllegalStateException("Could not retrieve frame")

                    if (bitmap != null) {
                        // 2. Convert the Android/JVM Bitmap into a MediaPipe Image
                        val mpImage = BitmapImageBuilder(bitmap).build()

                        // 3. Submit to Landmarker (blocking version because we are submitting video not live_stream
                        val result = poseLandmarker.detectForVideo(mpImage, timestampMs).landmarks()
                        if (result.isEmpty()) continue
                        val floatList: List<Float> = result.flatMap { list ->
                            list.flatMap { landmark ->
                                listOf(landmark.x(), landmark.y(), landmark.z())
                            }
                        }

                        val frameSize = floatList.size * Float.SIZE_BYTES
                        val buffer = ByteBuffer.allocateDirect(frameSize)
                        buffer.order(ByteOrder.nativeOrder())
                        if (!buffer.isDirect) {
                            assert(false, {"Buffer MUST be direct!"})
                        }
                        if (buffer.capacity() == 0) {
                            assert(false, {"Buffer is empty!"})
                        }
                        for (f in floatList) {
                            buffer.putFloat(f)
                        }

                        PhysicsAPI.submitFrame(
                            buffer
                        )
                        assert(handler.getState().second, {handler.getState().first})
                    }
                }
                PhysicsAPI.shutdown()
            } catch (e: Exception) {
                assert(false, {e.stackTraceToString()})
            } finally {
                retriever.release()
            }
            handler.callFinally()
        }
    }
    class SampleHandler(
        val predicate : (SampleHandler, FrameUpdateResponse) -> Pair<String, Boolean>,
        val finally : (SampleHandler) -> Unit
    ){
        private val data: MutableList<FloatArray> = mutableListOf()
        private var state: Pair<String, Boolean> = "State OK" to true
        fun callPredicate(fur : FrameUpdateResponse) {
            //if predicate already failed, fail fast
            if (!state.second){
                return Unit
            }
            state = predicate(this, fur)
        }
        fun callFinally(){
            finally(this)
        }
        fun getState() : Pair<String, Boolean>{
            return state
        }
        fun getData() : MutableList<FloatArray>{
            return data
        }
    }
}