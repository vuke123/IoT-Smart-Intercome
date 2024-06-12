import os
from flask import Flask, abort, send_file
import random
import cv2
from models.model import load_model, load_class_names, preprocess_image, postprocess_detections

app = Flask(__name__)

model_path = "models/yolov4"
class_names_path = "models/coco.names"

model = load_model(model_path)
class_names = load_class_names(class_names_path)

IMAGE_1 = os.path.join('images', 'fly.jpg')
IMAGE_2 = os.path.join('images', 'person.jpg')
IMAGE_3 = os.path.join('images', 'person1.jpg')

selected_image_front = ""
selected_image_back = ""

#slike za ha
@app.route('/front-door/get_image', methods=['GET'])
def get_image_front():
    global selected_image_front
    if os.path.exists(IMAGE_1) and os.path.exists(IMAGE_2):
        selected_image_front = IMAGE_2 if random.random() < 0.5 else IMAGE_1 if random.random() < 0.5 else IMAGE_3
        person_detected = process_image(selected_image_front)
        print(selected_image_front)
        if person_detected:
            return send_file(selected_image_front, mimetype='image/jpeg')
        else:
            return "No person detected."
    else:
        abort(404, description="One or both images not found")


@app.route('/back-door/get_image', methods=['GET'])
def get_image_back():
    global selected_image_back
    if os.path.exists(IMAGE_1) and os.path.exists(IMAGE_2):
        selected_image_back = IMAGE_2 if random.random() < 0.5 else IMAGE_1 if random.random() < 0.5 else IMAGE_3
        person_detected = process_image(selected_image_back)
        print(selected_image_back)
        if person_detected:
            return send_file(selected_image_back, mimetype='image/jpeg')
        else:
            return "No person detected."
    else:
        abort(404, description="One or both images not found")

#slike za app
@app.route('/back-door/image', methods=['GET'])
def image_back():
    global selected_image_back
    print(selected_image_back)
    return send_file(selected_image_back, mimetype='image/jpeg')

@app.route('/front-door/image', methods=['GET'])
def image_front():
    global selected_image_front
    return send_file(selected_image_front, mimetype='image/jpeg')


def process_image(selected_image):
    image = cv2.imread(selected_image)
    image_height, image_width, _ = image.shape

    blob = preprocess_image(image)
    model.setInput(blob)
    detections = model.forward()

    processed_detections = postprocess_detections(detections, class_names, image_width, image_height)

    for detection in processed_detections:
        class_id, confidence, box = detection
        if class_names[class_id] == 'person' and confidence > 0.5:
            return True
    return False

if __name__ == '_main_':
    app.run(host='0.0.0.0', port=5003)
