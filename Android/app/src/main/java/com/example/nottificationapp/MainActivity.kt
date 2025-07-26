package com.example.nottificationapp

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Card
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import coil.compose.AsyncImage
import com.example.nottificationapp.Models.Event

import com.example.nottificationapp.ui.theme.NottificationAppTheme
import com.google.firebase.Firebase
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener


class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            NottificationAppTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    MainScreen(

                    )
                }
            }
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
                        // Navigate to full image view
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
fun MainScreen(modifier: Modifier = Modifier){
    var events by remember { mutableStateOf<List<Event>>(emptyList()) }
    LaunchedEffect(Unit) {
        fetchIntrusionEvents {
            events=it
        }
    }
    IntrusionList(events)

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
    NottificationAppTheme {
        Greeting("Android")
    }
}