import onnxruntime as ort
import cv2
from datetime import datetime
from config import IMAGE_SAVE_DIR
import numpy as np

model=ort.InferenceSession("yolov8n.onnx")

def preprocess(name):
	image=cv2.imread(name)
	w,h=image.shape[1],image.shape[0]
	img=cv2.resize(image,(640,640))
	img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
	img=img.transpose(2,0,1)
	img=img.reshape(1,3,640,640)
	img=img/255.0
	img = img.astype(np.float32)
	return img, img_w, img_h

def filter_people(results):
	filter=[]
	for result in results:
		if result[4] > 0.5:
			filter.append(result)
	return filter
def isPerson(name):
	data=preprocess(name)
	print("Detecting")
	output=model.run(None,{"images":data})
	results=output[0][0]
	results=results.transpose()
	filtet=filter_people(results)
	print("Result")
	has=False
	filename=None
	for r in filter:
		for box in r.boxes:
			cls= int(box.cls[0])
			conf=float(box.conf[0])
			if cls==0 and conf >0.5:
				has=True
				x1,y1,x2,y2=map(int,box.xyxy[0])
				cv2.rectangle(frame,(x1,y1),(x2,y2),(0,255,0),2)
				print("Person detected with confidence: ",conf)
	if has:
		timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
		filename = f"{IMAGE_SAVE_DIR}/{timestamp}.jpg"
		cv2.imwrite(filename,frame)
	return filename
