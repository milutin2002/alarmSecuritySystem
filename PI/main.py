import RPi.GPIO as GPIO
import time
from dotenv import load_dotenv
import os
import firebase_admin
from firebase_admin import credentials, storage, db
import cv2 as cv
from datetime import datetime
from pyfcm import FCMNotification
from firebase_admin import credentials, messaging
import serial

load_dotenv()


api_key_var=os.getenv("API_KEY")

#serial config
ser=serial.Serial("/dev/serial0",115200,timeout=1)

#GPIO setup
#GPIO.setmode(GPIO.BCM)
#GPIO.setup(26,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)

def capture_image():
	cam=cv.VideoCapture(0)
	ret,image=cam.read()
	timestamp=datetime.now().strftime("%Y%m%d_%H%M%S")
	filename=f"/home/milutin/alarmSecuritySystem/PI/{timestamp}.jpg"
	cv.imwrite(filename,image)
	cam.release()
	return filename

def firebase_upload(image_path):
	global api_key
	bucket= storage.bucket()
	blob=bucket.blob(f"captured_images/{os.path.basename(image_path)}")
	blob.upload_from_filename(image_path)
	blob.make_public()
	image_url=blob.public_url
	message = messaging.Message(
    		notification=messaging.Notification(
        	title="Motion detection",
        	body="An intruder was detected"
    		),
    		data={  
        	"image_url": image_url
    		},
    		topic="alerts"
)
	response=messaging.send(message)
	print(response)
	print(f"Image uploaded: {image_url}")
	db.reference("events").push({
		'timestamp':datetime.now().isoformat(),
		'image_url':image_url
	})
def remove_file(image_path):
	os.remove(image_path)

def motion_detected():
	print("Capturing image\n")
	image_path=capture_image()
	firebase_upload(image_path)
	remove_file(image_path)
	time.sleep(5)

#GPIO.add_event_detect(26,GPIO.RISING,callback=motion_detected)


#FIREBASE setup

firebase_cred_path=os.getenv("FIREBASE_CRED_PATH")
firebase_bucket=os.getenv("FIREBASE_BUCKET")
firebase_db=os.getenv("FIREBASE_DB")

cred=credentials.Certificate(firebase_cred_path)
firebase_admin.initialize_app(cred,{
	'storageBucket':firebase_bucket,
	'databaseURL':firebase_db
})


try:
	while True:
		data=ser.readline().decode(errors="ignore").strip()
		if data and data=="Yes":
			print(data)
			motion_detected()
		time.sleep(1)
except KeyboardInterrupt:
	GPIO.cleanup()
