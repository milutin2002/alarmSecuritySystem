import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(26,GPIO.IN)

def motion_detected(channel):
	print("Motion detected by Pico and send to pi")
	time.sleep(5)

GPIO.add_event_detect(26,GPIO.RISING,callback=motion_detected)

try:
	while True:
		time.sleep(1)
except KeyboardInterrupt:
	GPIO.cleanup()
