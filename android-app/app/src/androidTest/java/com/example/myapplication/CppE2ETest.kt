package com.example.myapplication

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith
import java.nio.ByteBuffer

@RunWith(AndroidJUnit4::class)
class CppE2ETest {
    @Test
    fun testIncrementWorks() {
        val input = 5
        val expected = 7
        val result = PhysicsAPI.testIncrement(input)
        assertEquals(expected, result)
    }
    @Test
    fun testCallingMethods() {
        try {
            PhysicsAPI.initializeBuffer(1);

            val frameSize = 4 * 4 * 3 // pretend RGB
            val buffer = ByteBuffer.allocateDirect(frameSize)
            for (i in 0 until frameSize) {
                buffer.put(i, (i % 256).toByte())
            }

            // 3. Call submitFrame
            PhysicsAPI.submitFrame(
                buffer,
                4,
                4,
                1,
            )
            PhysicsAPI.registerListener()
            PhysicsAPI.shutdown()
        } catch (e : Exception) {
            assert(false, { "Exception thrown while calling some PhysicsAPI method" })
        }
    }
}