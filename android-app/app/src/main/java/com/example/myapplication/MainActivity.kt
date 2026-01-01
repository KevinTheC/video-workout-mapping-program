package com.example.myapplication

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.myapplication.ui.theme.MyApplicationTheme

class MainActivity : ComponentActivity() {

    companion object {
        init {
            System.loadLibrary("android_wrapper_cpp")
        }
        @JvmStatic
        external fun testIncrement(value: Int): Int
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)   // MUST be inside onCreate

        setContent {
            MyApplicationTheme {
                // Just for testing
                try {
                    val result = testIncrement(5)
                } catch (e: UnsatisfiedLinkError) {
                    Log.e("JNI", "Linking failed: ${e.message}")
                }
                val result = testIncrement(5)   // call JNI
                Text(text = "5 + 1 = $result")
            }
        }
    }
}
@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    MyApplicationTheme {
        Greeting("Android")
    }
}