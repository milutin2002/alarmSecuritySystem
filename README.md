<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>alarmSecuritySystem</title>
  <style>
    body {
      font-family: system-ui, sans-serif;
      line-height: 1.6;
      padding: 2rem;
      max-width: 900px;
      margin: auto;
    }
    code {
      background: #f4f4f4;
      padding: 2px 4px;
      border-radius: 4px;
      font-family: monospace;
    }
    pre {
      background: #f4f4f4;
      padding: 1rem;
      border-radius: 4px;
      overflow-x: auto;
    }
    table {
      border-collapse: collapse;
      width: 100%;
      margin-bottom: 1rem;
    }
    th, td {
      padding: 0.5rem;
      border: 1px solid #ddd;
      text-align: left;
    }
    h1, h2, h3 {
      margin-top: 2rem;
    }
  </style>
</head>
<body>

<h1>🔐 alarmSecuritySystem</h1>
<p>
  A smart motion-detection security system using Raspberry Pi Pico (C SDK), Raspberry Pi (Python), Firebase, and an Android app (Jetpack Compose). The system captures and sends real-time images when motion is detected.
</p>

<h2>🚀 Features</h2>
<ul>
  <li>🔌 PIR Motion Detection with Raspberry Pi Pico (C SDK)</li>
  <li>📷 Raspberry Pi Camera Integration using OpenCV</li>
  <li>☁️ Image Upload to Firebase Storage</li>
  <li>🕒 Event Logging to Firebase Realtime Database</li>
  <li>📲 Android App with Jetpack Compose</li>
  <li>🔔 Push Notifications using Firebase Cloud Messaging (FCM)</li>
  <li>🔄 Communication between Pico and Pi via GPIO</li>
</ul>

<h2>🧠 Architecture Overview</h2>
<pre>
[ PIR Sensor ] --> [ Pico (C SDK) ]
         |                  |
         |        GPIO Signal (3.3V)
         ↓                  ↓
   [ LED + Buzzer ]     [ Raspberry Pi (Python) ]
                             |
                        Captures Image
                        Uploads to Firebase
                        Sends FCM Notification
                             ↓
                      [ Android App (Compose) ]
                      - Receives push alert
                      - Views image and timestamp
</pre>

<h2>🛠️ Tech Stack</h2>
<table>
  <tr><th>Component</th><th>Tech/Platform</th></tr>
  <tr><td>Microcontroller</td><td>Raspberry Pi Pico (C SDK)</td></tr>
  <tr><td>Central Processing</td><td>Raspberry Pi (Python 3)</td></tr>
  <tr><td>Camera</td><td>Pi Camera / OpenCV</td></tr>
  <tr><td>Cloud</td><td>Firebase Storage + Realtime DB</td></tr>
  <tr><td>Notification</td><td>Firebase Cloud Messaging (FCM)</td></tr>
  <tr><td>Mobile App</td><td>Android (Jetpack Compose, Kotlin)</td></tr>
</table>

<h2>📦 Setup Instructions</h2>

<h3>🔌 1. Raspberry Pi Pico (C SDK)</h3>
<ul>
  <li>Clone project and build with CMake</li>
  <li>Use GPIO17 for PIR, GPIO13 for LED, GPIO15 for buzzer, GPIO1 for signal to Pi</li>
</ul>

<h3>🐍 2. Raspberry Pi (Python)</h3>
<pre><code>pip install -r requirements.txt</code></pre>

<p>Create <code>.env</code> file:</p>
<pre><code>
FIREBASE_CRED_PATH=your_service_account.json
FIREBASE_BUCKET=your-bucket-name.appspot.com
FIREBASE_DB=https://your-db.firebaseio.com
API_KEY=your_fcm_key (if needed)
</code></pre>

<p>Run:</p>
<pre><code>python main.py</code></pre>

<h3>📱 3. Android App (Jetpack Compose)</h3>
<ul>
  <li>Connect to Firebase</li>
  <li>Subscribe to <code>alerts</code> topic for FCM</li>
  <li>Retrieve events from Realtime Database</li>
  <li>Show captured images via <code>AsyncImage</code></li>
</ul>

<h2>🧪 Testing</h2>
<ol>
  <li>Move in front of the PIR sensor</li>
  <li>Pico sends GPIO high to Pi</li>
  <li>Pi captures and uploads image</li>
  <li>Notification is sent to phone</li>
  <li>Tap notification to see event</li>
</ol>

<h2>📌 Future Work</h2>
<ul>
  <li>[ ] Add RTOS to Pico for multitasking</li>
  <li>[ ] Integrate SPI camera (e.g., Arducam OV2640)</li>
  <li>[ ] Secure user authentication (Firebase Auth)</li>
  <li>[ ] Offline storage and sync</li>
</ul>

<h2>🧠 Goals</h2>
<p>This project is built as a real-world academic application with learning goals in:</p>
<ul>
  <li>Embedded systems (C / FreeRTOS)</li>
  <li>Python hardware control</li>
  <li>Cloud integration</li>
  <li>Mobile app development</li>
  <li>Full-stack IoT architecture</li>
</ul>

<h2>📸 Screenshots</h2>
<p><em>Add image links or placeholders here</em></p>

<h2>📜 License</h2>
<p>MIT License</p>

<h2>🤝 Contributions</h2>
<p>PRs and issues are welcome! Fork the repo and submit a pull request if you'd like to improve or extend the project.</p>

</body>
</html>
