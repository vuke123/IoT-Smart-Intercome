import os
from flask import Flask, jsonify, request
import random
import cv2
import requests
from models.model import load_model, load_class_names, preprocess_image, postprocess_detections


app = Flask(__name__)

model_path = "models/yolov4"
class_names_path = "models/coco.names"

model = load_model(model_path)
class_names = load_class_names(class_names_path)

images = ["images/person.jpg", "images/fly.jpg"]

def send_image_to_application(image_path, app_url):
    files = {'image': open(image_path, 'rb')}
    response = requests.post(app_url, files=files)
    if response.status_code == 200:
        print("Image sent to application successfully.")
    else:
        print(f"Failed to send image to application: {response.status_code}")


@app.route('/process_image', methods=['POST'])
def process_image():
    selected_image = random.choice(images)
    
    image = cv2.imread(selected_image)

    image_height, image_width, _ = image.shape
    
    blob = preprocess_image(image)
    
    model.setInput(blob)
    detections = model.forward()

    processed_detections = postprocess_detections(detections, class_names, image_width,  image_height)

    for detection in processed_detections:
        class_id, confidence, box = detection
        
        # Ako je detektirana osoba
        if class_names[class_id] == 'person' and confidence > 0.5:

            #PROMIJENITI IP ADRESU 
            app_url = "http://IP_ADRESA_APP:5002/receive_image"

            send_image_to_application(selected_image, app_url)

            # Vrati odgovor sa slikom na HA
            image_url = request.host_url + 'images/' + os.path.basename(selected_image)
            return jsonify({"message": "Person detected.", "image_url": image_url})
   
    
    return jsonify({"message": "No person detected."})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5003)
