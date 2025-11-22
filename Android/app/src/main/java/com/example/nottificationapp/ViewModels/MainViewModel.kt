package com.example.nottificationapp.ViewModels

import android.util.Log
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.lifecycle.ViewModel
import com.example.nottificationapp.Models.Event
import com.example.nottificationapp.MqttController
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.messaging.FirebaseMessaging

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

class MainViewModel: ViewModel() {
    var statusConnection by  mutableStateOf<String?>(null)
        private set
    var statusText by mutableStateOf<String?>(null)
        private set
    var events by  mutableStateOf<List<Event>>(emptyList())
        private set
    private val broker = "broker.emqx.io"
    init {
        FirebaseMessaging.getInstance().subscribeToTopic("alerts")
            .addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    Log.d("FCM", "Subscribed to topic 'alerts'")
                } else {
                    Log.e("FCM", "Subscription failed", task.exception)
                }
            }
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
        fetchIntrusionEvents {
            events=it
        }
    }
}