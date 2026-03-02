import time
import os
import threading
import cv2 as cv
from datetime import datetime
from picamera2 import Picamera2
from config import IMAGE_SAVE_DIR

camera = Picamera2()
camera.start()
time.sleep(1)

lock = threading.Lock()


def generateFrame():
    while True:
        with lock:
            frame = camera.capture_array()

        frame = cv.cvtColor(frame, cv.COLOR_RGB2BGR)
        ret, jpeg = cv.imencode(".jpg", frame)
        bytes = jpeg.tobytes()

        yield (b"--frame\r\n"
               b"Content-Type: image/jpeg\r\n\r\n" +
               bytes + b"\r\n")


def capture_image():
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"{IMAGE_SAVE_DIR}/{timestamp}.jpg"

    time.sleep(1)

    with lock:
        camera.capture_file(filename)

    return filename
