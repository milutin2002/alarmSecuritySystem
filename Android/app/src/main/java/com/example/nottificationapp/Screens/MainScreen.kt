package com.example.nottificationapp.Screens

import android.util.Log
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import coil.compose.AsyncImage
import com.example.nottificationapp.Models.Event
import com.example.nottificationapp.MqttController
import com.example.nottificationapp.ViewModels.MainViewModel
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener


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
fun MainScreen(modifier: Modifier = Modifier,viewModel: MainViewModel = viewModel()){

    val broker =  "broker.emqx.io"
    val statusConnection = viewModel.statusConnection
    val statusText = viewModel.statusText
    val events = viewModel.events

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