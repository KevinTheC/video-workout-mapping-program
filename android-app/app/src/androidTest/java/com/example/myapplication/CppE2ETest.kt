package com.example.myapplication

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class CppE2ETest {
    @Test
    fun testIncrementWorks() {
        val input = 5
        val expected = 6 // assuming your C++ adds 1
        val result = MainActivity.testIncrement(input)
        assertEquals(expected, result)
    }
}