package com.example.nottificationapp.Screens

import android.util.Log
import android.view.ViewGroup
import android.webkit.WebSettings
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material.icons.filled.ArrowForward
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.example.nottificationapp.MqttController

@Composable
fun StreamScreen(){
    val context = LocalContext.current
    Column(modifier = Modifier.fillMaxSize()) {
        AndroidView(modifier = Modifier.fillMaxWidth().weight(1f).padding(12.dp), factory = {
            WebView(context).apply {
                layoutParams= ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT)
                webViewClient= WebViewClient()
                settings.apply {
                    javaScriptEnabled = false  // MJPEG doesn't need JS
                    domStorageEnabled = false
                    cacheMode = WebSettings.LOAD_NO_CACHE
                    useWideViewPort = true
                    loadWithOverviewMode = true
                    builtInZoomControls = false
                    displayZoomControls = false
                    mediaPlaybackRequiresUserGesture = false
                }
                loadUrl("http://192.168.0.36:8080/stream")
            }
        }, update = {
                webView ->
            if(webView.url!="http://192.168.0.36:8080/stream"){
                webView.loadUrl("http://192.168.0.36:8080/stream")
            }
        })
        Row(horizontalArrangement = Arrangement.Center, verticalAlignment = Alignment.CenterVertically){
          IconButton(onClick ={
              MqttController.publishData("pico/servo/left", onError = {
                  Log.e("Error",it.message.toString())
              })
          }
          ) {
              Icon(Icons.Default.ArrowBack, contentDescription = null)
          }
            IconButton(onClick ={
                MqttController.publishData("pico/servo/right", onError = {
                    Log.e("Error",it.message.toString())
                })
            }
            ) {
                Icon(Icons.Default.ArrowForward, contentDescription = null)
            }
        }

    }
}