import cv2
import numpy as np

def load_model(model_path):
    net = cv2.dnn.readNetFromDarknet(model_path + ".cfg", model_path + ".weights")
    return net

def load_class_names(class_names_path):
    with open(class_names_path, "r") as f:
        class_names = f.read().strip().split("\n")
    return class_names

def preprocess_image(image):
    blob = cv2.dnn.blobFromImage(image, 1 / 255.0, (416, 416), swapRB=True, crop=False)
    return blob

def postprocess_detections(detections, image_width, image_height):
    processed_detections = []
    for detection in detections:
        scores = detection[5:]
        class_id = np.argmax(scores)
        confidence = scores[class_id]
        if confidence > 0.5:
            box = detection[0:4] * np.array([image_width, image_height, image_width, image_height])
            (centerX, centerY, box_width, box_height) = box.astype("int")
            x = int(centerX - (box_width / 2))
            y = int(centerY - (box_height / 2))
            processed_detections.append((class_id, confidence, (x, y, int(box_width), int(box_height))))
    return processed_detections

