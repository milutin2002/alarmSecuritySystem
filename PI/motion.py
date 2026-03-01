import time
import RPi.GPIO as GPIO
from config import MOTION_PIN
from camera import capture_image
from firebase_service import firebase_upload, remove_file

GPIO.setmode(GPIO.BCM)
GPIO.setup(MOTION_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


def motion_detected(channel):
    print("Capturing image")

    image_path = capture_image()
    firebase_upload(image_path)
    remove_file(image_path)

    time.sleep(5)


def init_motion():
    GPIO.add_event_detect(
        MOTION_PIN,
        GPIO.RISING,
        callback=motion_detected,
        bouncetime=200
    )


def cleanup():
    GPIO.cleanup()
