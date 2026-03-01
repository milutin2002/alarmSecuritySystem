import os
from datetime import datetime
import firebase_admin
from firebase_admin import credentials, storage, db, messaging
from config import FIREBASE_CRED_PATH, FIREBASE_BUCKET, FIREBASE_DB

cred = credentials.Certificate(FIREBASE_CRED_PATH)
firebase_admin.initialize_app(cred, {
    'storageBucket': FIREBASE_BUCKET,
    'databaseURL': FIREBASE_DB
})


def firebase_upload(image_path):
    bucket = storage.bucket()
    blob = bucket.blob(f"captured_images/{os.path.basename(image_path)}")

    blob.upload_from_filename(image_path)
    blob.make_public()

    image_url = blob.public_url

    message = messaging.Message(
        notification=messaging.Notification(
            title="Motion detection",
            body="An intruder was detected"
        ),
        data={"image_url": image_url},
        topic="alerts"
    )

    response = messaging.send(message)
    print(response)
    print(f"Image uploaded: {image_url}")

    db.reference("events").push({
        'timestamp': datetime.now().isoformat(),
        'image_url': image_url
    })


def remove_file(image_path):
    if os.path.exists(image_path):
        os.remove(image_path)
