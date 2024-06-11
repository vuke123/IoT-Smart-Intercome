import random

class MotionSensor:
    def sense_motion(self):
        return random.choices([1, 0], weights=(3,7), k=1)[0]
        #return 1
