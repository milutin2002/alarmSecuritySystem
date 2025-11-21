package com.example.nottificationapp

import android.R
import android.os.Bundle
import android.util.Log
import android.view.ViewGroup
import android.webkit.WebSettings
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.LocationOn
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.navigation.NavController
import androidx.navigation.NavHost
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import coil.compose.AsyncImage
import com.example.nottificationapp.Models.Event

import com.example.nottificationapp.ui.theme.NottificationAppTheme
import com.google.firebase.Firebase
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.messaging.FirebaseMessaging
import io.reactivex.plugins.RxJavaPlugins




class MainActivity : ComponentActivity() {


    sealed class Route(val route: String, val label: String) {
        object Home : Route("home", "Home")
        object Stream : Route("stream", "Stream")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        RxJavaPlugins.setErrorHandler { e ->
            android.util.Log.e("RxJava", "Unhandled error", e)
        }
        setContent {
            NottificationAppTheme {
                val navController=rememberNavController()
                val tabs=listOf(Route.Home, Route.Stream)
                Scaffold(modifier = Modifier.fillMaxSize(),{appTopBar(navController)}, bottomBar = {appBottomBar(navController,tabs)}) { innerPadding ->
                    var statusConnection by remember { mutableStateOf<String?>(null) }
                    val broker =  "broker.emqx.io"
                    var statusText by remember { mutableStateOf<String?>(null) }
                    LaunchedEffect(Unit) {
                        MqttController.createClient(broker,brokerPort=8883)
                        MqttController.connect(onConnected = {
                            Log.i("Connection","Connection established")
                            statusConnection="Connected"
                            MqttController.subscribeStatus(onMessage = { data->
                                statusText=data
                            }, onError = {
                                statusConnection="Error during subscription"
                            })
                        }, onError = {
                            statusConnection="Error during connection"
                        })
                    }
                    //MainScreen(statusConnection=statusConnection.toString())
                    NavHost(navController=navController,startDestination= Route.Home.route,modifier= Modifier.padding(innerPadding)){
                        composable(Route.Home.route){
                            MainScreen(statusConnection=statusConnection.toString())
                        }
                        composable(Route.Stream.route) {
                            streamScreen()
                        }
                    }
                }
            }
        }
    }
}

@Composable
fun currentRoute(navController: NavController): String? {
    val entry by navController.currentBackStackEntryFlow.collectAsState(initial = navController.currentBackStackEntry)
    return entry?.destination?.route
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun appTopBar(navController:NavHostController){
    val route=currentRoute(navController)
    CenterAlignedTopAppBar(
        title = {
            Text( if ( route == MainActivity.Route.Home.route) "Home" else "Stream", style = MaterialTheme.typography.titleLarge)
        }
    )
}

@Composable
fun streamScreen(){
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

    }
}
@Composable
fun appBottomBar(navController: NavController,tabs:List<MainActivity.Route>){
    val route=currentRoute(navController)
    NavigationBar {
        tabs.forEach {
            tab->
            NavigationBarItem(selected = tab.route==route, onClick = {
                navController.navigate(tab.route){

                }
            }, icon = {
                Icon(if (tab == MainActivity.Route.Home) Icons.Default.Home else Icons.Default.LocationOn,
                    contentDescription = null)
            }, label = {Text(tab.label)})
        }
    }
}
fun fetchIntrusionEvents(onResult:(List<Event>)->Unit){
    val database=FirebaseDatabase.getInstance()
    val ref=database.getReference("events")
    ref.addListenerForSingleValueEvent(object :ValueEventListener{
        override fun onDataChange(snapshot: DataSnapshot) {
            val list= mutableListOf<Event>()
            for(child in snapshot.children){
                val event=child.getValue(Event::class.java)
                event?.let { list.add(it) }
            }
            onResult(list.reversed())
        }

        override fun onCancelled(error: DatabaseError) {
            Log.e("Error","Canceled")
        }

    })
}
@Composable
fun IntrusionList(events: List<Event>){
    LazyColumn {
        items(events) { event ->
            Card(
                modifier = Modifier
                    .padding(8.dp)
                    .fillMaxWidth()
                    .clickable {

                    }
            ) {
                Column(modifier = Modifier.padding(8.dp)) {
                    Text("Time: ${event.timestamp}")
                    AsyncImage(
                        model = event.image_url,
                        contentDescription = null,
                        modifier = Modifier.height(200.dp)
                    )
                }
            }
        }
    }
}
@Composable
fun MainScreen(modifier: Modifier = Modifier,statusConnection: String){
    FirebaseMessaging.getInstance().subscribeToTopic("alerts")
        .addOnCompleteListener { task ->
            if (task.isSuccessful) {
                Log.d("FCM", "Subscribed to topic 'alerts'")
            } else {
                Log.e("FCM", "Subscription failed", task.exception)
            }
        }
    var events by remember { mutableStateOf<List<Event>>(emptyList()) }
    var status by remember { mutableStateOf<Boolean>(false) }
    var statusText by remember { mutableStateOf<String?>(null) }
    LaunchedEffect(Unit) {

        fetchIntrusionEvents {
            events=it
        }
    }
    Column(Modifier.padding(20.dp)) {
        Text(text = "Connection status: $statusConnection")
        Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.spacedBy(8.dp)){
            Button(onClick = {
                MqttController.publishData{
                }
            },modifier= Modifier.weight(1f)) {
                Text(text = "Turn on/off")
            }
        }
        Text(text = "Security status: $statusText")
        IntrusionList(events)
    }
}
