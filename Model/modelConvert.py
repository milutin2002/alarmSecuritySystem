# Export YOLOv8n model to ONNX
from ultralytics import YOLO

model = YOLO("yolov8n.pt")  # or yolov8n.yaml
model.export(format="onnx")