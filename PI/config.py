import os
from dotenv import load_dotenv

load_dotenv()

FIREBASE_CRED_PATH = os.getenv("FIREBASE_CRED_PATH")
FIREBASE_BUCKET = os.getenv("FIREBASE_BUCKET")
FIREBASE_DB = os.getenv("FIREBASE_DB")

IMAGE_SAVE_DIR = "/home/milutin/alarmSecuritySystem/PI"
MOTION_PIN = 26
